#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include "Command/ICommand.h"
#include "Constants.h"

class Hub;

class ACommandStandardStringList: public ICommand
{
public:
	using StringBlock = std::array<uint8_t, Constants::STRING_BLOCK_SIZE>;

	ACommandStandardStringList() = default;
	virtual ~ACommandStandardStringList() = default;

	virtual void Init(const std::vector<uint8_t>& packetBody) final;
	virtual bool IsFailed() const final;
	virtual std::vector<uint8_t> Serialize() const final;

	virtual void Execute(uint32_t requestUserID, Hub& hub) override = 0;

	virtual constexpr eCommandType GetType() const override = 0;

protected:
	std::vector<StringBlock> mData;

	static constexpr std::size_t MAX_DATA_SIZE
		= MAX_DATA_BODY_LENGTH / Constants::STRING_BLOCK_SIZE;
};
