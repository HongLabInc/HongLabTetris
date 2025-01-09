#include "GameMode.h"

#include <iostream>
#include <Windows.h>

GameMode::GameMode()
	: width(10),height(20), blockX(width/2), blockY(0)
{
	board = new char[width * height];
	frame = new char[width * height];
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

void GameMode::initailizeFrame()
{

}

void GameMode::Update(InputManager* im )
{
	if(cur_Block == nullptr) {
		cur_Block = new Block(4 , 0);
		cur_Block->Initalize();
	}
	// Move the block down
	else cur_Block->Update(im);


	if(cur_Block->GetY() >= height - 2) {
		cur_Block = nullptr;
	}
	

	blockY++;
	if(blockY >= height)
	{
		cur_Block = nullptr;
		blockY = 0; // Reset block position to the top
	}
}

void GameMode::Draw()
{
	drawBoard();
	drawBlock();
}

void GameMode::clearFrame()
{
	for(int i = 0; i < width * height; ++i)
		frame[i] = 0;
}




void GameMode::MoveBlockLeft()
{
	int idx = blockX + blockY * width;
	board[idx] = ' ';

	if(blockX <= 1) 
		blockX = 1;
	else blockX--;
}
void GameMode::MoveBlockRight()
{
	int idx = blockX + blockY * width;
	board[idx] = ' ';

	if(blockX  >= width - 2)
		blockX = width - 2;
	else blockX++;
}
void GameMode::MoveBlockDown()
{
	int idx = blockX + blockY * width;
	board[idx] = ' ';

	blockY = height - 3;
}

void GameMode::Instantiate(Block * block, int x)
{
	block->Initalize();
}
 


void GameMode::drawBoard()
{
	for(int i = 0; i < width * height; ++i)
	{
		frame[i] = board[i];
	}
	
}
void GameMode::drawBlock()
{
	if(cur_Block != nullptr) {
		int startX = cur_Block->GetX();
		int startY = cur_Block->GetY();
		const mat4x4& shape = cur_Block->GetShape();

		for(int i = 0; i < 4; ++i) {
			for(int j = 0; j < 4; ++j) {
				if(shape[i][j] != 0) {
					int boardX = startX + j;
					int boardY = startY + i;

					if(boardX >= 0 && boardX < width && boardY >= 0 && boardY < height) {
						int idx = boardX + boardY * width;
						frame[idx] = shape[i][j];
					}
				}
			}
		}
	}
	

	int oldY = blockY - 1;

	if(oldY >= 0)
	{
		// note: Prevent invalid array access by ensuring oldY is non-negative
		int idx = blockX + oldY * width;
		frame[idx] = ' ';
	}

	int idx = blockX + blockY * width;
	frame[idx] = '#';


	for(int i = 0; i < width * height; ++i) {
		consoleBuffer[i].Char.AsciiChar = frame[i];
	}

	SMALL_RECT writeArea = {0,0,(SHORT)width - 1,(SHORT)height - 1};
	WriteConsoleOutputA(consoleHandle,consoleBuffer,{(SHORT)width,(SHORT)height},{},&writeArea);
}

