#include "EventManager.h"

void EventManager::Subscribe(EventType eventType,std::function<void()> handler) {
    handlers[eventType].push_back(handler);
}

void EventManager::Publish(EventType eventType) {
    // 해당 이벤트 타입에 등록된 핸들러들 호출
    if(handlers.find(eventType) != handlers.end()) {
        for(auto& handler : handlers[eventType]) {
            handler();
        }
    }
}
