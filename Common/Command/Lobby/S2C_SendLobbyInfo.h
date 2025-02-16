#pragma once

#include "Command/ACommandStandardDoubleString.h"
#include "Translation.h"

class Hub;

namespace S2C
{
    class SendLobbyInfo final: public ACommandStandardDoubleString
    {
    public:
        SendLobbyInfo(std::string_view title, std::string_view notice)
        {
            mFirst = Translation::translate(title);
            mSecond = Translation::translate(notice);
        }

        SendLobbyInfo() = default;
        virtual ~SendLobbyInfo() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_SendLobbyInfo;
        }

    private:
    };
}
