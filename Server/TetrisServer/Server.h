#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <string>
#include <deque>

class Server: public std::enable_shared_from_this<Server> {
public:
    using MessageReceivedCallback = std::function<void(const std::string&, boost::asio::ip::tcp::socket*)>;

    Server();
    ~Server();


    bool InitializeAsServer(unsigned short port);
    void SetMessageReceivedCallback(MessageReceivedCallback callback);
    void Broadcast(const std::string& message);
    void SendToSocket(boost::asio::ip::tcp::socket* socketPtr, const std::string& message);
    void Stop();

private:
    // 내부적으로 클라이언트 세션을 관리하는 구조체
    struct ClientSession: public std::enable_shared_from_this<ClientSession> {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket;
        boost::asio::streambuf readBuffer; // streambuf로 변경
        std::deque<std::string> writeQueue;
        boost::asio::strand<boost::asio::io_context::executor_type> strand;

        ClientSession(boost::asio::io_context& ioContext)
            : socket(std::make_shared<boost::asio::ip::tcp::socket>(ioContext)),
            strand(ioContext.get_executor()) {}
    };

    void StartAccept();
    void HandleAccept(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const boost::system::error_code& error);
    void StartRead(std::shared_ptr<ClientSession> session);
    void HandleRead(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, std::size_t bytesTransferred);
    void StartWrite(std::shared_ptr<ClientSession> session, const std::string& message);
    void HandleWrite(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, std::size_t bytesTransferred);
    std::shared_ptr<boost::asio::ip::tcp::socket> GetSocketFromPointer(boost::asio::ip::tcp::socket* socketPtr);
    void CloseAllSessions();
    void RemoveSession(std::shared_ptr<ClientSession> session);


    boost::asio::io_context ioContext;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    std::thread ioThread;

    std::vector<std::shared_ptr<ClientSession>> clientSessions;
    std::mutex sessionsMutex;

    MessageReceivedCallback messageReceivedCallback;
};

