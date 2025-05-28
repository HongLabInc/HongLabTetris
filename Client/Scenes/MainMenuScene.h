#pragma once
#include "Scene.h"
#include "../GUI/Button.h"

class MainMenuScene: public Scene {

	constexpr static int MENU_WIDTH = 40;
	constexpr static int MENU_HEIGHT = 30;

    std::unique_ptr<Button> mMultiMode;
    std::unique_ptr<Button> mSingleMode;

    void InitializeButtons();

public:
    MainMenuScene(
        ConsoleRenderer& renderer,
        InputManager* inputManager,
        UIManager* uiManager,
        SceneManager* sceneManager);

    void Update(float deltaTime) override;

    void Draw() override;
};