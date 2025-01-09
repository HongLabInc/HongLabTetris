#include "GameMode.h"
#include "ConsoleColor.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode()
	: width(10)
	, height(20)
	, board(Board(width, height))
	, blockX(width / 2)
	, blockY(0)
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(consoleHandle);

	CONSOLE_CURSOR_INFO cursorInfo = {1, false};
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);

	consoleBuffer = new CHAR_INFO[width * height]();
}

GameMode::~GameMode()
{
	delete consoleBuffer;
	consoleBuffer = nullptr;
}

void GameMode::initializeBoard()
{
	// 초기화 코드 (프로그램 실행 직후 Engine 클래스에서 Run 메소드 호출시 initializeBoard 호출)
}

void GameMode::Update()
{
	// Move the block down
	blockY++;
	if(blockY >= height)
	{
		blockY = 0; // Reset block position to the top
	}
}

void GameMode::Draw()
{
	drawBoard();
	drawBlock();
}


void GameMode::MoveBlockLeft()
{
	//int idx = blockX + blockY * width;

	//board[idx] = ' ';
	board.SetCell(blockX, blockY, Cell::emptyCell);

	if(blockX <= 1) 
		blockX = 1;
	else blockX--;
}

void GameMode::MoveBlockRight()
{
	//int idx = blockX + blockY * width;
	//board[idx] = ' ';
	board.SetCell(blockX, blockY, Cell::emptyCell);

	if(blockX  >= width - 2)
		blockX = width - 2;
	else blockX++;
}

void GameMode::MoveBlockDown()
{
	//int idx = blockX + blockY * width;
	//board[idx] = ' ';
	board.SetCell(blockX, blockY, Cell::emptyCell);

	blockY = height - 3;
}
 


void GameMode::drawBoard()
{
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			const int index = x + y * width;
			const Cell* cell = board.GetCellPtr(x, y);

			consoleBuffer[index] = cell->ToCharInfo();
		}
	}
	SMALL_RECT writeArea = {0, 0, (SHORT)width - 1, (SHORT)height - 1};
	WriteConsoleOutputW(consoleHandle, consoleBuffer, {(SHORT)width, (SHORT)height}, {}, &writeArea);
}

void GameMode::drawBlock()
{
	int oldY = blockY - 1;

	if(oldY >= 0)
	{
		// note: Prevent invalid array access by ensuring oldY is non-negative
		//int idx = blockX + oldY * width;
		//board[idx] = ' ';
		board.SetCell(blockX, oldY, Cell::emptyCell);
	}

	//int idx = blockX + blockY * width;
	//board[idx] = '#';
	board.SetCell(blockX, blockY, Cell::blockCell);
}
