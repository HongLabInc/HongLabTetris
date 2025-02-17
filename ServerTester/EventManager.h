#pragma once

#include <memory>
#include <boost/asio.hpp>
#include "Hub/ClientHub.h"
#include "Command/Commands.h"

class EventManager
{
public :
	using IO = boost::asio::io_context;
	enum class eState // StateMachine
	{
		Main,
		WaitResponseFromServer,
		WritingNickName,
		WaitNickNameResponse,
		Lobby,
	};

	EventManager(IO& main, IO& input, IO& network);
	virtual ~EventManager() final = default;

	void SetState(eState state);
	eState GetState() const;

	void SetClientHub(std::shared_ptr<ClientHub> clientHub);

	void RunMain(); //thread-safe
	void SendCommandToServer(const ICommand& command); //thread-safe
	void ProcessConsoleInput(std::wstring_view text); //thread-safe

private:
	eState mState = eState::Main;
	std::shared_ptr<ClientHub> mClientHub;
	IO& mMainIO;
	IO& mInputIO;
	IO& mNetworkIO;
};
