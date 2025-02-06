#include "GameOverScene.h"

GameOverScene::GameOverScene(ConsoleRenderer & renderer, InputManager * im, SceneManager * sm) 
	:Scene(renderer, im, sm)
{

	posY = 10;
	renderer.Clear();
	mFrame = renderer.AddFrame(0, 0, 40, 30);
}

void GameOverScene::Update()
{
	mFrame->Clear();
	mFrame->SetText(5, posY, L"Game Over", static_cast<WORD>(ConsoleColor::BrightRed));
	posY--;
	Sleep(100);
}

void GameOverScene::Draw()
{
	mRenderer.Render();
}
