#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <atomic>
#include "Hub/TicketBooth.h"

class Server
{
public:
	using WorkGuard = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
	explicit Server(boost::asio::io_context& io, unsigned short port, WorkGuard& workGuard);
	~Server() = default;

	void ShutDownServer();
	void ShutDownServerForced();
	bool IsForcedShutdownRequested() const;

private:
	std::atomic<bool> mForcedShutdownRequested = false;
	WorkGuard mWorkGuard;
	boost::asio::io_context& mIO;
	std::shared_ptr<TicketBooth> mTicketBooth;
};
