#include "Client.h"
#include <iostream>

Client::Client()
    : resolver(nullptr), session(nullptr), isConnected(false)
{}

Client::~Client()
{
    Stop();
}

bool Client::InitializeAsClient(const std::string& host, unsigned short port)
{
    try {
        resolver = std::make_unique<boost::asio::ip::tcp::resolver>(ioContext);
        session = std::make_shared<ServerSession>(ioContext);
        StartConnect(host, port);

        ioThread = std::thread([this]() { ioContext.run(); });
        return true;
    } catch (std::exception& e) {
        std::cerr << "클라이언트 초기화 실패: " << e.what() << std::endl;
        return false;
    }
}

void Client::SetMessageReceivedCallback(MessageReceivedCallback callback)
{
    messageReceivedCallback_ = callback;
}

void Client::Send(const std::string& message)
{
    if (!isConnected) {
        std::cerr << "서버에 연결되어 있지 않습니다." << std::endl;
        return;
    }

    auto self = shared_from_this();
    boost::asio::post(session->strand, [this, self, message]() {
        bool writeInProgress = !session->writeQueue.empty();
        session->writeQueue.push_back(message);
        if (!writeInProgress) {
            StartWrite();
        }
    });
}

void Client::Stop()
{
    if (isConnected) {
        CloseConnection();
    }

    ioContext.stop();
    if (ioThread.joinable()) {
        ioThread.join();
    }
}

void Client::StartConnect(const std::string& host, unsigned short port)
{
    auto endpoints = resolver->resolve(host, std::to_string(port));
    boost::asio::async_connect(*session->socket, endpoints,
        [this, self = shared_from_this()](const boost::system::error_code& error, const boost::asio::ip::tcp::endpoint&) {
        HandleConnect(error);
    });
}

void Client::HandleConnect(const boost::system::error_code& error)
{
    if (!error) {
        isConnected = true;
        StartRead();
        std::cout << "서버에 성공적으로 연결되었습니다." << std::endl;
    } else {
        std::cerr << "서버 연결 실패: " << error.message() << std::endl;
    }
}

void Client::StartRead()
{
    boost::asio::async_read_until(*session->socket, session->readBuffer, '\n',
        [this, self = shared_from_this()](const boost::system::error_code& error, std::size_t bytesTransferred) {
        HandleRead(error, bytesTransferred);
    });
}

void Client::HandleRead(const boost::system::error_code& error, std::size_t bytesTransferred)
{
    if (!error) {
        std::istream is(&session->readBuffer);
        std::string message;
        std::getline(is, message);

        if (messageReceivedCallback_) {
            messageReceivedCallback_(message);
        }

        StartRead();
    } else {
        std::cerr << "읽기 오류: " << error.message() << std::endl;
        CloseConnection();
    }
}

void Client::StartWrite()
{
    boost::asio::async_write(*session->socket,
        boost::asio::buffer(session->writeQueue.front() + "\n"),
        boost::asio::bind_executor(session->strand,
        [this, self = shared_from_this()](const boost::system::error_code& error, std::size_t bytesTransferred) {
        HandleWrite(error, bytesTransferred);
    }));
}

void Client::HandleWrite(const boost::system::error_code& error, std::size_t bytesTransferred)
{
    if (!error) {
        session->writeQueue.pop_front();
        if (!session->writeQueue.empty()) {
            StartWrite();
        }
    } else {
        std::cerr << "쓰기 오류: " << error.message() << std::endl;
        CloseConnection();
    }
}

void Client::CloseConnection()
{
    if (session->socket->is_open()) {
        boost::system::error_code ec;
        session->socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        session->socket->close(ec);
    }
    isConnected = false;
}
