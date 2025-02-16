#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <atomic>
#include "Hub/TicketBooth.h"

class Server
{
public:
	explicit Server(boost::asio::io_context& io, unsigned short port);
	~Server() = default;

	void ShutDownServer();
	void ShutDownServerForced();
	bool IsForcedShutdownRequested() const;

private:
	std::atomic<bool> mForcedShutdownRequested = false;
	boost::asio::io_context& mIO;
	std::shared_ptr<TicketBooth> mTicketBooth;
};
