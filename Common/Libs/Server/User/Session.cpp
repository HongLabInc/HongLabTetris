#include <iostream>
#include <cassert>
#include "Session.h"
#include "Protocol/Packet.h"
#include "Protocol/SerDes.h"
#include "User/User.h"
#include "Command/Commands.h"

Session::Session(std::shared_ptr<User> user, boost::asio::io_context& io)
	: mUser(user)
	, mSocket(io)
	, mStrand(boost::asio::make_strand(io))
{
	mReadBuffer.reserve(constants::TCP_PROTOCOL_INITIAL_READ_BUFFER_SIZE);
	mWriteBuffer.reserve(constants::TCP_PROTOCOL_INITIAL_READ_BUFFER_SIZE);
}

Session::~Session()
{
	mSocket.close();
}

boost::asio::ip::tcp::socket& Session::GetSocket()
{
	return mSocket;
}

void Session::Start()
{
	readHeader();
}

void Session::Send(const ICommand& command)
{
	auto self(shared_from_this());

	mWriteBuffer = Packet::Serialize(SerDes::SerializeCommand(command));

	boost::asio::async_write(mSocket, boost::asio::buffer(mWriteBuffer), boost::asio::bind_executor
	(mStrand,
	 [this, self = std::move(self)](const boost::system::error_code& error, std::size_t bytes)
	 {
		 if (error)
		 {
			 return closeSession();
		 }
	 }
	));
}

void Session::readHeader()
{
	auto self(shared_from_this());

	mReadBuffer.resize(HEADER_SIZE);

	boost::asio::async_read(mSocket, boost::asio::buffer(mReadBuffer), boost::asio::bind_executor
	(mStrand,
	 [this, self = std::move(self)](const boost::system::error_code& error, std::size_t bytes) mutable
	 {
		 readPacketHead(bytes, error, self);
	 }
	));
}

void Session::readPacketHead(size_t bytes, const boost::system::error_code& error, std::shared_ptr<Session>& self)
{
	if (error)
	{
		return requestDisconnect();
	}
	assert(bytes == HEADER_SIZE && "Header size is not correct");

	mPacket.Reset();
	if (mPacket.ParseHeader(mReadBuffer.data()) == false)
	{
		return requestDisconnect();
	}

	readBody(std::move(self));
}

void Session::readBody(std::shared_ptr<Session> self)
{
	const PacketHeader& header = mPacket.GetHeader();

	assert(header.packetTotalLength >= HEADER_SIZE && "Packet size is not correct");
	const std::size_t bodySize = header.packetTotalLength - HEADER_SIZE;
	mReadBuffer.resize(bodySize);

	boost::asio::async_read(mSocket, boost::asio::buffer(mReadBuffer), boost::asio::bind_executor
	(mStrand,
	 [this, self = std::move(self), bodySize](const boost::system::error_code& error, std::size_t bytes)
	 {
		 readPacketBody(error, bodySize);
	 }
	));
}

void Session::readPacketBody(const boost::system::error_code& error, const size_t bodySize)
{
	if (error)
	{
		std::cerr << "Error reading body: " << error.message() << std::endl;
		return requestDisconnect();
	}

	if (mPacket.ParseBody(mReadBuffer.data(), bodySize) == false)
	{
		return requestDisconnect();
	}

	SerDes packetSerializer;
	packetSerializer.ParsePacket(mPacket);
	if (packetSerializer.IsSuccessful() == false)
	{
		return requestDisconnect();
	}

	if (std::shared_ptr<User> user = mUser.lock())
	{
		user->RequestAddEvent(packetSerializer.DeserializePacket());
	}

	readHeader();
}

void Session::closeSession()
{
	if (std::shared_ptr<User> user = mUser.lock())
	{
		user->CloseConnection();
	}
}

void Session::requestDisconnect()
{
	Send(common::NotifyDisconnect(
		translation::WLiteralToStr(translation::KEY_CONNECTION_TERMINATED)));
	closeSession();
}

