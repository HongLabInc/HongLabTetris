#pragma once

#include "ClientSession.h"

// 생성자: 내부 소켓 및 실행 스트랜드를 io_context를 통해 초기화

 ClientSession::ClientSession(boost::asio::io_context & ioContext)
    : socket_(ioContext),
    strand_(boost::asio::make_strand(ioContext))
{}

// 세션 시작: 비동기 읽기 작업을 시작합니다.

 void ClientSession::start() {
    doRead();
}

// 외부에서 전달받은 메시지를 클라이언트에 전송하기 위한 인터페이스

 void ClientSession::deliver(const std::string & message) {
    // writeQueue_에 메시지를 추가하고, 큐가 비어있으면 비동기 쓰기 시작
    auto self(shared_from_this());
    boost::asio::post(strand_,
        [this, self, message]() {
        bool writeInProgress = !writeQueue_.empty();
        writeQueue_.push_back(message);
        if (!writeInProgress) {
            doWrite();
        }
    }
    );
}

// 외부에서 메시지 처리 콜백을 설정하여, 읽은 데이터에 대해 별도의 비즈니스 로직을 처리할 수 있게 합니다.

void ClientSession::setMessageHandler(MessageHandler handler) {
    messageHandler_ = handler;
}

// 소켓에 대한 참조를 반환 (연결 설정 등 외부에서 필요시 사용)

 boost::asio::ip::tcp::socket & ClientSession::socket() {
    return socket_;
}

// 현재 참가중인 방의 이름을 반환합니다.

 const std::string & ClientSession::getRoomName() const {
    return roomName_;
}

// 세션이 참가하는 방 이름을 설정합니다.

 void ClientSession::setRoomName(const std::string & roomName) {
    roomName_ = roomName;
}

// 비동기 읽기 작업: 소켓에서 데이터를 읽고, 완성된 메시지가 있으면 콜백을 호출합니다.

 void ClientSession::doRead() {
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, readBuffer_, '\n',
        boost::asio::bind_executor(strand_,
        [this, self](const boost::system::error_code& ec, std::size_t bytesTransferred) {
        if (!ec) {
            std::istream is(&readBuffer_);
            std::string line;
            std::getline(is, line);
            if (messageHandler_) {
                messageHandler_(line, self);
            }
            doRead();
        }
        // 에러 발생 시 세션 종료 처리는 외부에서 관리하도록 할 수 있음
    }
    )
    );
}

// 비동기 쓰기 작업: writeQueue_에 쌓인 메시지를 순차적으로 전송합니다.

 void ClientSession::doWrite() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
        boost::asio::buffer(writeQueue_.front()),
        boost::asio::bind_executor(strand_,
        [this, self](const boost::system::error_code& ec, std::size_t /*bytesTransferred*/) {
        if (!ec) {
            writeQueue_.pop_front();
            if (!writeQueue_.empty()) {
                doWrite();
            }
        }
        // 에러 발생 시 세션 종료 처리는 외부에서 관리하도록 할 수 있음
    }
    )
    );
}
