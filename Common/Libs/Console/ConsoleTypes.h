#pragma once

#include <cstddef>

namespace console
{
	enum class eColor
	{
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		Default
	};

	struct Cell
	{
		wchar_t character;
		eColor fgColor;
		eColor bgColor;
	};

	struct Rect
	{
		std::size_t top;
		std::size_t left;
		std::size_t height;
		std::size_t width;
	};

	constexpr Cell DefaultCell{L' ', eColor::Default, eColor::Black};
}
