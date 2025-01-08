#include "GameMode.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode()
	: width(10),height(20), blockX(width/2), blockY(0)
{
	board = new char[width * height];
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(consoleHandle);
	CONSOLE_CURSOR_INFO cursorInfo = {1, false};
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	consoleBuffer = new CHAR_INFO[width * height]();
	for(int i = 0; i < width * height; ++i)
	{
		consoleBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
}

GameMode::~GameMode()
{
	delete board;
	board = nullptr;
	delete consoleBuffer;
	consoleBuffer = nullptr;
}

void GameMode::initializeBoard()
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int i = x + y * width;

			// Set left and right
			if(x == 0 || x == width - 1)
			{
				board[i] = '#';
			}
			// Set bottom
			else if(y == height - 1)
			{
				board[i] = '#';
			} else
			{
				board[i] = ' ';
			}
		}
	}
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
	int idx = blockX + blockY * width;
	board[idx] = ' ';

	if(blockX <= 0) 
		blockX = 0;
	else blockX--;
}

void GameMode::MoveBlockRight()
{
	int idx = blockX + blockY * width;
	board[idx] = ' ';

	if(blockX >= width)
		blockX = width;
	else blockX++;
}

void GameMode::MoveBlockDown()
{
	int idx = blockX + blockY * width;
	board[idx] = ' ';

	blockY = height - 3;
}

void GameMode::clearScreen()
{
	system("cls");
}


void GameMode::drawBoard()
{
	for(int i = 0; i < width * height; ++i)
	{
		consoleBuffer[i].Char.AsciiChar = board[i];
	}
	SMALL_RECT writeArea = {0, 0, (SHORT)width - 1, (SHORT)height - 1};
	WriteConsoleOutputA(consoleHandle, consoleBuffer, {(SHORT)width, (SHORT)height}, {}, &writeArea);
}

void GameMode::drawBlock()
{
	int oldY = blockY - 1;

	if(oldY >= 0)
	{
		// note: Prevent invalid array access by ensuring oldY is non-negative
		int idx = blockX + oldY * width;
		board[idx] = ' ';
	}

	int idx = blockX + blockY * width;
	board[idx] = '#';
}

