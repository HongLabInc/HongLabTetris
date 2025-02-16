#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include "Hub.h"

class Server;
class Session;
class User;
class GameRoom;

class Lobby final: public Hub
{
public:
	explicit Lobby(uint32_t id, Server& server, boost::asio::io_context& io,
				std::size_t roomSize);
	virtual ~Lobby() = default;

	void CreateGameRoom();

	void SetTitle(std::string_view title);
	std::string GetTitle() const;

	void SetNotice(std::string_view notice);
	std::string GetNotice() const;
	
	void SetParent(std::shared_ptr<Hub> ticketBooth);

	virtual void AddUserEvent(uint32_t userID) final;
	virtual void RemoveUserEvent(uint32_t userID) final;

	virtual void ShutDown() final;

	virtual void RequestCompletionHandler(bool canMove, uint32_t userID) final;
	virtual void AcceptCompletionHandler(bool canAccept, uint32_t userID) final;

private:
	uint32_t mNextGameRoomID = 1;
	std::weak_ptr<Hub> mParentHub;
	std::string mTitle;
	std::string mNotice;
	std::unordered_map<uint32_t, std::shared_ptr<GameRoom> > mGameRooms;
};
