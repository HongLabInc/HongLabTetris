#pragma once
#include <queue>
#include <array>
#include "ConsoleFrame.h"
#include "Block.h"
#include "InputManager.h"
#include "ColorManager.h"
#include "EventManager.h"
#include "ConsoleRenderer.h"

class TetrisBoard
{
public:
    TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height,InputManager* im,EventManager* em = nullptr);
    ~TetrisBoard();

    void InitBoard(int x,int y,int width,int height);
    void Update();
    void HandleInput();
    void MoveBlockDown();
    void UpdateGhostBlock();
    void Draw();

    bool IsFull();
    int GetTopRow();
    bool CheckCollision(std::unique_ptr<Block>& block);

private:
    void ClearBlockImage();
	void DrawBlock(ConsoleFrame* frame, std::unique_ptr<Block>& block, const Cell & blockCell);
    void LockBlock();

    void Instantiate();

    void CheckLines();
    void UpdateScore();
    void MoveLine(int targetLine, int sourceLine);
    void ClearLine(int row); // 꽉채워진 라인 지우기
    void MoveLines();
    static std::array<int, 100> generateScoreTable();

    static constexpr int mUpdateInterval = 100;
    static constexpr int mFirstInputDelayFrames = 8;
    static constexpr int mContinuousInputDelayFrames = 24;
    static constexpr int mQueueFrameSize = 8;

    static const std::array<int, 100> scoreTable;

    int mWidth;
    int mHeight;

    int mFirstInputFramesLeft = 0;
    int mContinuousInputFramesLeft = 0;
    int mFramesUntilUpdate = 0;

    int mScore = 0;
    int mCombo = 0;

    bool mIsBlockActive = false;
    bool mIsBlockReadyToLock = false;

    ConsoleRenderer& mRenderer;
    ConsoleFrame* mFrame;
    ConsoleFrame* mQueueFrame1;
    ConsoleFrame* mQueueFrame2;
    ConsoleFrame* mScoreFrame;
    std::unique_ptr<Block> mCurrentBlock;
    std::unique_ptr<Block> mSecondBlock;
    std::unique_ptr<Block> mThirdBlock;
    std::unique_ptr<Block> mGhostBlock;

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
