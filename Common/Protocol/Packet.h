#pragma once

#include <cstdint>
#include <vector>
#include "Constants.h"

struct PacketHeader
{
    uint32_t magic;     // 클라이언트가 실제로 보낸 메시지가 length보다 길 경우 다음 메시지에서 필터링
    uint16_t version;   // 프로토콜 버전
    uint16_t packetTotalLength;    // 전체 패킷 크기(헤더+바디)
    uint16_t checksum;  // 현재는 로직 생략(무조건 0)
    // 바디는 헤더 바로 뒤에 붙음(가변 길이)
};

class Packet
{
public:
    Packet(void) = default;
    ~Packet(void) = default;

    void Reset();

    bool ParseHeader(const uint8_t* buffer);
    bool ParseBody(const uint8_t* bodyData, std::size_t bodyLength);

    const PacketHeader& GetHeader() const;
    const std::vector<uint8_t>& GetBody() const;

    static std::vector<uint8_t> Serialize(const std::vector<uint8_t>& body);

private:
    static uint16_t calculateChecksum(const uint8_t* bodyData, std::size_t bodyLength);

	static constexpr std::size_t HEADER_SIZE = sizeof(PacketHeader);
    static constexpr std::size_t MAX_PACKET_SIZE = Constants::TCP_PROTOCOL_MAX_PACKET_SIZE;
	static constexpr uint32_t MAGIC_NUMBER = 0xABCD1234;
	static constexpr uint16_t VERSION = 1;

    PacketHeader mHeader = {};
    std::vector<uint8_t> mBody;
    bool mIsHeaderParsed = false;
};
