#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include "Constants.h"
#include "Protocol/Packet.h"

class ICommand;
class User;

class Session: public std::enable_shared_from_this<Session>
{
public:
	explicit Session(std::shared_ptr<User> user, boost::asio::io_context& io);
	~Session();

	boost::asio::ip::tcp::socket& GetSocket();

	void Start();
	void Send(const ICommand& command); //thread-safe

private:
	void readHeader();
	void readPacketHead(size_t bytes, const boost::system::error_code& error, std::shared_ptr<Session>& self);
	void readBody(std::shared_ptr<Session> self);
	void readPacketBody(const boost::system::error_code& error, const size_t bodySize);
	void closeSession();
	void requestDisconnect();

	std::weak_ptr<User> mUser;
	boost::asio::ip::tcp::socket mSocket;
	boost::asio::strand<boost::asio::io_context::executor_type> mStrand;
	std::vector<uint8_t> mReadBuffer;
	std::vector<uint8_t> mWriteBuffer;
	Packet mPacket;

	static constexpr std::size_t HEADER_SIZE = sizeof(PacketHeader);
};
