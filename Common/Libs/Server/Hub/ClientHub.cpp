#include <iostream>
#include <string>
#include <string_view>
#include <boost/asio/ip/tcp.hpp>
#include "ClientHub.h"
#include "User/User.h"
#include "User/Session.h"
#include "Translation.h"

ClientHub::ClientHub(boost::asio::io_context& io, IManager& eventManager)
	: Hub(1, io, 1)
	, mEventManager(eventManager)
{
}

void ClientHub::Connect(std::string_view ip, unsigned short port)
{
	if (mConnected)
	{
		return;
	}
	auto user = std::make_shared<User>(1, mIO, shared_from_this());
	auto& socket = user->CreateSession()->GetSocket();

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ip), port);

	socket.async_connect
	(endpoint,
	[this, user](const boost::system::error_code& error)
	{
		if (!error)
		{
			mUsers.emplace(1, std::move(user));
			user->SessionStart();
			mConnected = true;
		}
		else
		{
			mEventManager.PrintToOutputPanel(translation::KEY_SERVER_UNAVAILABLE);
			mEventManager.SetState(IManager::eClientState::Main);
			mEventManager.PrintToOutputPanel(translation::CLIENT_MAIN_ENTRY);
			CloseConnection();
		}
	});
}

void ClientHub::CloseConnection()
{
	mConnected = false;
	ShutDown();
}

IManager& ClientHub::GetEventManager()
{
	return mEventManager;
}

void ClientHub::AddEventToMainLoop(std::function<void(void)> callback)
{
	mEventManager.AddEventToMainLoop(std::move(callback));
}
