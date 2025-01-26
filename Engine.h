#pragma once
#include "ColorManager.h"
#include "InputManager.h"
#include "ConsoleRenderer.h"
#include "ModelPointer.h"
#include "EventManager.h"
#include "SceneManager.h"

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

private:
	ConsoleRenderer& consoleRenderer;
	GameMode* gameMode = nullptr;

	bool quit;

	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<EventManager> eventManager;
};
