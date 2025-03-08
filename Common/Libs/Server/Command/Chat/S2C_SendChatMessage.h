#pragma once

#include "Command/ACommandStandardDoubleString.h"

class Hub;

namespace s2c
{
    class SendChatMessage final: public ACommandStandardDoubleString
    {
    public:
        SendChatMessage(std::string_view userName, std::string_view message)
        {
            mFirst = userName;
            mSecond = message;
        }

        SendChatMessage() = default;
        virtual ~SendChatMessage() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
			ClientHub& clientHub = static_cast<ClientHub&>(hub);
			if (clientHub.GetEventManager().GetState() == IManager::eClientState::Lobby)
			{
				clientHub.GetEventManager().PrintToOutputPanel(
					boost::locale::conv::utf_to_utf<wchar_t>(mFirst) + L": " +
					boost::locale::conv::utf_to_utf<wchar_t>(mSecond));
			}
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_SendChatMessage;
        }

    private:
    };
}
