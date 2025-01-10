#include "Engine.h"
#include "GameMode.h"

#include <iostream>
#include <Windows.h>

Engine::Engine()
	: gameMode(nullptr), quit(false)
{
	LoadGameMode(new GameMode());
	inputManager = new InputManager();
}
 
Engine::~Engine()
{
	if(gameMode != nullptr)
	{
		delete gameMode;
		gameMode = nullptr;
	}
}

void Engine::LoadGameMode(GameMode * newGameMode)
{
	gameMode = newGameMode;
}

void Engine::Run()
{
	gameMode->initializeBoard();

	// 고해상도 카운터
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	int64_t currentTime = 0;
	int64_t previousTime = counter.QuadPart;

	float targetFrameRate = 5.0f; // 목표 fps
	float targetOneFrameTime = 1.0f / targetFrameRate;

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
			ProcessInput();
			Update(deltaTime);
			Draw();

			previousTime = currentTime;
		}
	}
}

void Engine::ProcessInput()
{
	inputManager->Update();
}

void Engine::Update(float deltaTime)
{
	inputManager->Update();
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
