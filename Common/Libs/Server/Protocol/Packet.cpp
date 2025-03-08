#include <cassert>
#include "Packet.h"
#include "Endian.h"

void Packet::Reset()
{
	mHeader = {};
	mBody.clear();
	mIsHeaderParsed = false;
}

bool Packet::ParseHeader(const uint8_t* buffer)
{
	assert(buffer != nullptr && "buffer is nullptr");
	const PacketHeader* ptr = reinterpret_cast<const PacketHeader*>(buffer);

	const uint32_t magic = endian::SwapBytes(ptr->magic);
	const uint16_t version = endian::SwapBytes(ptr->version);
	const uint16_t length = endian::SwapBytes(ptr->packetTotalLength);
	const uint16_t checkSum = endian::SwapBytes(ptr->checksum);

	if (magic != MAGIC_NUMBER) return false;
	if (version != VERSION) return false;
	if (length < HEADER_SIZE || length > MAX_PACKET_SIZE) return false;

	mHeader.magic = magic;
	mHeader.version = version;
	mHeader.packetTotalLength = length;
	mHeader.checksum = checkSum;
	mIsHeaderParsed = true;
	return true;
}

bool Packet::ParseBody(const uint8_t* bodyData, std::size_t bodyLength)
{
	assert(bodyData != nullptr && "bodyData is nullptr");
	assert(mIsHeaderParsed == true && "Header is not parsed");

	const uint16_t checkSum = calculateChecksum(bodyData, bodyLength);
	if (checkSum != mHeader.checksum) return false;

	mBody.assign(bodyData, bodyData + bodyLength);
	return true;
}

const PacketHeader& Packet::GetHeader() const
{
	return mHeader;
}

const std::vector<uint8_t>& Packet::GetBody() const
{
	return mBody;
}

std::vector<uint8_t> Packet::Serialize(const std::vector<uint8_t>& body)
{
	const uint8_t* bodyData = body.data();
	const uint16_t bodyLength = static_cast<uint16_t>(body.size());

	constexpr uint32_t magic = endian::SwapBytes(MAGIC_NUMBER);
	constexpr uint16_t version = endian::SwapBytes(VERSION);
	const uint16_t length = endian::SwapBytes(static_cast<uint16_t>(HEADER_SIZE + bodyLength));
	const uint16_t checkSum = calculateChecksum(bodyData, bodyLength);

	const PacketHeader header{magic, version, length, checkSum};
	const uint8_t* headerPtr = reinterpret_cast<const uint8_t*>(&header);

	assert(HEADER_SIZE + bodyLength <= MAX_PACKET_SIZE && "Packet size is too big");
	std::vector<uint8_t> outputPacket;
	outputPacket.reserve(HEADER_SIZE + bodyLength);

	outputPacket.insert(outputPacket.end(), headerPtr, headerPtr + HEADER_SIZE);
	outputPacket.insert(outputPacket.end(), bodyData, bodyData + bodyLength);

	return outputPacket;
}

uint16_t Packet::calculateChecksum(const uint8_t* bodyData, std::size_t bodyLength)
{
	//체크섬 생략, 필요하다면 추후 구현가능
	return 0;
}
