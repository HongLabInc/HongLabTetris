#include "Lobby.h"
#include "GameRoom.h"
#include "Constants.h"
#include "User/User.h"
#include "User/Session.h"
#include "Command/Commands.h"

Lobby::Lobby(uint32_t id, Server& server, boost::asio::io_context& io,
			 std::size_t roomSize)
	: Hub(id, server, io, roomSize)
{
}

void Lobby::CreateGameRoom()
{
	auto newRoom = std::make_shared<GameRoom>(mNextGameRoomID,
											  mServer, mIO,
											  Constants::SERVER_GAMEROOM_DEFAULT_SIZE);

	mGameRooms.emplace(mNextGameRoomID, std::move(newRoom));
	mGameRooms[mNextGameRoomID]->SetParent(shared_from_this());
	++mNextGameRoomID;
}

void Lobby::SetTitle(std::string_view title)
{
	mTitle = title;
}

std::string Lobby::GetTitle() const
{
	return mTitle;
}

void Lobby::SetNotice(std::string_view notice)
{
	mNotice = notice;
}

std::string Lobby::GetNotice() const
{
	return mNotice;
}

void Lobby::SetParent(std::shared_ptr<Hub> ticketBooth)
{
	mParentHub = ticketBooth;
}

void Lobby::AddUserEvent(uint32_t userID)
{
	// TODO: 유저 입장 메시지 브로드캐스트
}

void Lobby::RemoveUserEvent(uint32_t userID)
{
	// TODO: 유저 퇴장 메시지 브로드캐스트
}

void Lobby::ShutDown()
{
	mUsers.clear();
}

void Lobby::RequestCompletionHandler(bool canMove, uint32_t userID)
{
}

void Lobby::AcceptCompletionHandler(bool canAccept, uint32_t userID)
{
	if (canAccept)
	{
		mUsers[userID]->SendCommandToSession(S2C::NotifyLobbyEntry());
	}
	else
	{
		// TODO: Lobby 입장 거절된 경우 (현재는 무조건 수용)
		// 기본적으로 RequestMoveUser를 호출하는 Hub에서 IsJoinable로 체크를 하고 요청
		// User가 Lobby로 이동하는 도중에 입장 불가로 바뀐 경우에 대한 예외처리는 이곳에 작성
	}
}
