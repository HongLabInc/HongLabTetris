#include "C2S_RequestLobbyInfo.h"
#include "Hub/Hub.h"
#include "Hub/Lobby.h"
#include "User/User.h"
#include "User/Session.h"
#include "S2C_SendLobbyInfo.h"
#include "S2C_SendLobbyUserList.h"

void C2S::RequestLobbyInfo::Execute(uint32_t requestUserID, Hub& hub)
{
	auto lobby = static_cast<Lobby&>(hub);

	if (lobby.HasUser(requestUserID))
	{
		lobby.SendCommandToUser(requestUserID,
								S2C::SendLobbyInfo(lobby.GetTitle(), lobby.GetNotice()));
		// TODO: 로비 유저 목록 보내주기
	}
}
