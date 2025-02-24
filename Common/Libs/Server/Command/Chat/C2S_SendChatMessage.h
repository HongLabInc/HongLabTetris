#pragma once

#include "Command/ACommandStandardMessage.h"
#include "Hub/Hub.h"
#include "Constants.h"
#include "User/User.h"
#include "User/Session.h"

namespace c2s
{
	class SendChatMessage final: public ACommandStandardMessage
	{
	public:
		SendChatMessage(std::string_view message)
		{
			mMessage = message;
		}

		SendChatMessage() = default;
		virtual ~SendChatMessage() = default;

		virtual void Execute(uint32_t requestUserID, Hub& hub) final
		{
			if (hub.HasUser(requestUserID))
			{
				hub.BroadcastChatMessage(requestUserID, mMessage);
			}
		}

		virtual constexpr eCommandType GetType() const final
		{
			return eCommandType::C2S_SendChatMessage;
		}
	private:
	};
}
