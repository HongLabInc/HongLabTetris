#pragma once
#include <windows.h>
#include <queue>
#include <chrono>

class InputManager {
	std::queue<int> mInputQueue; // 키 입력 큐

public:
	bool IsKeyPressed(int key);   // 특정 키가 눌렸는지
	bool IsKeyDown(int key);      // 이번 프레임에 막 눌린 키인지

	void EnqueueInput(int key);
	int DequeueInput();

	void AddPressedKeysToQueue();

private:
	bool m_keyPressed[256] = { false, };
	bool prevKeyState[256] = { false, };

	std::chrono::steady_clock::time_point lastKeyEventTime[256];

	const std::chrono::milliseconds continuousInputInterval = std::chrono::milliseconds(166);
};
