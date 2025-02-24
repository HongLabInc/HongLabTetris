#pragma once

#include <boost/locale.hpp>
#include "Command/ACommandStandardMessage.h"
#include "Hub/Hub.h"
#include "Hub/TicketBooth.h"
#include "Hub/Lobby.h"
#include "Constants.h"
#include "User/User.h"
#include "User/Session.h"
#include "Command/Connection/Common_NotifyDisconnect.h"

namespace c2s
{
	class RequestLogin final: public ACommandStandardMessage
	{
	public:
		RequestLogin(const std::wstring& name)
		{
			mMessage = boost::locale::conv::from_utf(name, "UTF-8");
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
					 common::NotifyDisconnect(translation::KEY_SERVER_UNAVAILABLE));
					return;
				}

				booth.SetUserName(requestUserID,
								  mMessage.substr(0, constants::STRING_BLOCK_SIZE));

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
