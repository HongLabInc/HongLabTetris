#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "Board.h"

// 기본 게임모드
class GameMode
{
public:
    GameMode();
    ~GameMode();

    void initializeBoard();
    
    void Update();

    void Draw();

    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();

protected:
    void drawBoard();
    void drawBlock(); // Draw block and clear previous position

    const int width;
    const int height;
    Board board;

    int blockX;
    int blockY;

    HANDLE consoleHandle = 0;
    CHAR_INFO* consoleBuffer = nullptr;
};

