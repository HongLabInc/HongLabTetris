#pragma once

#include "Command/ACommandStandardStringList.h"
#include "Translation.h"

class Hub;

// TODO: userID, userName 쌍을 묶어서 클라이언트에게 전송
namespace S2C
{
    class SendLobbyUserList final: public ACommandStandardStringList
    {
    public:
        SendLobbyUserList(std::string_view message)
        {
        }

        SendLobbyUserList() = default;
        virtual ~SendLobbyUserList() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_SendLobbyUserList;
        }

    private:
    };
}
