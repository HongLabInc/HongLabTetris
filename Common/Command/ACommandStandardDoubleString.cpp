#include <cassert>
#include "ACommandStandardDoubleString.h"

void ACommandStandardDoubleString::Init(const std::vector<uint8_t>& packetBody)
{
	assert(packetBody.size() <= MAX_DATA_BODY_LENGTH);
	if (packetBody.size() <= sizeof(uint16_t))
	{
		mIsFailed = true;
		return;
	}
	uint16_t secondStart = *reinterpret_cast<const uint16_t*>(packetBody.data());
	secondStart = Endian::SwapBytes(secondStart);

	if (packetBody.size() < secondStart)
	{
		mIsFailed = true;
		return;
	}

	mFirst = std::string(packetBody.begin() + sizeof(uint16_t),
						 packetBody.begin() + secondStart);
	mSecond = std::string(packetBody.begin() + secondStart,
						 packetBody.end());
}

bool ACommandStandardDoubleString::IsFailed() const
{
	return mIsFailed;
}

std::vector<uint8_t> ACommandStandardDoubleString::Serialize() const
{
	uint16_t secondStart = static_cast<uint16_t>(
		std::min(sizeof(uint16_t) + mFirst.size(), MAX_DATA_BODY_LENGTH));

	std::size_t size = std::min(secondStart + mSecond.size(),
								MAX_DATA_BODY_LENGTH);
	std::vector<uint8_t> output(size);

	assert(size <= MAX_DATA_BODY_LENGTH);

	uint16_t header = Endian::SwapBytes(secondStart);

	std::memcpy(output.data(), &header, sizeof(header));
	std::memcpy(output.data() + sizeof(header), mFirst.data(),
				secondStart - sizeof(header));
	std::memcpy(output.data() + secondStart, mSecond.data(), size - secondStart);
	return output;
}
