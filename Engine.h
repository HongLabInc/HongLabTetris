#pragma once
#include "ColorManager.h"
#include "InputManager.h"
#include "ConsoleRenderer.h"
#include "ModelPointer.h"

class GameMode;
class Engine
{
public:
	explicit Engine(ConsoleRenderer& renderer);
	~Engine();

	void LoadGameMode(GameMode* newGameMode);

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

	InputManager* inputManager;
	ColorManager* colorManager;

};

