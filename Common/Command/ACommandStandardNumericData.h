#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "Command/ICommand.h"
#include "Constants.h"

class Hub;

class ACommandStandardNumericData: public ICommand
{
public:
	union Number32
	{
		uint32_t Int32;
		float Float;
	};

	ACommandStandardNumericData() = default;
	virtual ~ACommandStandardNumericData() = default;

	virtual void Init(const std::vector<uint8_t>& packetBody) final;
	virtual bool IsFailed() const final;
	virtual std::vector<uint8_t> Serialize() const final;

	virtual void Execute(uint32_t requestUserID, Hub& hub) override = 0;

	virtual constexpr eCommandType GetType() const override = 0;

protected:
	std::vector<Number32> mData;

	static constexpr std::size_t MAX_DATA_SIZE = MAX_DATA_BODY_LENGTH / sizeof(Number32);
};
