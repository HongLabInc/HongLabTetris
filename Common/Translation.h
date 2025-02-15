#pragma once

//이후 Boost Locale 등 다국어 지원 모듈을 붙일 수 있도록 한 랩핑입니다.
#include <string>
#include <string_view>

namespace Translation
{
	inline std::string translate(std::string_view message)
	{
		return std::string(message);
	}
}
