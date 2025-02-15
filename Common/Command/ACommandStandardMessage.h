#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include "Command/ICommand.h"

class Hub;

class ACommandStandardMessage: public ICommand
{
public:
	ACommandStandardMessage() = default;
	virtual ~ACommandStandardMessage() = default;

	virtual void Init(const std::vector<uint8_t>& packetBody) final;
	virtual bool IsFailed() const final;
	virtual std::vector<uint8_t> Serialize() const final;

	virtual void Execute(uint32_t requestUserID, Hub& hub) override = 0;

	virtual constexpr eCommandType GetType() const override = 0;
protected:
	std::string mMessage;
};
