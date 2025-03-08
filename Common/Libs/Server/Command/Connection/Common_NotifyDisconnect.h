#pragma once

#include "Command/ACommandStandardMessage.h"
#include "Translation.h"

class Hub;

namespace common
{
    class NotifyDisconnect final: public ACommandStandardMessage
    {
    public:
        NotifyDisconnect(std::string_view message)
        {
			mMessage = message;
        }

        NotifyDisconnect() = default;
        virtual ~NotifyDisconnect() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
            hub.CloseConnection();
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::Common_NotifyDisconnect;
        }

    private:
    };
}
