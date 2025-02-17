#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include "Hub/Hub.h"

class Server;
class Session;
class User;

class GameRoom final: public Hub
{
public:
	explicit GameRoom(uint32_t id, boost::asio::io_context& io,
				std::size_t roomSize);
	virtual ~GameRoom() = default;

	void SetParent(std::shared_ptr<Hub> lobby);

	virtual void AddUserEvent(uint32_t userID) final;
	virtual void RemoveUserEvent(uint32_t userID) final;

	virtual void ShutDown() final;

	virtual void RequestCompletionHandler(bool canMove, uint32_t userID) final;
	virtual void AcceptCompletionHandler(bool canAccept, uint32_t userID) final;

private:
	std::weak_ptr<Hub> mParentHub;
};
