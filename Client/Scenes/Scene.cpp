#include "Scene.h"

Scene::Scene(
    ConsoleRenderer& renderer, 
    InputManager* inputManager,
    UIManager* uiManager,
    SceneManager* sceneManager)
    : mRenderer(renderer), 
    mInputManager(inputManager),
    mUIManager(uiManager),
    mSceneManager(sceneManager) 
{
    mPendingSceneChange = SceneType::None;
}

SceneType Scene::GetPendingSceneChange() const {
    return mPendingSceneChange;
}

void Scene::ClearPendingSceneChange() {
    mPendingSceneChange = SceneType::None;
}