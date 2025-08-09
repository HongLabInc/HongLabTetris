#include "GameOverScene.h"

GameOverScene::GameOverScene(
	ConsoleRenderer & renderer, 
	InputManager * im, 
	UIManager* um, 
	SceneManager * sm) 
	:Scene(renderer, im, um, sm)
{

	posY = 10;
	renderer.Clear();
	mFrame = renderer.AddFrame(0, 0, 40, 30);
}

void GameOverScene::Update(float deltaTime)
{
	mFrame->Clear();
	mFrame->SetText(5, posY, L"Game Over", static_cast<WORD>(ConsoleColor::BrightRed));
	posY--;
	Sleep(100);
	mSceneManager->RequestSceneChange(SceneType::MainMenu);
}

void GameOverScene::Draw()
{
	mRenderer.Render();
}
