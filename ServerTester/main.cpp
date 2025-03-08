#include <cassert>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>
#include <format>
#include <boost/asio.hpp>
#include <boost/locale.hpp>
#include "Consoles.h"
#include "Hub/ClientHub.h"
#include "Constants.h"
#include "EventManager.h"
#include "Translation.h"

#ifdef _WIN32
#include <Windows.h>
#endif

std::atomic<bool> gRunning = true;

namespace
{
void setLocale();
void start();
}

int main()
{
	try
	{
		setLocale();
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
void setLocale()
{
	boost::locale::generator gen;
	std::locale loc = gen("UTF-8");
	std::locale::global(loc);
	std::wcin.imbue(loc);
	std::wcout.imbue(loc);

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
}

void start()
{
	std::size_t consoleHeight = 15;
	std::size_t consoleWidth = 80;
	auto console = std::make_shared<Console>(consoleHeight, consoleWidth);
	console->Init();

	auto panelOutput = console->AddPanel(0, 0, consoleHeight - 1, consoleWidth);
	auto panelInput = console->AddPanel(consoleHeight - 1, 0, 1, 100);

	boost::asio::io_context ioMain;
	boost::asio::io_context ioInput;
	boost::asio::io_context ioNetwork;
	auto workGuardNetwork = boost::asio::make_work_guard(ioNetwork);

	EventManager eventManager(ioMain, ioInput, ioNetwork);
	eventManager.SetConsole(console);
	eventManager.SetOutputPanel(panelOutput);
	assert(eventManager.GetState() == EventManager::eState::Main && "State is not Main");

	auto clientHub = std::make_shared<ClientHub>(ioNetwork, eventManager);
	eventManager.SetClientHub(clientHub);

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

	panelOutput->AppendLines(std::format(
		L"DEBUG: 서버 접속 아이피가 {}:{}로 설정되었습니다.",
		boost::locale::conv::utf_to_utf<wchar_t>(constants::DEFAULT_SERVER_IP),
		constants::DEFAULT_PORT
	)); //TEST LOG
	panelOutput->AppendLines(translation::CLIENT_MAIN_ENTRY);

	std::jthread inputThread
	([&eventManager, panel = std::move(panelInput)]()
	{
		eventManager.pollInput(panel);
	});

	eventManager.RunMainLoop();

	ioNetwork.stop();

	console->Clear();
	console->MoveCursor(0, 0);
	console->WriteString("Client terminated.");
}
}
