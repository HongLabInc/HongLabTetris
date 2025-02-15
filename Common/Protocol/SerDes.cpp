#include <cassert>
#include <cstring>
#include "Endian.h"
#include "SerDes.h"
#include "Packet.h"

void SerDes::ParsePacket(const Packet& packet)
{
	const std::vector<uint8_t>& packetBody = packet.GetBody();
	if (packetBody.size() < sizeof(uint16_t))
	{
		mParsed = false;
		return;
	}

	assert(packetBody.size() >= sizeof(uint16_t));
	uint16_t id;
	std::memcpy(&id, packetBody.data(), sizeof(uint16_t));
	id = Endian::SwapBytes(id);

	if (id >= NUM_TYPE_ENUMS)
	{
		mParsed = false;
		return;
	}

	assert(packetBody.begin() + sizeof(uint16_t) <= packetBody.end());
	mPacketBody.assign(packetBody.begin() + sizeof(uint16_t), packetBody.end());
	mTypeID = id;
	mParsed = true;
}

bool SerDes::IsSuccessful() const
{
	return mParsed;
}

std::shared_ptr<ICommand> SerDes::DeserializePacket() const
{
	assert(mTypeID < NUM_TYPE_ENUMS);
	std::shared_ptr<ICommand> output = Command::GetCommand(mTypeID);
	output->Init(mPacketBody);

	return output;
}

std::vector<uint8_t> SerDes::SerializeCommand(const ICommand& command)
{
	uint16_t id = Endian::SwapBytes(static_cast<uint16_t>(command.GetType()));

	std::vector<uint8_t> body = command.Serialize();
	std::vector<uint8_t> output(sizeof(uint16_t) + body.size());

	std::memcpy(output.data(), &id, sizeof(uint16_t));
	std::copy(body.begin(), body.end(), output.begin() + sizeof(uint16_t));

	return output;
}
