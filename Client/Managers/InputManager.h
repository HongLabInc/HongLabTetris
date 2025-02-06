#pragma once
#include <windows.h>
#include <queue>
#include <chrono>

enum MouseButton {
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_BUTTON_COUNT
};

class InputManager {
	std::queue<int> mInputQueue; // 키 입력 큐 
	std::queue<int> mMouseInputQueue;


public:


	#pragma region KeyBoard Method


	bool IsKeyPressed(int key);   // 특정 키가 눌렸는지
	bool IsKeyDown(int key);      // 이번 프레임에 막 눌린 키인지

	void EnqueueInput(int key);
	int DequeueInput();

	void AddPressedKeysToQueue();
	#pragma endregion

	#pragma region Mouse Method
	bool IsMousePressed(MouseButton button); // 특정 마우스 버튼이 현재 눌린 상태인지 반환
	bool IsMouseDown(MouseButton button);      // 이번 프레임에 새로 눌린 마우스 버튼인지 반환

	void EnqueueMouseInput(int eventCode);
	int DequeueMouseInput();

	void SetMousePosition(int x, int y);
	POINT GetMousePosition() const;

	void AddMouseEventsToQueue();
	#pragma endregion 


private:
	bool m_keyPressed[256] = { false, };
	bool prevKeyState[256] = { false, };

	std::chrono::steady_clock::time_point lastKeyEventTime[256];

	const std::chrono::milliseconds continuousInputInterval = std::chrono::milliseconds(166);

	// 마우스 상태 관리
	bool m_mousePressed[MOUSE_BUTTON_COUNT] = {false, };
	bool prevMouseState[MOUSE_BUTTON_COUNT] = {false, };

	std::chrono::steady_clock::time_point lastMouseEventTime[MOUSE_BUTTON_COUNT];


	POINT m_mousePosition = {0, 0};
};
