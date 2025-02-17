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
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_SendChatMessage;
        }

    private:
    };
}
