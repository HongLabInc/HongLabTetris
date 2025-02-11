#include "Scene.h"

Scene::Scene(
    ConsoleRenderer& renderer, 
    InputManager* inputManager,
    UIManager* uiManager,
    SceneManager* sceneManager)
    : mRenderer(renderer), 
    mInputManager(inputManager),
    mUIManager(uiManager),
    mSceneManager(sceneManager) {

}