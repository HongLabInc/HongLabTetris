#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "Protocol/Endian.h"
#include "Protocol/Packet.h"
#include "Constants.h"

class Hub;

class ICommand
{
public:
	enum class eCommandType: uint16_t
	{
		Common_NotifyDisconnect,
		S2C_Welcome,
		S2C_NotifyLobbyEntry,
		S2C_SendLobbyInfo,
		S2C_SendLobbyUserList,
		S2C_SendChatMessage,
		C2S_RequestLogin,
		C2S_RequestLobbyInfo,
		C2S_SendChatMessage,
		Count
	};

	ICommand() = default;
	virtual ~ICommand() = default;

	virtual void Init(const std::vector<uint8_t>& packetBody) = 0;
	virtual bool IsFailed() const = 0;
	virtual std::vector<uint8_t> Serialize() const = 0;

	virtual void Execute(uint32_t requestUserID, Hub& hub) = 0;
	virtual constexpr eCommandType GetType() const = 0;

protected:
	bool mIsFailed = false;

	static constexpr std::size_t MAX_DATA_BODY_LENGTH
		= constants::TCP_PROTOCOL_MAX_PACKET_SIZE
		- sizeof(PacketHeader)
		- sizeof(eCommandType);
};
