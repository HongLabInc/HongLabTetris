#pragma once

// 기본 게임모드
class GameMode
{
public:
    GameMode();
    ~GameMode();

    void initializeBoard();
    
    void Update();

    void Draw();

protected:
    void clearScreen();
    void drawBoard();
    // Draw block and clear previous position
    void drawBlock();


protected:
    const int width;
    const int height;
    char* board = nullptr;

    int blockX;
    int blockY;
};

