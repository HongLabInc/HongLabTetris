#include "Engine.h"

#include <iostream>
#include <Windows.h>

Engine::Engine(ConsoleRenderer& renderer)
	: consoleRenderer(renderer)
	, gameMode(nullptr)
	, quit(false)
{
	inputManager = std::make_unique<InputManager>();
	sceneManager = std::make_unique<SceneManager>(renderer, inputManager.get());
	eventManager = std::make_unique<EventManager>();


}
 
Engine::~Engine()
{
	if(gameMode != nullptr)
	{
		delete gameMode;
		gameMode = nullptr;
	}

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

	constexpr int INPUT_FREQUENCY_MULTIPLIER = 5;
	float refreshTime = targetOneFrameTime / static_cast<float>(INPUT_FREQUENCY_MULTIPLIER);

	int frameUntilRender = 1;
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

		if(deltaTime >= refreshTime)
		{
			ProcessInput();
			--frameUntilRender;

			if (frameUntilRender <= 0)
			{

				Update(deltaTime);
				Draw();
				frameUntilRender = INPUT_FREQUENCY_MULTIPLIER;
			}

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
	sceneManager->Update();

	//Sleep(500);
}

void Engine::Draw()
{
	sceneManager->Draw();
}

void Engine::QuitGame()
{
	quit = true;
}
