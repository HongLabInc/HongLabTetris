// PlayingScene.h
#pragma once
#include "Scene.h"
#include "../GameModeType.h"
#include "../TetrisBoard.h"

class PlayingScene: public Scene {
public:
    PlayingScene(
        ConsoleRenderer& renderer, 
        InputManager* im, 
        UIManager* um,
        SceneManager* sm, 
        GameModeType mode);

    void Update(float deltaTime) override;

    void Draw() override;

private:
    void SetupSingleMode();
    void SetupMultiplayerMode();

    std::vector<std::shared_ptr<TetrisBoard>> mBoards;
    GameModeType mCurrentMode;
};