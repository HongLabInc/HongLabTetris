#pragma once

#include <cstdint>
#include <cassert>
#include "ICommand.h"

#include "Authentication/C2S_RequestLogin.h"
#include "Authentication/S2C_Welcome.h"
#include "Chat/C2S_SendChatMessage.h"
#include "Chat/S2C_SendChatMessage.h"
#include "Connection/Common_NotifyDisconnect.h"
#include "Lobby/C2S_RequestLobbyInfo.h"
#include "Lobby/S2C_NotifyLobbyEntry.h"
#include "Lobby/S2C_SendLobbyInfo.h"
#include "Lobby/S2C_SendLobbyUserList.h"

namespace Command
{
	using Type = ICommand::eCommandType;

	constexpr uint16_t NUM_TYPE_ENUMS = static_cast<uint16_t>(Type::Count);

	consteval uint16_t toInt(Type type)
	{
		return static_cast<uint16_t>(type);
	}

	inline std::shared_ptr<ICommand> GetCommand(uint16_t typeID)
	{
		assert(typeID < NUM_TYPE_ENUMS);
		switch (typeID)
		{
			case toInt(Type::C2S_RequestLogin):
				return std::make_shared<c2s::RequestLogin>();
			case toInt(Type::S2C_Welcome):
				return std::make_shared<s2c::Welcome>();
			case toInt(Type::C2S_SendChatMessage):
				return std::make_shared<c2s::SendChatMessage>();
			case toInt(Type::S2C_SendChatMessage):
				return std::make_shared<s2c::SendChatMessage>();
			case toInt(Type::Common_NotifyDisconnect):
				return std::make_shared<common::NotifyDisconnect>();
			case toInt(Type::C2S_RequestLobbyInfo):
				return std::make_shared<c2s::RequestLobbyInfo>();
			case toInt(Type::S2C_NotifyLobbyEntry):
				return std::make_shared<s2c::NotifyLobbyEntry>();
			case toInt(Type::S2C_SendLobbyInfo):
				return std::make_shared<s2c::NotifyLobbyEntry>();
			case toInt(Type::S2C_SendLobbyUserList):
				return std::make_shared<s2c::NotifyLobbyEntry>();
			default:
				assert(false);
				return nullptr;
		}
	}
}
