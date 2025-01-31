#include "Server.h"
#include <iostream>
#include <algorithm>


Server::Server() {}

Server::~Server() {
    Stop();
}

bool Server::InitializeAsServer(unsigned short port) {

    try {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(ioContext, endpoint);

        StartAccept();

        ioThread = std::thread([this]() {
            ioContext.run();
        });

        std::wcout << L"서버가 포트 " << port << L"에서 시작되었습니다." << std::endl;

        return true;

    } 
    catch (std::exception& e) {
        std::cerr << "서버 초기화 오류: " << e.what() << std::endl;
        return false;
    }
}

void Server::SetMessageReceivedCallback(MessageReceivedCallback callback) {
    messageReceivedCallback = callback;
}

void Server::Broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    for (auto& session : clientSessions) {
        StartWrite(session, message + "\n"); // 메시지 구분자 추가
    }
}

void Server::SendToSocket(boost::asio::ip::tcp::socket* socketPtr, const std::string& message) {
    auto socket = GetSocketFromPointer(socketPtr);
    if (socket) {
        std::lock_guard<std::mutex> lock(sessionsMutex);
        for (auto& session : clientSessions) {
            if (session->socket.get() == socketPtr) {
                StartWrite(session, message + "\n"); // 메시지 구분자 추가
                break;
            }
        }
    }
}

std::shared_ptr<boost::asio::ip::tcp::socket> Server::GetSocketFromPointer(boost::asio::ip::tcp::socket* socketPtr) {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    for (auto& session : clientSessions) {
        if (session->socket.get() == socketPtr) {
            return session->socket;
        }
    }
    return nullptr;
}

void Server::Stop() {
    ioContext.stop();
    if (ioThread.joinable()) {
        ioThread.join();
    }

    CloseAllSessions();
}

void Server::StartAccept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
    acceptor->async_accept(*socket,
        [this, socket](const boost::system::error_code& error) {
        HandleAccept(socket, error);
    }
    );
}

void Server::HandleAccept(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const boost::system::error_code& error) {
    if (!error) {
        auto session = std::make_shared<ClientSession>(ioContext);
        session->socket = socket;

        {
            std::lock_guard<std::mutex> lock(sessionsMutex);
            clientSessions.push_back(session);
        }

        StartRead(session);
        std::cout << "Client is connected" << std::endl;
    } else {
        std::cerr << "수락 오류: " << error.message() << std::endl;
    }

    StartAccept(); // 다음 클라이언트 수락 시작
}

void Server::StartRead(std::shared_ptr<ClientSession> session) {
    boost::asio::async_read_until(*session->socket, session->readBuffer, '\n',
        boost::asio::bind_executor(session->strand,
        [this, session](const boost::system::error_code& error, std::size_t bytesTransferred) {
        HandleRead(session, error, bytesTransferred);
    }
    )
    );
}

void Server::HandleRead(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, std::size_t bytesTransferred) {
    if (!error) {
        std::istream is(&session->readBuffer);
        std::string message;
        std::getline(is, message); // '\n'을 기준으로 메시지 분리

        std::cout << "받은 메시지: " << message << std::endl;

        if (messageReceivedCallback) {
            messageReceivedCallback(message, session->socket.get());
        }

        StartRead(session); // 다음 읽기 시작
    } else {
        if (error == boost::asio::error::eof) {
            std::cout << "클라이언트가 연결을 종료했습니다." << std::endl;
        } else {
            std::cerr << "읽기 오류 또는 연결 종료: " << error.message() << std::endl;
        }
        RemoveSession(session);
    }
}

void Server::StartWrite(std::shared_ptr<ClientSession> session, const std::string& message) {
    boost::asio::post(session->strand, [this, session, message]() {
        bool writeInProgress = !session->writeQueue.empty();
        session->writeQueue.push_back(message);
        if (!writeInProgress) {
            boost::asio::async_write(*session->socket, boost::asio::buffer(session->writeQueue.front()),
                boost::asio::bind_executor(session->strand,
                [this, session](const boost::system::error_code& error, std::size_t /*bytesTransferred*/) {
                HandleWrite(session, error, 0);
            }
            )
            );
        }
    });
}

void Server::HandleWrite(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, std::size_t /*bytesTransferred*/) {
    if (!error) {
        session->writeQueue.pop_front();
        if (!session->writeQueue.empty()) {
            boost::asio::async_write(*session->socket, boost::asio::buffer(session->writeQueue.front()),
                boost::asio::bind_executor(session->strand,
                [this, session](const boost::system::error_code& error, std::size_t /*bytesTransferred*/) {
                HandleWrite(session, error, 0);
            }
            )
            );
        }
    } else {
        std::cerr << "쓰기 오류: " << error.message() << std::endl;
        RemoveSession(session);
    }
}

void Server::CloseAllSessions() {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    for (auto& session : clientSessions) {
        if (session->socket->is_open()) {
            boost::system::error_code ec;
            session->socket->close(ec);
        }
    }
    clientSessions.clear();
}

void Server::RemoveSession(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    auto it = std::remove(clientSessions.begin(), clientSessions.end(), session);
    if (it != clientSessions.end()) {
        clientSessions.erase(it, clientSessions.end());
    }

    if (session->socket->is_open()) {
        boost::system::error_code ec;
        session->socket->close(ec);
    }

    std::cout << "클라이언트 연결이 종료되었습니다." << std::endl;
}
