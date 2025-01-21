#include "GameMode.h"
#include "ConsoleColor.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode(ConsoleRenderer& renderer, GameModeType mode)
	: mRenderer(renderer)
	, mCurrentMode(mode)
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

void GameMode::Update(InputManager* im)
{
    for(auto& board : mBoards)
    {
		board->Update(im);
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
    mBoards.emplace_back(std::make_shared<TetrisBoard>(mRenderer, 3, 3, 12, 24));
}

void GameMode::SetupMultiplayerMode()
{
}
