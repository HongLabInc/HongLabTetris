#pragma once

#include <boost/locale.hpp>
#include "Command/ACommandStandardMessage.h"
#include "Translation.h"
#include "Hub/ClientHub.h"

namespace s2c
{
    class Welcome final: public ACommandStandardMessage
    {
    public:
        Welcome(std::string_view welcomeMessage)
        {
			mMessage = welcomeMessage;
        }

        Welcome() = default;
        virtual ~Welcome() = default;

		virtual void Execute(uint32_t requestUserID, Hub& hub) final
		{
			ClientHub& clientHub = static_cast<ClientHub&>(hub);
			if (clientHub.GetEventManager().GetState() == IManager::eClientState::WaitResponseFromServer)
			{
				clientHub.GetEventManager().AddEventToMainLoop
				([this, self = shared_from_this(), clientHubPtr = clientHub.shared_from_this()]()
				 {
					 auto clientHub = static_cast<ClientHub&>(*clientHubPtr);
					 clientHub.GetEventManager().PrintToOutputPanel(
						 boost::locale::conv::utf_to_utf<wchar_t>(mMessage));
					 clientHub.GetEventManager().SetState(IManager::eClientState::WritingNickName);
				 });
			}
		}

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_Welcome;
        }

    private:
    };
}
