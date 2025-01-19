#include "Engine.h"
#include "GameMode.h"

#include <iostream>
#include <Windows.h>

Engine::Engine(ConsoleRenderer& renderer)
	: consoleRenderer(renderer)
	, gameMode(nullptr)
	, quit(false)
{
	LoadGameMode(new GameMode(renderer, GameMode::GameModeType::Single));

	inputManager = new InputManager();

	colorManager = new ColorManager();
}
 
Engine::~Engine()
{
	if(gameMode != nullptr)
	{
		delete gameMode;
		gameMode = nullptr;
	}

	delete colorManager;
	delete inputManager;
}

void Engine::LoadGameMode(GameMode * newGameMode)
{
	gameMode = newGameMode;
}

void Engine::Initailize()
{
	InitializeModelPointers();

}

void Engine::Run()
{
	// 고해상도 카운터
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	int64_t currentTime = 0;
	int64_t previousTime = counter.QuadPart;

	float targetFrameRate = consoleRenderer.GetFrameRate();
	float targetOneFrameTime = consoleRenderer.GetFrameTime();

	// Main Game Loop
	while(true)
	{
		if(quit)
		{
			break;
		}

		// 프레임 시간 계산
		QueryPerformanceCounter(&counter);
		currentTime = counter.QuadPart;

		float deltaTime = static_cast<float>(currentTime - previousTime) /
			static_cast<float>(frequency.QuadPart);

		if(deltaTime >= targetOneFrameTime)
		{
			static int count = 0;
			++count;
			ProcessInput();
			Update(deltaTime);
			Draw();

			previousTime = currentTime;
		}

	}
}

void Engine::ProcessInput()
{
	inputManager->AddPressedKeysToQueue();
}

void Engine::Update(float deltaTime)
{
	gameMode->Update(inputManager);

	//Sleep(500);
}

void Engine::Draw()
{
	gameMode->Draw();
}

void Engine::QuitGame()
{
	quit = true;
}
