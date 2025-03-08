#pragma once

#include "Command/ACommandStandardSignal.h"
#include "Translation.h"
#include "Hub/ClientHub.h"

class EventManager;

namespace s2c
{
    class NotifyLobbyEntry final: public ACommandStandardSignal
    {
    public:
        NotifyLobbyEntry() = default;
        virtual ~NotifyLobbyEntry() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
			ClientHub& clientHub = static_cast<ClientHub&>(hub);
			if (clientHub.GetEventManager().GetState() == IManager::eClientState::WaitNickNameResponse)
			{
				clientHub.GetEventManager().PrintToOutputPanel(translation::KEY_LOBBY_ENTRY);
                clientHub.GetEventManager().ScrollOutputPanel(3);
				clientHub.GetEventManager().AddEventToMainLoop
				([this, self = shared_from_this(), clientHubPtr = clientHub.shared_from_this()]()
				 {
					 auto clientHub = static_cast<ClientHub&>(*clientHubPtr);
					 clientHub.GetEventManager().SetState(IManager::eClientState::Lobby);
					 clientHub.GetEventManager().SendCommandToServer(c2s::RequestLobbyInfo());
				 });
                
			}
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_NotifyLobbyEntry;
        }

    private:
    };
}
