#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "Command/ICommand.h"
#include "Constants.h"

class Hub;

class ACommandStandardNumericData: public ICommand
{
public:
	union Number32
	{
		uint32_t Int32;
		float Float;
	};

	ACommandStandardNumericData() = default;
	virtual ~ACommandStandardNumericData() = default;

	virtual void Init(const std::vector<uint8_t>& packetBody) final;
	virtual bool IsFailed() const final;
	virtual std::vector<uint8_t> Serialize() const final;

	virtual void Execute(uint32_t requestUserID, Hub& hub) override = 0;

	virtual constexpr eCommandType GetType() const override = 0;

protected:
	std::vector<Number32> mData;

	static constexpr std::size_t MAX_DATA_SIZE = MAX_DATA_BODY_LENGTH / sizeof(Number32);
};


// NOTE
// 현대 네트워킹에서 MTU는 IPv4 576바이트, IPv6 1280 바이트가 사용됩니다.
// IPv4 MTU 576 바이트 - IPv4 헤더 20바이트 - TCP 헤더 20바이트 = 536바이트
// Packet 클래스의 PacketHeader 12바이트와 SerDes 클래스의 헤더 2바이트를 제외한 크기는 522바이트입니다.
// 
// 따라서 패킷 파편화를 막아야 할 경우, 상속 클래스에서 mData 사이즈는 130 이하를 사용하는 것이 좋습니다.
