#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include <string>
#include <string_view>
#include "Hub.h"

class Server;
class Session;
class User;
class Lobby;

class TicketBooth final: public Hub
{
public:
	explicit TicketBooth(uint32_t id, Server& server, boost::asio::io_context& io,
				std::size_t roomSize, unsigned short port);
	virtual ~TicketBooth() = default;

	void CreateLobby();
	void SetUserName(uint32_t userID, std::string_view name);

	void StartAccept();

	virtual void AddUserEvent(uint32_t userID) override;
	virtual void RemoveUserEvent(uint32_t userID) override;

	virtual void ShutDown() final;

	std::shared_ptr<Lobby> GetLobby();

	virtual void RequestCompletionHandler(bool canMove, uint32_t userID) final;
	virtual void AcceptCompletionHandler(bool canAccept, uint32_t userID) final;

private:
	void acceptConnection(std::shared_ptr<User>& user, boost::system::error_code error);

	Server& mServer;
	boost::asio::ip::tcp::acceptor mAcceptor;
	uint32_t mNextUserID = 1; // TODO: ID가 오버플로우 되는 경우 고려하기
	std::shared_ptr<Lobby> mLobby; // 추후 필요하다면 std::vector 형태로 여러 로비를 둘 수 있음
};
