#include "InputManager.h"
#include <iostream>

#pragma region KeyBoard Method

bool InputManager::IsKeyPressed(int key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
bool InputManager::IsKeyDown(int key)
{
    return false;
}

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

    int keysToCheck[] = {VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN, VK_RETURN, VK_SPACE};

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

#pragma endregion


#pragma region Mouse Method

bool InputManager::IsMousePressed(MouseButton button) {
    return m_mousePressed[button];
}

bool InputManager::IsMouseDown(MouseButton button) {
    return m_mousePressed[button] && !prevMouseState[button];
}

void InputManager::EnqueueMouseInput(int eventCode) {
    mMouseInputQueue.push(eventCode);
}

int InputManager::DequeueMouseInput() {
    if (mMouseInputQueue.empty()) return -1;
    int event = mMouseInputQueue.front();
    mMouseInputQueue.pop();
    return event;
}

POINT InputManager::GetMousePosition() const {
    return m_mousePosition;
}

void InputManager::SetMousePosition(int x, int y) {
    m_mousePosition = {x, y};
}

// 새로 추가된 이벤트 관련 메서드들

size_t InputManager::AddMouseListener(MouseEventCallback callback) {
    size_t newId = mNextListenerId++;
    mMouseListeners[newId] = callback;
    return newId;
}

void InputManager::RemoveMouseListener(size_t id) {
    mMouseListeners.erase(id);
}

void InputManager::ProcessMouseEvent(MouseButton button, bool isPressed) {
    MouseEvent event{
        button,
        m_mousePosition,
        isPressed
    };

    auto listenersCopy = mMouseListeners;
    for (const auto& [id, listener] : listenersCopy) {
        listener(event);
    }
}

void InputManager::UpdateMousePosition() {
    m_prevMousePosition = m_mousePosition;  // 현재 위치를 이전 위치로 저장

    POINT screenPos;
    GetCursorPos(&screenPos);
    HWND terminalWindow = GetForegroundWindow();
    RECT windowRect, clientRect;
    GetWindowRect(terminalWindow, &windowRect);
    GetClientRect(terminalWindow, &clientRect);

    // 타이틀 바와 경계선의 높이 계산
    int borderHeight = windowRect.bottom - windowRect.top - clientRect.bottom;
    int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

    // 윈도우 좌표로 변환 (타이틀 바와 경계선 직접 고려)
    screenPos.x -= windowRect.left;
    screenPos.y -= (windowRect.top + titleBarHeight);

    int CONSOLE_WIDTH = 80;
    int CONSOLE_HEIGHT = 29;

    // 콘솔 좌표로 변환
    float ratioX = static_cast<float>(screenPos.x) / clientRect.right;
    float ratioY = static_cast<float>(screenPos.y) / clientRect.bottom;

    m_mousePosition.x = static_cast<int>(ratioX * CONSOLE_WIDTH);
    m_mousePosition.y = static_cast<int>(ratioY * CONSOLE_HEIGHT);

    if (m_prevMousePosition.x != m_mousePosition.x ||
        m_prevMousePosition.y != m_mousePosition.y) {
        ProcessMouseEvent(MOUSE_LEFT, false);  // 마우스 이동 이벤트
    }
}

void InputManager::AddMouseEventsToQueue() {
    auto currentTime = std::chrono::steady_clock::now();

    // 마우스 버튼 상태 업데이트
    bool newStates[MOUSE_BUTTON_COUNT] = {
        (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0,
        (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0,
        (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0
    };

    for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i) {
        m_mousePressed[i] = newStates[i];

        if (m_mousePressed[i] != prevMouseState[i]) {
            ProcessMouseEvent(static_cast<MouseButton>(i), m_mousePressed[i]);

            if (m_mousePressed[i]) {  // 눌렸을 때만 큐에 추가
                EnqueueMouseInput(i);
                lastMouseEventTime[i] = currentTime;
            }
        }
        prevMouseState[i] = m_mousePressed[i];
    }
}

void InputManager::Update()
{
    UpdateMousePosition();
    AddMouseEventsToQueue();
    AddPressedKeysToQueue();
}

#pragma endregion





