#pragma once
#include "ConsoleFrame.h"
#include "Block.h"
#include "InputManager.h"
#include "ConsoleRenderer.h"

class TetrisBoard
{
public:
    TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height);
    ~TetrisBoard();

    void Init();
    void Update(InputManager* im);
    void Draw();

private:
    void DrawBoard();
    void DrawBlock();

    int mWidth;
    int mHeight;
    ConsoleFrame* mFrame;
    Block* mCurrentBlock;
};
