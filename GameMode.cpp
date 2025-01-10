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
	board = new char[width * height];
	frame = new char[width * height];
    
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

void GameMode::Instantiate(Block * block, int x)
{
	block->Initalize();
}
 


void GameMode::drawBoard()
{
	for(int y = 0; y < height; ++y)
	{
		frame[i] = board[i];
	}
	
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
		//int idx = blockX + oldY * width;
		//board[idx] = ' ';
		board.SetCell(blockX, oldY, Cell::emptyCell);
	}

	//int idx = blockX + blockY * width;
	//board[idx] = '#';
	board.SetCell(blockX, blockY, Cell::blockCell);
}
