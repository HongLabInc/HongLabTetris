#include "Scene.h"

Scene::Scene(ConsoleRenderer& renderer, InputManager* inputManager, SceneManager* sceneManager)
    : mRenderer(renderer), mInputManager(inputManager), mSceneManager(sceneManager) {

}