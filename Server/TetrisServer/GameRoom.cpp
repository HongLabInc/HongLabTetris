#include "GameRoom.h"

GameRoom::GameRoom(const std::string &name)
    : roomName(name) {}

// 클라이언트를 방에 추가합니다.
void GameRoom::AddClient(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(roomMutex);
    participants.push_back(session);
}

// 클라이언트를 방에서 제거합니다.
void GameRoom::RemoveClient(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(roomMutex);
    auto it = std::remove(participants.begin(), participants.end(), session);
    if (it != participants.end()) {
        participants.erase(it, participants.end());
    }
}

// 방의 모든 참가자에게 메시지를 전송합니다.
void GameRoom::Broadcast(const std::string &message) {
    std::lock_guard<std::mutex> lock(roomMutex);
    for (auto &session : participants) {
        session->deliver(message);
    }
}

const std::string & GameRoom::GetName() const {
    return roomName;
}
