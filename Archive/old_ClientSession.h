// ClientSession.h
#pragma once

#include <deque>
#include <memory>
#include <string>
#include <functional>
#include <boost/asio.hpp>

// ClientSession은 클라이언트와의 비동기 통신을 관리하는 역할에 집중합니다.
// 메시지 처리 로직이나 방 관리 등 비즈니스 로직은 외부(예: 서버, 게임룸 클래스)에서 콜백을 통해 처리하도록 합니다.
class ClientSession: public std::enable_shared_from_this<ClientSession> {
public:
    // 메시지 처리 콜백: 완성된 메시지와 현재 세션(shared_ptr)을 인자로 전달
    using MessageHandler = std::function<void(const std::string&, std::shared_ptr<ClientSession>)>;

    // 생성자: 내부 소켓 및 실행 스트랜드를 io_context를 통해 초기화
    explicit ClientSession(boost::asio::io_context& ioContext);

    ~ClientSession() = default;

    // 세션 시작: 비동기 읽기 작업을 시작합니다.
    void start();

    // 외부에서 전달받은 메시지를 클라이언트에 전송하기 위한 인터페이스
    void deliver(const std::string& message);

    // 외부에서 메시지 처리 콜백을 설정하여, 읽은 데이터에 대해 별도의 비즈니스 로직을 처리할 수 있게 합니다.
    void setMessageHandler(MessageHandler handler);

    // 소켓에 대한 참조를 반환 (연결 설정 등 외부에서 필요시 사용)
    boost::asio::ip::tcp::socket& socket();

    // 현재 참가중인 방의 이름을 반환합니다.
    const std::string& getRoomName() const;

    // 세션이 참가하는 방 이름을 설정합니다.
    void setRoomName(const std::string& roomName);

private:
    // 비동기 읽기 작업: 소켓에서 데이터를 읽고, 완성된 메시지가 있으면 콜백을 호출합니다.
    void doRead();

    // 비동기 쓰기 작업: writeQueue_에 쌓인 메시지를 순차적으로 전송합니다.
    void doWrite();

    // 소켓과 관련된 비동기 작업을 순차적으로 실행하기 위한 실행 스트랜드
    boost::asio::ip::tcp::socket socket_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;

    // 입력 버퍼: 클라이언트로부터 읽은 데이터 보관
    boost::asio::streambuf readBuffer_;
    // 출력 큐: 클라이언트로 보낼 메시지 저장
    std::deque<std::string> writeQueue_;

    // 현재 세션이 속한 방의 이름 (비즈니스 로직에서 필요시만 사용)
    std::string roomName_;

    // 읽은 메시지에 대해 호출할 콜백 함수 (비즈니스 로직 분리)
    MessageHandler messageHandler_;
};
