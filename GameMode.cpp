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
		board.Update(im);
    }
}

void GameMode::Draw()
{
    for(auto& board : mBoards)
    {
        board.Draw();
    }

    mRenderer.Render();
}

void GameMode::SetupSingleMode()
{
	mBoards.push_back(TetrisBoard(mRenderer, 3, 3, 12, 24));

	//임시 출력 테스트
	ShowExampleConsoleFrame();
}

void GameMode::SetupMultiplayerMode()
{
}






void GameMode::ShowExampleConsoleFrame()
{
	ConsoleFrame* titleFrame = mRenderer.AddFrame(18, 5, 30, 10);

	titleFrame->DrawRectangle(0, 0, 30, 10, Cell(Cell::Type::Border, L'+',
		static_cast<WORD>(ConsoleColor::BrightYellow)));

	for(int i = 1; i < 9; ++i)
	{
		titleFrame->SetCell(14, i, Cell(Cell::Type::Block, L'|',
			static_cast<WORD>(ConsoleColor::BrightCyan)));
		titleFrame->SetCell(15, i, Cell(Cell::Type::Block, L'|',
			static_cast<WORD>(ConsoleColor::BrightCyan)));
	}

	titleFrame->FillRectangle(5, 4, 20, 1, Cell(Cell::Type::Block, L'-',
		static_cast<WORD>(ConsoleColor::BrightGreen)));

	std::wstring tetrisText = L"  TETRIS  ";
	for(size_t i = 0; i < tetrisText.size(); ++i)
	{
		titleFrame->SetCell(10 + i, 4, Cell(Cell::Type::Block, tetrisText[i],
			static_cast<WORD>(ConsoleColor::BrightRed)));
	}
}
