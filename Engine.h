#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h> // 필요한 경우

#include "ColorManager.h"
#include "InputManager.h"
#include "ConsoleRenderer.h"
#include "ModelPointer.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Client.h"

class GameMode;
class Engine
{
public:
	explicit Engine(ConsoleRenderer& renderer);
	~Engine();

	void Initailize();
	void Run();

private:

	void ProcessInput();
	void Update(float deltaTime);
	void Draw();

	void QuitGame();

	// Network Massage 처리
	void HandleNetworkMessage(const std::string& message);
	void SendNetworkMessage(const std::string& message);

private:
	ConsoleRenderer& consoleRenderer;
	GameMode* gameMode = nullptr;

	bool quit;

	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<EventManager> eventManager;


	// 네트워크 관리
	std::shared_ptr<Client> client;
};
