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
    void InitBoard(int x,int y,int width,int height);
    void Update(InputManager* im);
    void Draw();

    bool CheckCollision();



private:
    void DrawBoard();
    void DrawBlock();
    void LockBlock();

    void Instantiate();

    void CheckLines();
    void ClearLine(int row); // 꽉채워진 라인 지우기
    void MoveLines();


    int mWidth;
    int mHeight;
    ConsoleFrame* mFrame;
    Block* mCurrentBlock;

    std::vector<std::vector<bool>> isFilled;
    std::vector<int> rowCounts;
    std::vector<int> colCounts;

    int maxHorizontalPixels;
    int maxVerticalPixels;

    int curHeight;
};
