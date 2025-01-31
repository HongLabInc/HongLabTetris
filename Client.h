#pragma once

#include <boost/asio.hpp>
#include <winsock2.h>
#include <memory>
#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <string>

class Client: public std::enable_shared_from_this<Client> {
public:
    using MessageReceivedCallback = std::function<void(const std::string&)>;

    Client();
    ~Client();

    bool InitializeAsClient(const std::string& host, unsigned short port);
    void SetMessageReceivedCallback(MessageReceivedCallback callback);
    void Send(const std::string& message);
    void Stop();

private:
    struct ServerSession: public std::enable_shared_from_this<ServerSession> {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket;
        boost::asio::streambuf readBuffer;
        std::deque<std::string> writeQueue;
        boost::asio::strand<boost::asio::io_context::executor_type> strand;

        ServerSession(boost::asio::io_context& ioContext)
            : socket(std::make_shared<boost::asio::ip::tcp::socket>(ioContext)),
            strand(ioContext.get_executor()) {}
    };

    void StartConnect(const std::string& host, unsigned short port);
    void HandleConnect(const boost::system::error_code& error);
    void StartRead();
    void HandleRead(const boost::system::error_code& error, std::size_t bytesTransferred);
    void StartWrite();
    void HandleWrite(const boost::system::error_code& error, std::size_t bytesTransferred);
    void CloseConnection();

    boost::asio::io_context ioContext;
    std::unique_ptr<boost::asio::ip::tcp::resolver> resolver;
    std::shared_ptr<ServerSession> session;
    std::thread ioThread;

    std::mutex writeMutex;
    MessageReceivedCallback messageReceivedCallback_;
    bool isConnected;
};
