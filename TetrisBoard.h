#pragma once
#include "ConsoleFrame.h"
#include "Block.h"
#include "InputManager.h"
#include "ColorManager.h"
#include "ConsoleRenderer.h"

class TetrisBoard
{
public:
    TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height);
    ~TetrisBoard();

    void Init();
    void InitBoard(int x,int y,int width,int height);
    void Update(InputManager* im);
    void UpdateGhostBlock();
    void Draw();

    bool CheckCollision(Block* block);



private:
    void DrawBoard();
    void DrawBlock(Block* block);
    void LockBlock();

    void Instantiate();

    void CheckLines();
    void ClearLine(int row); // 꽉채워진 라인 지우기
    void MoveLines();


    int mWidth;
    int mHeight;
    bool mIsButtonHeld = false;
    int mFirstInputFramesLeft = 0;
    int mContinuousInputFramesLeft = 0;
    int mFramesUntilUpdate = 0;
    static constexpr int mUpdateInterval = 100;
    static constexpr int mFirstInputDelayFrames = 8;
    static constexpr int mContinuousInputDelayFrames = 24;
    ConsoleRenderer& mRenderer;
    ConsoleFrame* mFrame;
    Block* mCurrentBlock;
    Block* mGhostBlock;

    std::vector<std::vector<bool>> isFilled;
    std::vector<int> rowCounts;
    std::vector<int> colCounts;

    int maxHorizontalPixels;
    int maxVerticalPixels;

    int curHeight;

    ColorManager* colorManager;
};
