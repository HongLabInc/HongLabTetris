#pragma once

#include <cstdint>
#include <cassert>
#include "ICommand.h"

#include "Authentication/C2S_RequestLogin.h"
#include "Authentication/S2C_Welcome.h"
#include "Chat/C2S_SendChatMessage.h"
#include "Chat/S2C_SendChatMessage.h"
#include "Connection/C2S_RequestLogout.h"
#include "Connection/S2C_DisconnectUser.h"
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
				return std::make_shared<C2S::RequestLogin>();
			case toInt(Type::S2C_Welcome):
				return std::make_shared<S2C::Welcome>();
			case toInt(Type::C2S_SendChatMessage):
				return std::make_shared<C2S::SendChatMessage>();
			case toInt(Type::S2C_SendChatMessage):
				return std::make_shared<S2C::SendChatMessage>();
			case toInt(Type::C2S_RequestLogout):
				return std::make_shared<C2S::RequestLogout>();
			case toInt(Type::S2C_DisconnectUser):
				return std::make_shared<S2C::DisconnectUser>();
			case toInt(Type::C2S_RequestLobbyInfo):
				return std::make_shared<C2S::RequestLobbyInfo>();
			case toInt(Type::S2C_NotifyLobbyEntry):
				return std::make_shared<S2C::NotifyLobbyEntry>();
			case toInt(Type::S2C_SendLobbyInfo):
				return std::make_shared<S2C::NotifyLobbyEntry>();
			case toInt(Type::S2C_SendLobbyUserList):
				return std::make_shared<S2C::NotifyLobbyEntry>();
			default:
				assert(false);
				return nullptr;
		}
	}
}
