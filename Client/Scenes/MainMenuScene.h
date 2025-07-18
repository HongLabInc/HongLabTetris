#pragma once
#include "Scene.h"
#include "../GUI/Button.h"
#include "../GameModeType.h"

class MainMenuScene: public Scene {

	constexpr static int MENU_WIDTH = 40;
	constexpr static int MENU_HEIGHT = 30;

    std::unique_ptr<Button> mMultiMode;
    std::unique_ptr<Button> mSingleMode;

    GameModeType mSelectedMode;  // 현재 선택된 게임 모드

    void InitializeButtons();
    void UpdateButtonColors();   // 버튼 색상 업데이트

public:
    MainMenuScene(
        ConsoleRenderer& renderer,
        InputManager* inputManager,
        UIManager* uiManager,
        SceneManager* sceneManager);

    void Update(float deltaTime) override;

    void Draw() override;

    GameModeType GetSelectedMode() const { return mSelectedMode; }
};