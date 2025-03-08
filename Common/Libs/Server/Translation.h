#pragma once

//이후 Boost Locale 등 다국어 지원 모듈을 붙일 수 있도록 한 랩핑입니다.
#include <string>
#include <string_view>
#include <boost/locale.hpp>

namespace translation
{
// TODO: 다국어 모듈 추가 이후엔 문자열 내용을 키값으로 변경

	// 서버 관련 문자열
	constexpr wchar_t SERVER_WELCOME_MESSAGE[] = L"HongLab 테트리스 서버입니다. 접속할 닉네임을 입력해 주세요.";
	constexpr wchar_t SERVER_LOBBY_TITLE[] = L"HongLab 테트리스 서버 대기실";
	constexpr wchar_t SERVER_LOBBY_NOTICE[] = L"환영합니다. 이곳은 HongLab 테트리스 대기실입니다.";

	// 클라이언트 관련 문자열
	constexpr wchar_t CLIENT_MAIN_ENTRY[] = L"서버에 접속을 시도하려면 /connect, 종료하려면 /quit을 입력하세요.";
	constexpr wchar_t CLIENT_QUIT[] = L"종료합니다.";

	// 기타 문자열
	constexpr wchar_t KEY_CONNECTION_TERMINATED[] = L"연결이 종료되었습니다.";
	constexpr wchar_t KET_TRY_CONNECT[] = L"서버에 연결을 시도합니다.";
	constexpr wchar_t KEY_WAITING_FOR_SERVER_RESPONSE[] = L"서버의 응답을 기다리는 중입니다.";
	constexpr wchar_t KEY_SERVER_UNAVAILABLE[] = L"서버와 연결할수 없습니다.";
	constexpr wchar_t KEY_ANONYMOUS_USER[] = L"이름없는 유저";
	constexpr wchar_t KEY_LOBBY_ENTRY[] = L"대기실에 입장하였습니다.";
	constexpr wchar_t KEY_TRY_LOGIN_FRONT[] = L"닉네임 ";
	constexpr wchar_t KEY_TRY_LOGIN_BACK[] = L"으로 로그인을 시도합니다.";

	inline std::string Translate(std::string_view key)
	{
		return std::string(key);
	}

	inline std::string WLiteralToStr(std::wstring_view wstr)
	{
		return Translate(boost::locale::conv::utf_to_utf<char>(std::wstring(wstr)));
	}
}
