#include <thread>
#include "EventManager.h"
#include "Command/Commands.h"

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

void EventManager::RunMain()
{
	mInputIO.poll();
	mMainIO.poll();
}

void EventManager::SendCommandToServer(const ICommand& command)
{
	if (mClientHub->HasUser(1))
	{
		mClientHub->SendCommandToUser(1, command);
	}
}

void EventManager::ProcessConsoleInput(std::wstring_view text)
{
	mNetworkIO.post
	([this, text = std::wstring(text.begin(), text.end())]()
	 {
		 switch (mState)
		 {
			 case eState::Main:
				 break;
			 case eState::WaitResponseFromServer:
				 break;
			 case eState::WritingNickName:
				 SendCommandToServer(c2s::RequestLogin(text));
				 mState = eState::WaitNickNameResponse;
				 break;
			 case eState::WaitNickNameResponse:
				 break;
			 case eState::Lobby:
				 break;
			 default:
				 assert(false);
				 break;
		 }
	 });
}
