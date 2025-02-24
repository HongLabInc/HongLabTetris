#include "GameRoom.h"

GameRoom::GameRoom(uint32_t id, boost::asio::io_context& io,
			 std::size_t roomSize)
	: Hub(id, io, roomSize)
{
}

void GameRoom::SetParent(std::shared_ptr<Hub> lobby)
{
	mParentHub = lobby;
}

void GameRoom::AddUserEvent(uint32_t userID)
{
}

void GameRoom::RemoveUserEvent(uint32_t userID)
{
}

void GameRoom::ShutDown()
{
	mUsers.clear();
}

void GameRoom::RequestCompletionHandler(bool canMove, uint32_t userID)
{
}

void GameRoom::AcceptCompletionHandler(bool canAccept, uint32_t userID)
{
}
