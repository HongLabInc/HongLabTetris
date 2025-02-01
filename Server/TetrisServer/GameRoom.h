#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <string>
#include <algorithm>
#include "ClientSession.h"

class GameRoom {
public:
    explicit GameRoom(const std::string &name);

    // 클라이언트 추가/제거 (Server에서 호출)
    void AddClient(std::shared_ptr<ClientSession> session);
    void RemoveClient(std::shared_ptr<ClientSession> session);

    // 방에 속한 모든 참가자에게 메시지 브로드캐스트
    void Broadcast(const std::string &message);

    // 방 이름 반환
    const std::string & GetName() const;

private:
    std::string roomName;
    std::vector<std::shared_ptr<ClientSession>> participants;
    std::mutex roomMutex;
};
