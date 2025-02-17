#include <cassert>
#include <algorithm>
#include <execution>
#include "ACommandStandardNumericData.h"

void ACommandStandardNumericData::Init(const std::vector<uint8_t>& packetBody)
{
	constexpr std::size_t elemSize = sizeof(uint32_t) / sizeof(uint8_t);

	assert(packetBody.size() <= MAX_DATA_BODY_LENGTH);
	std::size_t size = packetBody.size();
	if (size == 0 || size % elemSize != 0)
	{
		mIsFailed = true;
		return;
	}
	size /= elemSize;
	
	mData.resize(size);
	std::memcpy(mData.data(), packetBody.data(), packetBody.size());

	std::transform
	(std::execution::unseq, mData.begin(), mData.end(), mData.begin(),
	 [](Number32 num) -> Number32
	 {
		 num.Int32 = Endian::SwapBytes(num.Int32);
		 return num;
	 });
}

bool ACommandStandardNumericData::IsFailed() const
{
	return mIsFailed;
}

std::vector<uint8_t> ACommandStandardNumericData::Serialize() const
{
	constexpr std::size_t elemSize = sizeof(uint32_t) / sizeof(uint8_t);

	std::size_t size = std::min(mData.size() * elemSize, MAX_DATA_BODY_LENGTH);

	assert(size <= MAX_DATA_BODY_LENGTH);

	size /= elemSize;

	std::vector<Number32> swap(mData);
	std::transform
	(std::execution::unseq, swap.begin(), swap.begin() + size, swap.begin(),
	 [](Number32 num) -> Number32
	 {
		 num.Int32 = Endian::SwapBytes(num.Int32);
		 return num;
	 });

	std::vector<uint8_t> output(size * elemSize);
	std::memcpy(output.data(), swap.data(), size * elemSize);

	return output;
}
