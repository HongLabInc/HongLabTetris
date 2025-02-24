#pragma once

#include "Command/ACommandStandardSignal.h"
#include "Hub/Hub.h"

namespace c2s
{
	class RequestLobbyInfo final: public ACommandStandardSignal
	{
	public:
		RequestLobbyInfo() = default;
		virtual ~RequestLobbyInfo() = default;

		virtual void Execute(uint32_t requestUserID, Hub& hub) final;

		virtual constexpr eCommandType GetType() const final
		{
			return eCommandType::C2S_RequestLobbyInfo;
		}
	private:
	};
}
