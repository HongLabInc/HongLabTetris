#include "Server.h"
#include "ClientSession.h"
#include "GameRoom.h"

#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

//
// 생성자, 소멸자 및 기본 초기화
//
Server::Server()
    : acceptor(nullptr)
{}

Server::~Server() {
    Stop();
    if (ioThread.joinable()) {
        ioThread.join();
    }
}

bool Server::InitializeAsServer(unsigned short port) {
    try {
        tcp::endpoint endpoint(tcp::v4(), port);
        acceptor = std::make_unique<tcp::acceptor>(ioContext, endpoint);
        
        StartAccept();
        
        // io_context를 별도 스레드에서 실행
        ioThread = std::thread([this]() {
            ioContext.run();
        });
    } catch (std::exception& e) {
        std::cerr << "InitializeAsServer 예외: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Server::Stop() {
    ioContext.stop();
    if (acceptor) {
        boost::system::error_code ec;
        acceptor->close(ec);
    }
    CloseAllSessions();
}

void Server::SetMessageReceivedCallback(MessageReceivedCallback callback) {
    messageReceivedCallback = callback;
}

//
// 전체 클라이언트에게 메시지 브로드캐스트 (각 ClientSession::deliver() 호출)
//
void Server::Broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    for (auto& session : clientSessions) {
        session->deliver(message);
    }
}

//
// 특정 소켓에 메시지 전송 (비동기 쓰기)
// (ClientSession의 deliver()를 사용하는 것이 권장되지만, 기존 인터페이스를 위해 구현)
//
void Server::SendToSocket(tcp::socket* socketPtr, const std::string& message) {
    if (socketPtr && socketPtr->is_open()) {
        auto writeBuffer = std::make_shared<std::string>(message);
        boost::asio::async_write(*socketPtr,
            boost::asio::buffer(*writeBuffer),
            [writeBuffer](const boost::system::error_code& ec, std::size_t /*bytesTransferred*/) {
            if (ec) {
                std::cerr << "SendToSocket async_write 오류: " << ec.message() << std::endl;
            }
        }
        );
    }
}

//
// 방 관리 관련 함수
//
std::shared_ptr<GameRoom> Server::GetRoom(const std::string& roomName) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    auto it = rooms.find(roomName);
    if (it != rooms.end()) {
        return it->second;
    }
    return nullptr;
}

void Server::JoinRoom(std::shared_ptr<ClientSession> session, const std::string& roomName) {
    std::shared_ptr<GameRoom> room;
    {
        std::lock_guard<std::mutex> lock(roomsMutex);
        auto it = rooms.find(roomName);
        if (it == rooms.end()) {
            room = std::make_shared<GameRoom>(roomName);
            rooms[roomName] = room;
        } else {
            room = it->second;
        }
    }
    if (room) {
        // GameRoom은 세션을 추가하는 AddClient()를 제공한다고 가정
        room->AddClient(session);
        session->setRoomName(roomName);
    }
}

void Server::LeaveRoom(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    for (auto& pair : rooms) {
        // GameRoom은 세션 제거를 위한 RemoveClient()를 제공한다고 가정
        pair.second->RemoveClient(session);
    }
    session->setRoomName("");
}

void Server::ListRooms(std::shared_ptr<ClientSession> session) {
    std::string roomList = "Rooms: ";
    {
        std::lock_guard<std::mutex> lock(roomsMutex);
        for (auto& pair : rooms) {
            roomList += pair.first + " ";
        }
    }
    session->deliver(roomList);
}

//
// 연결 수락 관련 함수 
//
void Server::StartAccept() {
    // 새 ClientSession 생성 (내부에 소켓 생성)
    auto session = std::make_shared<ClientSession>(ioContext);
    std::cout << "New Client try to connection" << std::endl;
    acceptor->async_accept(session->socket(),
        [this, session](const boost::system::error_code& error) {
        HandleAccept(session, error);
    }
    );
}

void Server::HandleAccept(std::shared_ptr<ClientSession> session, const boost::system::error_code& error) {
    if (!error) {
        std::cout << "New Client is connected" << std::endl;
        {
            std::lock_guard<std::mutex> lock(sessionsMutex);
            clientSessions.push_back(session);
            std::cout << "Total Clients: " << clientSessions.size() << std::endl;

        }
        // ClientSession의 메시지 처리 콜백을 서버의 messageReceivedCallback과 연결
        session->setMessageHandler([this](const std::string& message, std::shared_ptr<ClientSession> session) {
            if (messageReceivedCallback) {
                messageReceivedCallback(message, &session->socket());
            }
        });
        // 세션 내에서 비동기 읽기를 시작 (내부적으로 doRead() 호출)
        session->start();
    } else {
        std::cerr << "연결 수락 오류: " << error.message() << std::endl;
    }
    
    std::cout << "Wait Next client" << std::endl;
    StartAccept();
}

//
// 세션 제거 및 소켓 종료
//
void Server::RemoveSession(std::shared_ptr<ClientSession> session) {
    {
        std::lock_guard<std::mutex> lock(sessionsMutex);
        auto it = std::remove(clientSessions.begin(), clientSessions.end(), session);
        clientSessions.erase(it, clientSessions.end());
    }
    boost::system::error_code ec;
    session->socket().shutdown(tcp::socket::shutdown_both, ec);
    session->socket().close(ec);
}

//
// 모든 클라이언트 세션 종료
//
void Server::CloseAllSessions() {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    for (auto& session : clientSessions) {
        boost::system::error_code ec;
        session->socket().shutdown(tcp::socket::shutdown_both, ec);
        session->socket().close(ec);

        std::cout << "Session is closed" << std::endl;
    }
    clientSessions.clear();
}
