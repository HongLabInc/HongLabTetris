#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "Block.h"
#include "InputManager.h"
#include "Board.h"


// 기본 게임모드
class GameMode
{
public:
    GameMode();
    ~GameMode();

    void initializeBoard();
    void initailizeFrame();
    
    void Update(InputManager* im);

    void Draw();
    void clearFrame();

    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();

    void Instantiate(Block* block, int x);

protected:
    void drawBoard();
    void drawBlock(); // Draw block and clear previous position

    const int width;
    const int height;

    char* board = nullptr;
    char* frame = nullptr;

    Board board;


    int blockX;
    int blockY;

    HANDLE consoleHandle = 0;
    CHAR_INFO* consoleBuffer = nullptr;

    Block* cur_Block = nullptr;
};

