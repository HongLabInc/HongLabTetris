#pragma once

#include "Command/ACommandStandardSignal.h"
#include "Hub/Hub.h"

namespace C2S
{
	class RequestLogout final: public ACommandStandardSignal
	{
	public:
		RequestLogout() = default;
		virtual ~RequestLogout() = default;

		virtual void Execute(uint32_t requestUserID, Hub& hub) final
		{
			if (hub.HasUser(requestUserID))
			{
				hub.RemoveUser(requestUserID);
			}
		}

		virtual constexpr eCommandType GetType() const final
		{
			return eCommandType::C2S_RequestLogout;
		}
	private:
	};
}
