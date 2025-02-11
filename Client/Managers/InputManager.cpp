#include "InputManager.h"
#include <iostream>

#pragma region KeyBoard Method

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

    int keysToCheck[] = {VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN, VK_RETURN};

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
#pragma endregion

#pragma region Mouse Method
bool InputManager::IsMousePressed(MouseButton button) {
    
    UINT vkButton = 0;
    switch (button) {
    case MOUSE_LEFT:
        vkButton = VK_LBUTTON;
        break;
    case MOUSE_RIGHT:
        vkButton = VK_RBUTTON;
        break;
    case MOUSE_MIDDLE:
        vkButton = VK_MBUTTON;
        break;

    default:
        return false;
    }

    return (GetAsyncKeyState(vkButton) & 0x8000) != 0;
}

bool InputManager::IsMouseDown(MouseButton button) {
    if (button < 0 || button >= MOUSE_BUTTON_COUNT)
        return false;
    return (m_mousePressed[button] && !prevMouseState[button]);
}

void InputManager::EnqueueMouseInput(int eventCode) {
    mMouseInputQueue.push(eventCode);
}

int InputManager::DequeueMouseInput() {
    if (mMouseInputQueue.empty()) {
        return -1; // 또는 적절한 무효 값
    }
    int eventCode = mMouseInputQueue.front();
    mMouseInputQueue.pop();
    return eventCode;
}

void InputManager::SetMousePosition(int x, int y) {
    m_mousePosition.x = x;
    m_mousePosition.y = y;
}

POINT InputManager::GetMousePosition() const {
    POINT p = {0, 0};
    if (GetCursorPos(&p)) {
        HWND consoleWindow = GetConsoleWindow();
        if (consoleWindow) {
            if (ScreenToClient(consoleWindow, &p)) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_FONT_INFOEX cfi = {0};
                cfi.cbSize = sizeof(cfi);

                if (GetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
                    // 픽셀 좌표를 문자 셀 좌표로 변환
                    if (cfi.dwFontSize.X > 0 && cfi.dwFontSize.Y > 0) {
                        p.x = p.x / cfi.dwFontSize.X;
                        p.y = p.y / cfi.dwFontSize.Y;
                    }

                    // 디버깅용 좌표 출력
                    #ifdef _DEBUG
                    std::wcout << L"Mouse Cell Position: " << p.x << L", " << p.y << std::endl;
                    #endif
                }
            }
        }
    }
    return p;
}

void InputManager::AddMouseEventsToQueue()
{
    using clock = std::chrono::steady_clock;
    auto now = clock::now();
    int EventsToCheck[] = {MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT};

    for (int button : EventsToCheck) {
        if (IsMousePressed(static_cast<MouseButton>(button))) {
            if (!prevMouseState[button]) {
                EnqueueMouseInput(button);
                prevMouseState[button] = true;
                lastMouseEventTime[button] = now;
            } else {
                if (now - lastMouseEventTime[button] >= continuousInputInterval) {
                    lastMouseEventTime[button] = now;  // 시간 갱신
                }
            }
        } else {
            prevMouseState[button] = false;
        }
    }

}

#pragma endregion

