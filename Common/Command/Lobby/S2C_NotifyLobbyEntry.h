#pragma once

#include "Command/ACommandStandardSignal.h"
#include "Translation.h"

class Hub;

namespace S2C
{
    class NotifyLobbyEntry final: public ACommandStandardSignal
    {
    public:
        NotifyLobbyEntry() = default;
        virtual ~NotifyLobbyEntry() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_NotifyLobbyEntry;
        }

    private:
    };
}
