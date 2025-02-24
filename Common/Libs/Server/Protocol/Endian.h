#pragma once

#include <cstdint>
#include <bit>

namespace Endian
{
	constexpr uint16_t SwapBytes(uint16_t value)
	{
		if constexpr (std::endian::native == std::endian::big) return value;

		return static_cast<uint16_t>((value >> 8) | (value << 8));
	}

	constexpr uint32_t SwapBytes(uint32_t value)
	{
		if constexpr (std::endian::native == std::endian::big) return value;

		return ((value & 0x000000FFU) << 24) |
			((value & 0x0000FF00U) << 8) |
			((value & 0x00FF0000U) >> 8) |
			((value & 0xFF000000U) >> 24);
	}

	constexpr uint64_t SwapBytes(uint64_t value)
	{
		if constexpr (std::endian::native == std::endian::big) return value;

		return ((value & 0x00000000000000FFULL) << 56) |
			((value & 0x000000000000FF00ULL) << 40) |
			((value & 0x0000000000FF0000ULL) << 24) |
			((value & 0x00000000FF000000ULL) << 8) |
			((value & 0x000000FF00000000ULL) >> 8) |
			((value & 0x0000FF0000000000ULL) >> 24) |
			((value & 0x00FF000000000000ULL) >> 40) |
			((value & 0xFF00000000000000ULL) >> 56);
	}
}
