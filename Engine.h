#pragma once
#include "PlayerController.h"

class GameMode;
class Engine
{
public:
	explicit Engine();
	~Engine();

	void LoadGameMode(GameMode* newGameMode);

	void Run();

private:

	void ProcessInput();
	void Update(float deltaTime);
	void Draw();

	void QuitGame();

private:
	GameMode* gameMode = nullptr;
	PlayerController* playerController;  // add controller

	bool quit;

};

