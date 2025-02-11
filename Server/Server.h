// Server.h
#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <functional>
#include <string>

// 전방 선언 (구현은 별도의 헤더(ClientSession.h, GameRoom.h)에서 관리)
class ClientSession;
class GameRoom;

// 서버에서 메시지 수신 시 호출할 콜백 함수 타입
// (전달인자로 메시지와 해당 메시지를 보낸 소켓 포인터를 전달)
using MessageReceivedCallback = std::function<void(const std::string&, boost::asio::ip::tcp::socket*)>;

class Server {
public:
    Server();
    ~Server();

    // 지정된 포트에서 서버 초기화 (포트 바인딩 및 수락 시작)
    bool InitializeAsServer(unsigned short port);

    // 서버 정지 (io_context 종료 및 모든 세션 닫기)
    void Stop();

    // 메시지 수신 콜백 설정
    void SetMessageReceivedCallback(MessageReceivedCallback callback);

    // 전체 클라이언트에게 메시지 브로드캐스트
    void Broadcast(const std::string& message);

    // 특정 소켓에 메시지 전송
    void SendToSocket(boost::asio::ip::tcp::socket* socketPtr, const std::string& message);

    // [방 관리 기능]
    // 지정된 이름의 방을 가져옴 (없으면 nullptr 반환)
    std::shared_ptr<GameRoom> GetRoom(const std::string& roomName);
    // 클라이언트 세션을 해당 방에 참가시킴 (방이 없으면 새로 생성)
    void JoinRoom(std::shared_ptr<ClientSession> session, const std::string& roomName);
    // 클라이언트 세션이 참가 중인 방에서 퇴장
    void LeaveRoom(std::shared_ptr<ClientSession> session);
    // 현재 서버에 존재하는 방 목록을 클라이언트에 전송
    void ListRooms(std::shared_ptr<ClientSession> session);

private:
    // 비동기 IO를 위한 io_context 객체
    boost::asio::io_context ioContext;
    // 클라이언트 연결 수락용 acceptor
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    // 현재 연결되어 있는 클라이언트 세션들
    std::vector<std::shared_ptr<ClientSession>> clientSessions;
    // 클라이언트 세션 목록 접근 동기화를 위한 mutex
    std::mutex sessionsMutex;
    // io_context 실행을 위한 스레드
    std::thread ioThread;
    // 메시지 수신 시 호출할 콜백 함수
    MessageReceivedCallback messageReceivedCallback;

    // 방 관리를 위한 컨테이너 (방 이름 -> GameRoom)
    std::map<std::string, std::shared_ptr<GameRoom>> rooms;
    std::mutex roomsMutex;

    // 연결 수락 및 데이터 처리를 위한 내부 함수들
    void StartAccept();
    void HandleAccept(std::shared_ptr<ClientSession> session, const boost::system::error_code& error);

    // 클라이언트 세션에서 데이터 읽기를 시작
    void StartRead(std::shared_ptr<ClientSession> session);
    // 클라이언트로부터 메시지를 읽은 후 처리
    void HandleRead(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, std::size_t bytesTransferred);

    // 지정된 클라이언트 세션에 메시지 쓰기를 시작
    void StartWrite(std::shared_ptr<ClientSession> session, const std::string& message);
    // 쓰기 완료 후 후속 처리
    void HandleWrite(std::shared_ptr<ClientSession> session, const boost::system::error_code& error, std::size_t bytesTransferred);

    // 세션 제거 및 소켓 종료
    void RemoveSession(std::shared_ptr<ClientSession> session);
    // 모든 클라이언트 세션 종료
    void CloseAllSessions();
};

