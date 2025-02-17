#include <iostream>
#include <string>
#include <string_view>
#include <boost/asio/ip/tcp.hpp>
#include "ClientHub.h"
#include "User/User.h"
#include "User/Session.h"

ClientHub::ClientHub(boost::asio::io_context& io, ILocalCommandExecutor& localExecutor)
	: Hub(1, io, 1)
{
}

void ClientHub::Connect(std::string_view ip, unsigned short port)
{
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
		}
		else
		{
			CloseConnection();
		}
	});
}

void ClientHub::CloseConnection()
{
	ShutDown();
}
