#include <iostream>
#include "TicketBooth.h"
#include "User/User.h"
#include "User/Session.h"
#include "Constants.h"
#include "Lobby.h"
#include "Command/Commands.h"

TicketBooth::TicketBooth(uint32_t id, Server& server, boost::asio::io_context& io,
						 std::size_t roomSize, unsigned short port)
	: Hub(id, io, roomSize)
	, mServer(server)
	, mAcceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

void TicketBooth::CreateLobby()
{
	constexpr uint32_t lobbyID = 1; //현재는 서버에 로비 1개만 상주

	mLobby = std::make_shared<Lobby>(lobbyID, mIO, constants::SERVER_HUB_DEFAULT_SIZE);
	mLobby->SetParent(shared_from_this());
}

void TicketBooth::SetUserName(uint32_t userID, std::string_view name)
{
	if (HasUser(userID))
	{
		mUsers[userID]->SetName(name);
	}
}

void TicketBooth::StartAccept()
{
	auto user = std::make_shared<User>(mNextUserID, mIO, shared_from_this());
	boost::asio::ip::tcp::socket& socket = user->CreateSession()->GetSocket();

	mAcceptor.async_accept
	(socket, boost::asio::bind_executor
	(mStrand,
	 [this, user](boost::system::error_code error)
	 {
		 if (!error)
		 {
			 mUsers.emplace(mNextUserID, std::move(user));
			 mUsers[mNextUserID++]->SessionStart();
			 boost::asio::dispatch
			 (mStrand,
			  [this, user]()
			  {
				  user->SendCommandToSession(
					  s2c::Welcome(translation::KEY_WELCOME_MESSAGE));
			  });
		 }
		 StartAccept();
	 }));
}

void TicketBooth::AddUserEvent(uint32_t userID)
{
}

void TicketBooth::RemoveUserEvent(uint32_t userID)
{
}

void TicketBooth::ShutDown()
{
	mAcceptor.close();
	mUsers.clear();
	mLobby.reset();
}

std::shared_ptr<Lobby> TicketBooth::GetLobby()
{
	return mLobby;
}

void TicketBooth::RequestCompletionHandler(bool canMove, uint32_t userID)
{
	if (canMove == false)
	{
		mUsers[userID]->SendCommandToSession(
			common::NotifyDisconnect(translation::KEY_SERVER_UNAVAILABLE));
		RemoveUser(userID);
	}
}

void TicketBooth::AcceptCompletionHandler(bool canAccept, uint32_t userID)
{
}
