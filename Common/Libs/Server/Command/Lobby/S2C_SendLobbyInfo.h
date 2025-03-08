#pragma once

#include "Command/ACommandStandardDoubleString.h"
#include "Translation.h"
#include "Hub/ClientHub.h"

namespace s2c
{
    class SendLobbyInfo final: public ACommandStandardDoubleString
    {
    public:
        SendLobbyInfo(std::string_view title, std::string_view notice)
        {
            mFirst = title;
            mSecond = notice;
        }

        SendLobbyInfo() = default;
        virtual ~SendLobbyInfo() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
			ClientHub& clientHub = static_cast<ClientHub&>(hub);
			if (clientHub.GetEventManager().GetState() == IManager::eClientState::Lobby)
			{
				clientHub.GetEventManager().PrintToOutputPanel(L"======================================================================");
				clientHub.GetEventManager().PrintToOutputPanel(boost::locale::conv::utf_to_utf<wchar_t>(mFirst));
				clientHub.GetEventManager().ScrollOutputPanel(1);
				clientHub.GetEventManager().PrintToOutputPanel(boost::locale::conv::utf_to_utf<wchar_t>(mSecond));
				clientHub.GetEventManager().PrintToOutputPanel(L"======================================================================");
				clientHub.GetEventManager().ScrollOutputPanel(1);
			}
		}

		virtual constexpr eCommandType GetType() const final
		{
			return eCommandType::S2C_SendLobbyInfo;
		}

	private:
	};
}
