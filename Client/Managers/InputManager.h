#pragma once
#include <windows.h>
#include <queue>
#include <chrono>
#include <functional>
#include <vector>
#include <unordered_map>

namespace ConsoleConstants {
    const int CONSOLE_WIDTH = 80;
    const int CONSOLE_HEIGHT = 25;
    const int CELL_WIDTH = 2;
    }

enum MouseButton {
    MOUSE_LEFT = 0,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_BUTTON_COUNT
};

struct MouseEvent {
    MouseButton button;
    POINT position;
    bool isPressed;
};

class InputManager {
    using MouseEventCallback = std::function<void(const MouseEvent&)>;

private:
    std::queue<int> mInputQueue;      // 키 입력 큐 
    std::queue<int> mMouseInputQueue; // 마우스 입력 큐

    // 키보드 상태
    bool m_keyPressed[256] = {false, };
    bool prevKeyState[256] = {false, };
    std::chrono::steady_clock::time_point lastKeyEventTime[256];
    const std::chrono::milliseconds continuousInputInterval = std::chrono::milliseconds(166);

    // 마우스 상태
    bool m_mousePressed[MOUSE_BUTTON_COUNT] = {false, };
    bool prevMouseState[MOUSE_BUTTON_COUNT] = {false, };
    std::chrono::steady_clock::time_point lastMouseEventTime[MOUSE_BUTTON_COUNT];
    POINT m_mousePosition = {0, 0};
    POINT m_prevMousePosition = {0, 0};

    // 이벤트 리스너
    size_t mNextListenerId = 0;
    std::unordered_map<size_t, MouseEventCallback> mMouseListeners;
public:
    #pragma region KeyBoard Method
    bool IsKeyPressed(int key);   // 특정 키가 눌렸는지
    bool IsKeyDown(int key);      // 이번 프레임에 막 눌린 키인지
    void EnqueueInput(int key);
    int DequeueInput();
    void AddPressedKeysToQueue();
    #pragma endregion

    #pragma region Mouse Method
    // 기존 메서드들
    bool IsMousePressed(MouseButton button);

    bool IsMouseDown(MouseButton button);

    void EnqueueMouseInput(int eventCode);

    int DequeueMouseInput();

    POINT GetMousePosition() const;

    void SetMousePosition(int x, int y);

    // 새로 추가된 이벤트 관련 메서드들
    size_t AddMouseListener(MouseEventCallback callback);

    void RemoveMouseListener(size_t index);

    void ProcessMouseEvent(MouseButton button, bool isPressed);

    void UpdateMousePosition();

    void AddMouseEventsToQueue();
    #pragma endregion

    void Update();

};