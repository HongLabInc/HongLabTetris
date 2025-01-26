#include "SceneManager.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

SceneManager::SceneManager(ConsoleRenderer& renderer, InputManager* inputManager)
    : mRenderer(renderer)
    , mInputManager(inputManager)
    , mCurrentScene(nullptr)
{
    // 시작 시 MainMenu로 전환 (원하면 다른 SceneType도 가능)
    RequestSceneChange(SceneType::MainMenu);
}

void SceneManager::RequestSceneChange(SceneType type)
{
    // 기존 Scene 제거
    mCurrentScene.reset();

    // 새로운 Scene 생성
    switch (type) {
    case SceneType::MainMenu:
    mCurrentScene = std::make_unique<MainMenuScene>(mRenderer, mInputManager, this);
    break;
    case SceneType::Playing:
	mCurrentScene = std::make_unique<PlayingScene>(mRenderer, mInputManager, this, GameModeType::Single);   
    break;
    case SceneType::GameOver:
	mCurrentScene = std::make_unique<GameOverScene>(mRenderer, mInputManager, this);
    break;
    default:
        //throw std::runtime_error("Unknown SceneType requested");
    break;
    }
}

void SceneManager::Update()
{
    // 현재 Scene 업데이트
    if (mCurrentScene) {
        mCurrentScene->Update();
    }
}

void SceneManager::Draw()
{
    // 현재 Scene 렌더링
    if (mCurrentScene) {
        mCurrentScene->Draw();
    }
}
