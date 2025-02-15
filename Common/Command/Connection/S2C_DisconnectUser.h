#pragma once

#include "Command/ACommandStandardMessage.h"
#include "Translation.h"

class Hub;

namespace S2C
{
    class DisconnectUser final: public ACommandStandardMessage
    {
    public:
        DisconnectUser(std::string_view message)
        {
            mMessage = Translation::translate(message);
        }

        DisconnectUser() = default;
        virtual ~DisconnectUser() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_DisconnectUser;
        }

    private:
    };
}
