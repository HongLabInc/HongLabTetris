#pragma once
#include <windows.h>

class InputManager {

public:
	void Update();                // 매 프레임마다 입력 상태 갱신
	bool IsKeyPressed(int key);   // 특정 키가 눌렸는지
	bool IsKeyDown(int key);      // 이번 프레임에 막 눌린 키인지

private:
	bool m_keyPressed[256] = {
		false,
	};

};
