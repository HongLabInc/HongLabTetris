#include "InputManager.h"

void InputManager::EnqueueInput(int key) {
	mInputQueue.push(key);
}

int InputManager::DequeueInput() {
	
	if(mInputQueue.empty())
		return -1;

	int key = mInputQueue.front();
	mInputQueue.pop();
	return key;

}

void InputManager::AddPressedKeysToQueue() {

    using clock = std::chrono::steady_clock;
    auto now = clock::now();

    int keysToCheck[] = {VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN};

    for(int key : keysToCheck) {
        if(IsKeyPressed(key)) {
            if(!prevKeyState[key]) {
                EnqueueInput(key);
                prevKeyState[key] = true;
                lastKeyEventTime[key] = now;  
            } else {
                if(now - lastKeyEventTime[key] >= continuousInputInterval) {
                    EnqueueInput(key);
                    lastKeyEventTime[key] = now;  // 시간 갱신
                }
            }
        } else {
            prevKeyState[key] = false;
        }
    }
}

bool InputManager::IsKeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000) != 0;
}

bool InputManager::IsKeyDown(int key)
{
	return false;
}
