#pragma once
#include "Scene.h"

class MainMenuScene: public Scene {

	constexpr static int MENU_WIDTH = 40;
	constexpr static int MENU_HEIGHT = 30;

public:
    MainMenuScene(ConsoleRenderer& renderer, InputManager* inputManager, SceneManager* sceneManager);

    void Update() override;

    void Draw() override;
};