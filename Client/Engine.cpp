#include "Engine.h"

#include <iostream>

Engine::Engine(ConsoleRenderer& renderer)
	: consoleRenderer(renderer)
	, quit(false)
{
	inputManager = std::make_unique<InputManager>();
	uiManager = std::make_unique<UIManager>();
	sceneManager = std::make_unique<SceneManager>(renderer, inputManager.get(), uiManager.get());
	eventManager = std::make_unique<EventManager>();

	client = std::make_shared<Client>();
}
 
Engine::~Engine()
{
	if (client != nullptr)
		client->Stop();

}



void Engine::Initailize()
{
	InitializeModelPointers();


	std::string host = "127.0.0.1"; // 서버 호스트
	unsigned short port = 12345;     // 서버 포트

	if (!client->InitializeAsClient(host, port)) {
		std::cerr << "클라이언트 초기화 실패. 게임을 종료합니다.\n";
		QuitGame();
		return;
	}

	client->SetMessageReceivedCallback(
		[this](const std::string& message) { HandleNetworkMessage(message); }
	);

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


	// 게임 종료 시 클라이언트 정리
	if (client) {
		client->Stop();
	}
}

void Engine::ProcessInput()
{
	inputManager->Update();
}

void Engine::Update(float deltaTime)
{
	sceneManager->Update(deltaTime);

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




// Handle Network Message
void Engine::HandleNetworkMessage(const std::string& message)
{
	// 수신한 네트워크 메시지 처리
	std::cout << "네트워크 메시지 수신: " << message << std::endl;

	// 메시지에 따라 게임 상태 업데이트
	if (message == "PlayerJump")
	{

	}
	// 기타 메시지 처리...
}

void Engine::SendNetworkMessage(const std::string& message)
{
	if (client) {
		client->Send(message);
	}
}
