#include "InputManager.h"


void InputManager::Update()
{
	m_keyPressed[VK_LEFT]  = GetAsyncKeyState(VK_LEFT);
	m_keyPressed[VK_RIGHT] = GetAsyncKeyState(VK_RIGHT);
	m_keyPressed[VK_SPACE] = GetAsyncKeyState(VK_SPACE);
}

bool InputManager::IsKeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000) != 0;
}

bool InputManager::IsKeyDown(int key)
{
	return false;
}
