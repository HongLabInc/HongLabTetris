#pragma once

#include <functional>
#include <vector>
#include <map>

class EventManager {
public:
    // 이벤트 타입 정의 (열거형 사용)
    enum class EventType {
        BlockSpawned,
        LineCleared,
        GameOver,
    };

    // 이벤트 구독
    void Subscribe(EventType eventType,std::function<void()> handler);

    // 이벤트 발생
    void Publish(EventType eventType);

private:
    // 이벤트 타입별 핸들러 목록
    std::map<EventType,std::vector<std::function<void()>>> handlers;
};