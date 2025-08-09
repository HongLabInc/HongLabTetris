#include "GameScene.h"

PlayingScene::PlayingScene(
    ConsoleRenderer & renderer, 
    InputManager * im,
    UIManager* um,
	SceneManager* sm,
    GameModeType mode)
	: Scene(renderer, im, um, sm)
	, mGameManager(renderer, sm, im)
    , mCurrentMode(mode)
{

	mRenderer.Clear();
	mFrame = mRenderer.AddFrame(10, 5, 40, 30);

    // 게임 시작
    mGameManager.Start(mCurrentMode);
}

void PlayingScene::Update(float deltaTime) {
    mGameManager.Update(deltaTime);
}

void PlayingScene::Draw() {
    mGameManager.Draw();
    mRenderer.Render();
}
