#include "GameMode.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode()
	: width(10),height(20)
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

void GameMode::clearScreen()
{
}

void GameMode::initializeBoard()
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int i = x + y * width;
			board[i] = ' ';
		}
	}
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

void GameMode::drawBlock(int oldX,int oldY,int newX,int newY)
{
	if(oldY >= 0)
	{
		// note: Prevent invalid array access by ensuring oldY is non-negative
		int idx = oldX + oldY * width;
		board[idx] = ' ';
	}

	int idx = newX + newY * width;
	board[idx] = '#';
}

const int GameMode::GetWidth() const
{
	return this->width;
}

const int GameMode::GetHeight() const
{
	return this->height;
}
