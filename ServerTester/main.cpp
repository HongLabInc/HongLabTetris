#include <cassert>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <boost/asio.hpp>
#include "Consoles.h"
#include "Hub/ClientHub.h"
#include "Constants.h"
#include "EventManager.h"

std::atomic<bool> gRunning = true;

namespace
{
	void start();
	void pollInput(boost::asio::io_context& networkIO, EventManager& eventManager,
				   std::shared_ptr<Panel> inputPanel);
	void runMainLoop(std::shared_ptr<Console> console, EventManager& eventManager);
}

int main()
{
	try
	{
		start();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Fatal exception in main: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

namespace
{
	void start()
	{
		std::size_t consoleHeight = 30;
		std::size_t consoleWidth = 100;
		auto console = std::make_shared<Console>(consoleHeight, consoleWidth);

		auto panelOutput = console->AddPanel(0, 0, 28, 100);
		auto panelInput = console->AddPanel(28, 0, 2, 100);

		boost::asio::io_context ioMain;
		boost::asio::io_context ioInput;
		boost::asio::io_context ioNetwork;
		auto workGuardNetwork = boost::asio::make_work_guard(ioNetwork);

		EventManager eventManager(ioMain, ioInput, ioNetwork);
		assert(eventManager.GetState() == EventManager::eState::Main);

		auto clientHub = std::make_shared<ClientHub>(ioNetwork, eventManager);
		clientHub->Connect(constants::DEFAULT_SERVER_IP, constants::DEFAULT_PORT);

		std::jthread networkThread([&ioNetwork]()
		{
			try
			{
				ioNetwork.run();
			}
			catch (std::exception& e)
			{
				std::cerr << "[NetworkThread] Exception: " << e.what() << std::endl;
			}
		});

		eventManager.SetClientHub(clientHub);

		std::jthread inputThread([&ioInput, &eventManager, panel = std::move(panelInput)]()
		{
			pollInput(ioInput, eventManager, panel);
		});

		runMainLoop(console, eventManager);

		ioNetwork.stop();

		console->Clear();
		console->MoveCursor(0, 0);
		console->WriteString("Client terminated.");
	}

	void pollInput(boost::asio::io_context& inputIO, EventManager& eventManager,
				   std::shared_ptr<Panel> inputPanel)
	{
		while (gRunning)
		{
			std::wstring line;
			if (!std::getline(std::wcin, line))
			{
				gRunning = false;
				break;
			}
			if (line == L"/quit")
			{
				gRunning = false;
				break;
			}

			inputIO.post([line = std::move(line), &eventManager]()
			{
				eventManager.ProcessConsoleInput(line);
			});
		}
	}

	void runMainLoop(std::shared_ptr<Console> console, EventManager& eventManager)
	{
		while (gRunning)
		{
			using namespace std::chrono_literals;

			eventManager.RunMain();
			console->Refresh();
			std::this_thread::sleep_for(33ms);
		}
	}
}
