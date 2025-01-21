#pragma once
#include "ConsoleFrame.h"
#include "Block.h"
#include "InputManager.h"
#include "ColorManager.h"
#include "EventManager.h"
#include "ConsoleRenderer.h"

class TetrisBoard
{
public:
    TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height,InputManager* im,EventManager* em);
    ~TetrisBoard();

    void InitBoard(int x,int y,int width,int height);
    void Update();
    void HandleInput();
    void MoveBlockDown();
    void UpdateGhostBlock();
    void Draw();

    bool IsFull();
    int GetTopRow();

    bool CheckCollision(Block* block);

private:
    void ClearBlockImage();
    void DrawBlock(Block * block, const Cell & blockCell);
    void LockBlock();

    void Instantiate();

    void CheckLines();
    void MoveLine(int targetLine, int sourceLine);
    void ClearLine(int row); // 꽉채워진 라인 지우기
    void MoveLines();

    static constexpr int mUpdateInterval = 100;
    static constexpr int mFirstInputDelayFrames = 8;
    static constexpr int mContinuousInputDelayFrames = 24;

    int mWidth;
    int mHeight;
    bool mIsButtonHeld = false;
    int mFirstInputFramesLeft = 0;
    int mContinuousInputFramesLeft = 0;
    int mFramesUntilUpdate = 0;

    bool mIsBlockActive = false;

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

    std::unique_ptr<ColorManager> colorManager;
    
    // ptr 소유권 Engine에게 있음(여기서도 unique로 받을 필요는 없을듯?)
	InputManager* mInputManager;
	EventManager* mEventManager;
};
