#include <cassert>
#include <algorithm>
#include <execution>
#include "ACommandStandardStringList.h"

void ACommandStandardStringList::Init(const std::vector<uint8_t>& packetBody)
{
	constexpr std::size_t elemSize = constants::STRING_BLOCK_SIZE;

	assert(packetBody.size() <= MAX_DATA_BODY_LENGTH);
	std::size_t size = packetBody.size();
	if (size % elemSize != 0) // size가 0인 경우도 허용
	{
		mIsFailed = true;
		return;
	}
	size /= elemSize;
	
	mData.resize(size);

	assert(packetBody.size() == size * elemSize);
	std::memcpy(mData.data(), packetBody.data(), packetBody.size());
}

bool ACommandStandardStringList::IsFailed() const
{
	return mIsFailed;
}

std::vector<uint8_t> ACommandStandardStringList::Serialize() const
{
	constexpr std::size_t elemSize = constants::STRING_BLOCK_SIZE;

	std::size_t size = std::min(mData.size() * elemSize, MAX_DATA_BODY_LENGTH);
	assert(size <= MAX_DATA_BODY_LENGTH);

	size /= elemSize;

	std::vector<uint8_t> output(size * elemSize);
	std::memcpy(output.data(), mData.data(), size * elemSize);

	return output;
}
