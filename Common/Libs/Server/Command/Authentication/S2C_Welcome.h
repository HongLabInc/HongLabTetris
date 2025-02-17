#pragma once

#include "Command/ACommandStandardMessage.h"
#include "Translation.h"

class Hub;

namespace s2c
{
    class Welcome final: public ACommandStandardMessage
    {
    public:
        Welcome(std::string_view message)
        {
            mMessage = translation::translate(message);
        }

        Welcome() = default;
        virtual ~Welcome() = default;

        virtual void Execute(uint32_t requestUserID, Hub& hub) final
        {
        }

        virtual constexpr eCommandType GetType() const final
        {
            return eCommandType::S2C_Welcome;
        }

    private:
    };
}
