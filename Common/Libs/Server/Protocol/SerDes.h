#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "Command/Commands.h"

class Packet;

class SerDes
{
public:
	SerDes() = default;
	~SerDes() = default;

	void ParsePacket(const Packet& packet);
	bool IsSuccessful() const;

	std::shared_ptr<ICommand> DeserializePacket() const;

	static std::vector<uint8_t> SerializeCommand(const ICommand& command);

private:
	bool mParsed = false;
	uint16_t mTypeID = NUM_TYPE_ENUMS;
	std::vector<uint8_t> mPacketBody;
	static constexpr uint16_t NUM_TYPE_ENUMS = static_cast<uint16_t>(ICommand::eCommandType::Count);
};
