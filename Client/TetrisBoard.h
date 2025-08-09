#pragma once
#include <queue>
#include <array>
#include "ConsoleFrame.h"
#include "Block.h"
#include "Managers/InputManager.h"
#include "Managers/ColorManager.h"
#include "Managers/EventManager.h"
#include "ConsoleRenderer.h"

class TetrisBoard
{
public:
    TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height, InputManager* im,EventManager* em = nullptr);
    ~TetrisBoard();

    void InitBoard(int x,int y,int width,int height);
    void Update(float deltaTime);
    virtual void HandleInput();
    void MoveBlockDown(float deltaTime);
    void UpdateGhostBlock();
    void Draw();

    bool IsFull();
    int GetTopRow();
    bool CheckCollision(std::unique_ptr<Block>& block);
    bool TryRotateWithWallKick(bool clockwise = true);
    void SetFallSpeed(float speed);

    int GetScore() const { return mScore; }

protected:
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

    static constexpr float mDropIntervalSeconds = 0.1f;
    static constexpr float mSoftDropIntervalSeconds = mDropIntervalSeconds / 20;

    static constexpr int mFirstInputDelayFrames = 8;
    static constexpr int mContinuousInputDelayFrames = 24;
    static constexpr int mQueueFrameSize = 8;

	static constexpr float mFallMinSpeed = 1.0f;
    static constexpr float mFallMaxSpeed = 10.0f;

    static const std::array<int, 100> scoreTable;

    int mWidth;
    int mHeight;

    int mFirstInputFramesLeft = 0;
    int mContinuousInputFramesLeft = 0;

    float mTimeUntilDropSeconds = 0.0f;
    float mFallSpeed = 1.0f;

    int mScore = 0;
    int mCombo = 0;

    bool mIsBlockActive = false;
    bool mIsBlockReadyToLock = false;
    bool mIsSoftDropping = false;

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
