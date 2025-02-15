#pragma once

#include "Command/ACommandStandardMessage.h"
#include "Hub/Hub.h"
#include "Hub/TicketBooth.h"
#include "Hub/Lobby.h"
#include "Constants.h"
#include "User/User.h"
#include "User/Session.h"
#include "Command/Connection/S2C_DisconnectUser.h"

namespace C2S
{
	class RequestLogin final: public ACommandStandardMessage
	{
	public:
		RequestLogin(std::string_view name)
		{
			mMessage = name;
		}

		RequestLogin() = default;
		virtual ~RequestLogin() = default;

		virtual void Execute(uint32_t requestUserID, Hub& hub) final
		{
			if (hub.HasUser(requestUserID))
			{
				// TODO: 이후 보안 절차를 추가
				TicketBooth& booth = static_cast<TicketBooth&>(hub);
				std::shared_ptr<Lobby> lobby = booth.GetLobby();

				assert(hub.IsJoinable() == true);
				// 현재는 무조건 접속을 승인
				if (hub.IsJoinable() == false)
				{
					booth.SendCommandToUser(requestUserID,
											S2C::DisconnectUser("현재 서버 접속이 불가능합니다."));
					return;
				}

				booth.SetUserName(requestUserID,
								  mMessage.substr(0, Constants::STRING_BLOCK_SIZE));

				booth.RequestMoveUser(requestUserID, std::move(lobby));
			}
		}

		virtual constexpr eCommandType GetType() const final
		{
			return eCommandType::C2S_RequestLogin;
		}
	private:
	};
}
