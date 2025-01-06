#include "GameMode.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode()
	: width(10),height(20)
{
	board = new char[width * height];
}

GameMode::~GameMode()
{
	if(board != nullptr)
	{
		delete board;
		board = nullptr;
	}
}

void GameMode::clearScreen()
{
	system("cls");
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

void GameMode::drawBoard()
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int idx = x + y * width;
			std::cout << board[idx];
		}
		std::cout << std::endl;
	}
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
