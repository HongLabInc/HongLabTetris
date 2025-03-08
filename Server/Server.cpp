#include <iostream>
#include "Server.h"
#include "Constants.h"
#include "boost/asio.hpp"

Server::Server(boost::asio::io_context& io, unsigned short port, WorkGuard& workGuard)
	: mWorkGuard(workGuard)
	, mIO(io)
	, mTicketBooth(std::make_shared<TicketBooth>(1, *this, io,
												 constants::SERVER_HUB_DEFAULT_SIZE, port))
{
	mTicketBooth->CreateLobby();
	mTicketBooth->StartAccept();
}

void Server::ShutDownServer()
{
	mWorkGuard.reset();
	mTicketBooth->ShutDown();
	mTicketBooth.reset();
}

void Server::ShutDownServerForced()
{
	mForcedShutdownRequested = true;
	mIO.stop();
}

bool Server::IsForcedShutdownRequested() const
{
	return mForcedShutdownRequested;
}
