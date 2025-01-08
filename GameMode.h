#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

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
    // Draw block and clear previous position
    void drawBlock();


protected:
    const int width;
    const int height;
    char* board = nullptr;

    int blockX;
    int blockY;

    HANDLE consoleHandle = 0;
    CHAR_INFO* consoleBuffer = nullptr;

};

