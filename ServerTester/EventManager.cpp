#include <thread>
#include <iostream>
#include <boost/locale.hpp>
#include "EventManager.h"
#include "Command/Commands.h"

#ifdef _WIN32
#include <Windows.h>
#endif

extern std::atomic<bool> gRunning;

EventManager::EventManager(IO& main, IO& input, IO& network)
	: mMainIO(main)
	, mInputIO(input)
	, mNetworkIO(network)
{
}

void EventManager::SetState(eState state)
{
	mState = state;
}

EventManager::eState EventManager::GetState() const
{
	return mState;
}

void EventManager::SetClientHub(std::shared_ptr<ClientHub> clientHub)
{
	mClientHub = std::move(clientHub);
}

void EventManager::SetConsole(std::shared_ptr<Console> console)
{
	mConsole = std::move(console);
}

void EventManager::SetOutputPanel(std::shared_ptr<Panel> outputPanel)
{
	mOutputPanel = std::move(outputPanel);
}

void EventManager::RunMainLoop()
{
	std::mutex& mutex = mConsole->GetMutex();
	while (gRunning)
	{
		using namespace std::chrono_literals;

		mutex.lock();
		mInputIO.poll();
		mMainIO.poll();

		assert(mConsole != nullptr && "Console is nullptr");
		mConsole->Refresh();
		mutex.unlock();

		std::this_thread::sleep_for(16ms);
	}
}

void EventManager::SendCommandToServer(const ICommand& command)
{
	if (mClientHub->HasUser(1))
	{
		mClientHub->SendCommandToUser(1, command);
	}
}

void EventManager::pollInput(std::shared_ptr<Panel> inputPanel)
{
	std::mutex& mutex = mConsole->GetMutex();
	const int consoleHeight = static_cast<int>(mConsole->GetHeight());
	const int consoleWidth = static_cast<int>(mConsole->GetWidth());
	const std::string emptyLine(consoleWidth, ' ');

	while (gRunning)
	{
		mConsole->MoveCursor(consoleHeight - 1, 0);
		std::wstring line;
		if (!std::getline(std::wcin, line))
		{
			std::cerr << "Error reading from console." << std::endl;
			gRunning = false;
			break;
		}

#ifdef _WIN32
		mConsole->MoveCursor(consoleHeight - 1, 0);
		mConsole->WriteString(emptyLine);
#endif

		if (line == L"/quit")
		{
			mutex.lock();
			inputPanel->Clear();
			mOutputPanel->AppendLines(translation::CLIENT_QUIT);
			mConsole->Refresh();
			mutex.unlock();
			gRunning = false;
			break;
		}

		mutex.lock();
		inputPanel->Clear();
		mConsole->Refresh();
		mutex.unlock();

		mInputIO.post
		([this, line = std::move(line)]()
		 {
			 ProcessConsoleInput(line);
		 });
	}
}

void EventManager::AddEventToMainLoop(std::function<void(void)> callback)
{
	boost::asio::post(mMainIO, std::move(callback));
}

void EventManager::PrintToOutputPanel(std::wstring_view message)
{
	boost::asio::post
	(mMainIO,
	 [this, message = std::wstring(message)]()
	 {
		 mOutputPanel->AppendLines(message);
	 });
}

void EventManager::ScrollOutputPanel(std::size_t numLines)
{
	boost::asio::post
	(mMainIO,
	 [this, numLines]()
	 {
		 mOutputPanel->Scroll(numLines);
	 });
}

void EventManager::ProcessConsoleInput(std::wstring_view text)
{
	mNetworkIO.post
	([this, text = std::wstring(text.begin(), text.end())]()
	 {
		 switch (mState)
		 {
			 case eState::Main:
				 if (text == L"/connect")
				 {
					 PrintToOutputPanel(translation::KET_TRY_CONNECT);
					 mClientHub->Connect(constants::DEFAULT_SERVER_IP, constants::DEFAULT_PORT);
					 mState = eState::WaitResponseFromServer;
				 }
				 break;
			 case eState::WaitResponseFromServer:
				 PrintToOutputPanel(translation::KEY_WAITING_FOR_SERVER_RESPONSE);
				 break;
			 case eState::WritingNickName:
				 PrintToOutputPanel(translation::KEY_TRY_LOGIN_FRONT + text + translation::KEY_TRY_LOGIN_BACK);
				 SendCommandToServer(c2s::RequestLogin(boost::locale::conv::utf_to_utf<char>(text)));
				 mState = eState::WaitNickNameResponse;
				 break;
			 case eState::WaitNickNameResponse:
				 PrintToOutputPanel(translation::KEY_WAITING_FOR_SERVER_RESPONSE);
				 break;
			 case eState::Lobby:
				 SendCommandToServer(c2s::SendChatMessage(boost::locale::conv::utf_to_utf<char>(text)));
				 break;
			 default:
				 assert(false && "Invalid state");
				 break;
		 }
	 });
}
