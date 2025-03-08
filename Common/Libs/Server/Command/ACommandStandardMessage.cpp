#include <cassert>
#include "ACommandStandardMessage.h"

void ACommandStandardMessage::Init(const std::vector<uint8_t>& packetBody)
{
	assert(packetBody.size() <= MAX_DATA_BODY_LENGTH && "Data body size is too big");
	
	mMessage = std::string(packetBody.begin(), packetBody.end());
}

bool ACommandStandardMessage::IsFailed() const
{
	return mIsFailed;
}

std::vector<uint8_t> ACommandStandardMessage::Serialize() const
{
	const std::size_t size = std::min(mMessage.size(), MAX_DATA_BODY_LENGTH);

	assert(size <= MAX_DATA_BODY_LENGTH && "Data body size is too big");

	std::vector<uint8_t> output(size);
	std::memcpy(output.data(), mMessage.data(), size);
	return output;
}
