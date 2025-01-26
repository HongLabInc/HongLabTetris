#include "Scene.h"
#include "SceneManager.h" 

Scene::Scene(ConsoleRenderer& renderer, InputManager* inputManager, SceneManager* sceneManager)
    : mRenderer(renderer), mInputManager(inputManager), mSceneManager(sceneManager) {

}