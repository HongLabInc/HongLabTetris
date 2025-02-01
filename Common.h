#pragma once
// Common.h
#pragma once

#include <string>
#include <functional>
#include <boost/asio.hpp>

// 메시지 구분자 (양쪽 모두 동일하게 사용)
constexpr char MESSAGE_DELIMITER = '\n';

// 서버와 클라이언트 모두가 사용할 수 있는 공통 콜백 타입들

// 서버: 메시지와 해당 메시지를 보낸 소켓 포인터를 전달
using ServerMessageReceivedCallback = std::function<void(const std::string&, boost::asio::ip::tcp::socket*)>;

// 클라이언트: 메시지(서버에서 전달된 내용)만 전달
using ClientMessageReceivedCallback = std::function<void(const std::string&)>;

// 메시지 끝에 구분자를 붙이는 유틸리티 함수 (선택 사항)
inline std::string addDelimiter(const std::string& message) {
    return message + MESSAGE_DELIMITER;
}