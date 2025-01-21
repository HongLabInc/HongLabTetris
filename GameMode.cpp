#include "GameMode.h"
#include "ConsoleColor.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode(ConsoleRenderer& renderer,GameModeType mode, InputManager* im,EventManager* em)
	: mRenderer(renderer)
	,mCurrentMode(mode)
	,mInputManager(im)
	,mEventManager(em)
{
	switch(mCurrentMode)
    {
        case GameModeType::Single:
            SetupSingleMode();
            break;
        case GameModeType::Multiplayer:
            SetupMultiplayerMode();
            break;
    }
}

void GameMode::Update()
{
    for(auto& board : mBoards)
    {
		board->Update();
    }
}

void GameMode::Draw()
{
    for(auto& board : mBoards)
    {
        board->Draw();
    }

    mRenderer.Render();
}

void GameMode::SetupSingleMode()
{
    mBoards.emplace_back(std::make_shared<TetrisBoard>(mRenderer, 3, 3, 12, 24, mInputManager, mEventManager));
}

void GameMode::SetupMultiplayerMode()
{
}
