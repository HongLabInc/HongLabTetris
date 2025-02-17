#pragma once

//이후 Boost Locale 등 다국어 지원 모듈을 붙일 수 있도록 한 랩핑입니다.
#include <string>
#include <string_view>

namespace translation
{
	// TODO: 다국어 모듈 추가 이후엔 문자열 내용을 키값으로 변경
	constexpr char KEY_CONNECTION_TERMINATED[] = "연결이 종료되었습니다.";
	constexpr char KEY_WELCOME_MESSAGE[] = "HongLab 테트리스 서버입니다. 닉네임은 설정해 주세요.";
	constexpr char KEY_SERVER_UNAVAILABLE[] = "현재 서버 접속이 불가능합니다.";
	constexpr char KEY_ANONYMOUS_USER[] = "이름없는 유저";

	inline std::string translate(std::string_view key)
	{
		return std::string(key);
	}
}
