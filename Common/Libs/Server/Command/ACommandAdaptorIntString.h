#pragma once
#include <string>
#include <string_view>
#include <stdexcept>
#include <bit>
#include <cstdint>
#include "ICommand.h"
#include "ACommandStandardDoubleString.h"
#include "Protocol/Endian.h"

class Hub;

class ACommandAdaptorIntString: public ACommandStandardDoubleString
{
public:
	ACommandAdaptorIntString() = default;

	void SetIntString(int intValue, std::string_view str)
	{
		uint32_t swapped = endian::SwapBytes(std::bit_cast<uint32_t>(intValue));
		mFirst.resize(sizeof(uint32_t));
		std::memcpy(mFirst.data(), &swapped, sizeof(uint32_t));
		mSecond = str;
	}

	int GetInt() const
	{
		if (mFirst.size() < sizeof(uint32_t))
		{
			return 0;
		}
		uint32_t swapped;
		std::memcpy(&swapped, mFirst.data(), sizeof(uint32_t));

		static_assert(sizeof(int) == sizeof(uint32_t) && "int is not 32bit");
		return std::bit_cast<int>(endian::SwapBytes(swapped));
	}

	std::string_view GetString() const
	{
		return mSecond;
	}

	virtual constexpr eCommandType GetType() const override = 0;
	virtual void Execute(uint32_t requestUserID, Hub& hub) override = 0;

protected:
};
