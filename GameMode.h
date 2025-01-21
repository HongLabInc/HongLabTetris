#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include "TetrisBoard.h"
#include "ConsoleRenderer.h"
#include "InputManager.h"
#include "EventManager.h"

class GameMode
{
public:
    enum class GameModeType
    {
        Single,
        Multiplayer
    };
    GameMode(ConsoleRenderer& renderer, GameModeType mode, InputManager* im, EventManager* em);
    ~GameMode() = default;

    void Update();
    void Draw();
    void OnGameEnd();

private:
	void SetupSingleMode();
	void SetupMultiplayerMode();

    ConsoleRenderer& mRenderer;
    std::vector<std::shared_ptr<TetrisBoard>> mBoards;
    GameModeType mCurrentMode;

	InputManager* mInputManager;
	EventManager* mEventManager;
};
