#pragma once

#include <cstddef>

namespace constants
{
    constexpr const char DEFAULT_SERVER_IP[] = "127.0.0.1";
	constexpr unsigned short DEFAULT_PORT = 12345;
	constexpr unsigned int DEFAULT_THREAD_NUMBER = 2;

    constexpr std::size_t TCP_PROTOCOL_INITIAL_READ_BUFFER_SIZE = 4096;
    constexpr std::size_t TCP_PROTOCOL_MAX_PACKET_SIZE = 65535;

    constexpr std::size_t SERVER_HUB_DEFAULT_SIZE = 1000;
    constexpr std::size_t SERVER_GAMEROOM_DEFAULT_SIZE = 6;

    constexpr std::size_t STRING_BLOCK_SIZE = 128; // 고정길이 문자열 사이즈
}
