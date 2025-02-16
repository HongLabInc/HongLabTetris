#include "ACommandStandardSignal.h"

void ACommandStandardSignal::Init(const std::vector<uint8_t>& packetBody)
{
}

bool ACommandStandardSignal::IsFailed() const
{
	return mIsFailed;
}

std::vector<uint8_t> ACommandStandardSignal::Serialize() const
{
	return {};
}
