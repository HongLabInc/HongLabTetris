#define NOMINMAX
#include <algorithm>
#include <string>
#include "ModelPointer.h"
#include "TetrisBoard.h"
#include "Managers/ConsoleColor.h"

const std::array<int, 100> TetrisBoard::scoreTable = TetrisBoard::generateScoreTable();
TetrisBoard::TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height,InputManager* im,EventManager* em)
    : mWidth(width)
    , mHeight(height)
    , mRenderer(renderer)
    , colorManager(std::make_unique<ColorManager>())
    , mFrame(renderer.AddFrame(x, y, width, height))
    , mQueueFrame1(renderer.AddFrame(x + width + 1, y, mQueueFrameSize, mQueueFrameSize))
	, mQueueFrame2(renderer.AddFrame(x + width + 1, y + mQueueFrameSize + 1, mQueueFrameSize, mQueueFrameSize))
	, mScoreFrame(renderer.AddFrame(x + width, y + mQueueFrameSize * 2 + 2, mQueueFrameSize + 2, 5))
    , isFilled(std::vector<std::vector<bool>>(height, std::vector<bool>(width, false)))
    , rowCounts(std::vector<int>(height, 0))
    , maxVerticalPixels(height - 1)
    , maxHorizontalPixels(width - 2)
	, mInputManager(im)
    , mEventManager(em)
{
    colorManager->AddBrightColors();

    mFrame->Clear();
    mFrame->DrawRectangle(0, 0, mWidth, mHeight, Cell::borderCell);
    mFrame->FillRectangle(1, 1, mWidth - 2, mHeight - 2, Cell::emptyCell);
    mQueueFrame1->Clear();
    mQueueFrame1->DrawRectangle(0, 0, mQueueFrameSize, mQueueFrameSize, Cell::borderCell);
    mQueueFrame1->FillRectangle(1, 1, mQueueFrameSize - 2, mQueueFrameSize - 2, Cell::emptyCell);
    mQueueFrame2->Clear();
    mQueueFrame2->DrawRectangle(0, 0, mQueueFrameSize, mQueueFrameSize, Cell::borderCell);
    mQueueFrame2->FillRectangle(1, 1, mQueueFrameSize - 2, mQueueFrameSize - 2, Cell::emptyCell);
    mScoreFrame->Clear();
    mScoreFrame->SetText(1, 0, L"S C O R E", static_cast<WORD>(ConsoleColor::BrightYellow));
    UpdateScore();

    InitBoard(0, 0, mWidth, mHeight);

    mSecondBlock = std::make_unique<Block>(2, 2, colorManager->GetRandomColor());
    mThirdBlock = std::make_unique<Block>(2, 2, colorManager->GetRandomColor());
	mSecondBlock->Initalize();
	mThirdBlock->Initalize();
}

TetrisBoard::~TetrisBoard()
{
    mRenderer.RemoveFrame(mFrame);
}

void TetrisBoard::InitBoard(int x,int y,int width,int height) {

    if(x >= mWidth || y >= mHeight)
        return;
    if(x + width <= 0 || y + height <= 0)
        return;

    for(int i = 0; i < width; ++i)
    {
        isFilled[y + height - 1][x + i] = true;
    }
    for(int j = 0; j < height; ++j)
    {
        isFilled[y + j][x] = true;
        isFilled[y + j][x + width - 1] = true;
    }
}

void TetrisBoard::Update(float deltaTime)
{
    if (mIsBlockActive == false)
    {
		Instantiate();
    }
   
    HandleInput();

    MoveBlockDown(deltaTime);

    if (mIsBlockActive == false)
    {
		CheckLines();
    }
}

void TetrisBoard::Instantiate()
{
	mCurrentBlock = std::move(mSecondBlock);
	mSecondBlock = std::move(mThirdBlock);
	mThirdBlock = std::make_unique<Block>(2, 2, colorManager->GetRandomColor());
    mThirdBlock->Initalize();

    mCurrentBlock->SetX(mWidth / 2 - 2);

	mGhostBlock = std::make_unique<Block>(mCurrentBlock->GetX(), 0, ConsoleColor::Black);
	mGhostBlock->CopyFrom(*mCurrentBlock);
	mGhostBlock->SetTexture(ConsoleColor::Cyan);
    
    mQueueFrame1->FillRectangle(1, 1, mQueueFrameSize - 2, mQueueFrameSize - 2, Cell::emptyCell);
    Cell blockCell{};
    blockCell.SetBackgroundColor(mSecondBlock->GetTexture());
    DrawBlock(mQueueFrame1, mSecondBlock, blockCell);

    mQueueFrame2->FillRectangle(1, 1, mQueueFrameSize - 2, mQueueFrameSize - 2, Cell::emptyCell);
	blockCell.SetForegroundColor(ConsoleColor::White);
	blockCell.SetBackgroundColor(ConsoleColor::Black);
	blockCell.SetChar(L'\u25A0');
	DrawBlock(mQueueFrame2, mThirdBlock, blockCell);

	mIsBlockActive = true;
}

void TetrisBoard::HandleInput()
{
	// Input Block Move
	mCurrentBlock->UpdatePos();

	int key = mInputManager->DequeueInput();
	if (key != -1) {
		if (key == VK_LEFT) mCurrentBlock->MoveLeft();
		else if (key == VK_RIGHT) mCurrentBlock->MoveRight();
		else if (key == VK_UP) {
            TryRotateWithWallKick(true);
		}
		else if (key == VK_SPACE) {
            while (!CheckCollision(mCurrentBlock)) {
                mCurrentBlock->UpdatePos();
                mCurrentBlock->MoveDown();
            }
            mIsBlockReadyToLock = true;
        }
        else if (key == VK_DOWN) {
            mIsSoftDropping = true;
        }
	}
	
	if (!mInputManager->IsKeyPressed(VK_DOWN)) {
		mIsSoftDropping = false;
	}

	if (CheckCollision(mCurrentBlock))
		mCurrentBlock->rollback();

	UpdateGhostBlock();
}

void TetrisBoard::MoveBlockDown(float deltaTime)
{
	float currentInterval = mIsSoftDropping ? mSoftDropIntervalSeconds : mDropIntervalSeconds;
	
	mTimeUntilDropSeconds += deltaTime * mFallSpeed;
	if (mIsBlockReadyToLock || mTimeUntilDropSeconds >= currentInterval)
	{
		// Update Basic Move
		mCurrentBlock->UpdatePos();
		mCurrentBlock->Update();

		if (CheckCollision(mCurrentBlock)) {

			mCurrentBlock->rollback();
			LockBlock();
            mCurrentBlock.reset();
			mIsBlockActive = false;
            mIsBlockReadyToLock = false;
		}
		mTimeUntilDropSeconds = 0.0f;
	}
}

bool TetrisBoard::CheckCollision(std::unique_ptr<Block>& block)
{
    int worldX = block->GetX();
    int worldY = block->GetY();
    const int size = block->GetMatrixSize();

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            
            if(block->GetValue(i, j) != 0) {
                int boardX = worldX + j;
                int boardY = worldY + i;

                if(boardX < 0 || boardX >= mWidth ||
                   boardY < 0 || boardY >= mHeight) {
                    return true;
                }

                // 해당 보드 위치에 이미 블록이 있는지 검사
                if(isFilled[boardY][boardX]) {
                    return true;
                }
            }
        }
    }
    return false;
}

void TetrisBoard::UpdateGhostBlock() {

    if(mCurrentBlock == nullptr) return;

    mGhostBlock->CopyFrom(*mCurrentBlock);
    mGhostBlock->SetTexture(ConsoleColor::Cyan);

    while(!CheckCollision(mGhostBlock)) {
        mGhostBlock->UpdatePos();
        mGhostBlock->MoveDown();
    }

    mGhostBlock->rollback();
}

void TetrisBoard::LockBlock()
{
    int worldX = mCurrentBlock->GetX();
    int worldY = mCurrentBlock->GetY();
    const int size = mCurrentBlock->GetMatrixSize();

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            if(mCurrentBlock->GetValue(i, j) != 0) {
                int boardX = worldX + j;
                int boardY = worldY + i;

                // 블록이 보드 내부인지 확인
                if(boardX >= 0 && boardX < mWidth &&
                   boardY >= 0 && boardY < mHeight) {
                    isFilled[boardY][boardX] = true;
                    rowCounts[boardY]++;
                    Cell blockCell(Cell::Type::Block, L' ');
                    blockCell.SetBackgroundColor(mCurrentBlock->GetTexture());
                    mFrame->SetCell(boardX, boardY, blockCell);
                }
            }
        }
    }
}

void TetrisBoard::ClearLine(int row)
{
    const int columnStart = 1;
    const int height = 1;

    rowCounts[row] = 0;
    mFrame->FillRectangle(row, columnStart, mWidth - 2, height, Cell::emptyCell);
	std::fill(isFilled[row].begin() + 1, isFilled[row].end() - 1, false);
}

void TetrisBoard::CheckLines()
{
    bool isAnyLineCleared = false;

    for (int i = maxVerticalPixels - 1; i >= 0; --i)
    {
        if (rowCounts[i] == maxHorizontalPixels)
        {
            isAnyLineCleared = true;
            ClearLine(i);
            mScore = std::min(999999999, mScore + scoreTable[mCombo]);
            mCombo = std::min(99, mCombo + 1);
        }
    }

    if (isAnyLineCleared)
    {
		MoveLines();
    }
    else
    {
        mCombo = 0;
    }

    UpdateScore();
}

void TetrisBoard::UpdateScore()
{
    std::wstring score = std::to_wstring(mScore);
    int position = (mQueueFrameSize / 2 + 1) - static_cast<int>(score.length() / 2);

    mScoreFrame->FillRectangle(0, 2, mQueueFrameSize + 2, 1, Cell::emptyCell);
    mScoreFrame->SetText(position, 2, score, static_cast<WORD>(ConsoleColor::BrightWhite));

	mScoreFrame->FillRectangle(0, 4, mQueueFrameSize + 2, 1, Cell::emptyCell);
    if (mCombo >= 3)
    {
        std::wstring combo = L"Combo " + std::to_wstring(mCombo);
		position = (mQueueFrameSize / 2 + 1) - static_cast<int>(combo.length() / 2);

		mScoreFrame->SetText(position, 4, combo, static_cast<WORD>(ConsoleColor::BrightGreen));
    }
}

void TetrisBoard::MoveLine(int targetLine, int sourceLine)
{
	for (int i = 1; i < mWidth - 1; ++i)
	{
		mFrame->SetCell(i, targetLine, mFrame->GetCell(i, sourceLine));
        isFilled[targetLine][i] = isFilled[sourceLine][i];
	}
	rowCounts[targetLine] = rowCounts[sourceLine];
	ClearLine(sourceLine);
}

void TetrisBoard::MoveLines() {

    int targetLine = maxVerticalPixels - 1;
    int sourceLine = targetLine - 1;
    while(sourceLine >= 0) {

        if(rowCounts[targetLine]) {
            targetLine--;
            sourceLine--;
        } else if(rowCounts[sourceLine]) {
            isFilled[targetLine] = isFilled[sourceLine];
            rowCounts[targetLine] = rowCounts[sourceLine];

            MoveLine(targetLine, sourceLine);
             
            targetLine--;
            sourceLine--;
        } else if(!rowCounts[sourceLine])
            sourceLine--;
    }


}

void TetrisBoard::Draw()
{
    ClearBlockImage();

    if (mIsBlockActive)
    {
        Cell blockCell(static_cast<WORD>(ConsoleColor::Black) << 4);
        blockCell.SetForegroundColor(ConsoleColor::White);

        blockCell.SetChar(L'\u25A0');
        DrawBlock(mFrame, mGhostBlock, blockCell);

        blockCell.SetChar(L'\u263A'); //스마일
        blockCell.SetForegroundColor(ConsoleColor::Black);
        blockCell.SetBackgroundColor(mCurrentBlock->GetTexture());
        DrawBlock(mFrame, mCurrentBlock, blockCell);
    }
}

bool TetrisBoard::IsFull()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 1; j < mWidth - 1; j++) {
			if(isFilled[i][j] == true) {
				return true;
			}
		}
    }

    return false;
}

int TetrisBoard::GetTopRow()
{
    for(int i = 0; i < mHeight - 1; ++i) {
        for(int j = 1; j < mWidth - 1; ++j) { 
            if(isFilled[i][j]) {
                return i;
            }
        }
    }
    return mHeight; 
}

void TetrisBoard::ClearBlockImage()
{
    for (int row = 0; row < mHeight - 1; ++row)
    {
        for (int column = 1; column < mWidth - 1; ++column)
        {
            if (isFilled[row][column] == false)
            {
                mFrame->SetCell(column, row, Cell::emptyCell);
            }
        }
    }
}

void TetrisBoard::DrawBlock(ConsoleFrame* frame, std::unique_ptr<Block>& block, const Cell& blockCell)
{
    int startX = block->GetX();
    int startY = block->GetY();
    int size = block->GetMatrixSize();

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (block->GetValue(i, j) != 0)
            {
                int boardX = startX + j;
                int boardY = startY + i;

                if (boardX >= 1 && boardX < frame->GetWidth() - 1 && boardY >= 1 && boardY < frame->GetHeight() - 1)
                {
                    frame->SetCell(boardX, boardY, blockCell);
                }
            }
        }
    }
}

std::array<int, 100> TetrisBoard::generateScoreTable()
{
    std::array<int, 100> table{};

    table[0] = 100;
    for (int i = 1; i < 100; ++i)
    {
        table[i] = static_cast<int>(table[i - 1] * 1.54f);
    }

    return table;
}

bool TetrisBoard::TryRotateWithWallKick(bool clockwise)
{
    if (mCurrentBlock == nullptr) return false;
    
    ShapeType pieceType = mCurrentBlock->GetType();
    RotationState currentRotation = mCurrentBlock->GetRotation();
    RotationState nextRotation = mCurrentBlock->GetNextRotation(clockwise);
    
    if (pieceType == SHAPE_O) {
        // O블록은 회전 사용 X
        return true;
    }
    
    int originalX = mCurrentBlock->GetX();
    int originalY = mCurrentBlock->GetY();
    
    RotationTransition transition;
    if (currentRotation == ROTATION_0 && nextRotation == ROTATION_1) {
        transition = TRANS_0_TO_1;
    } else if (currentRotation == ROTATION_1 && nextRotation == ROTATION_0) {
        transition = TRANS_1_TO_0;
    } else if (currentRotation == ROTATION_1 && nextRotation == ROTATION_2) {
        transition = TRANS_1_TO_2;
    } else if (currentRotation == ROTATION_2 && nextRotation == ROTATION_1) {
        transition = TRANS_2_TO_1;
    } else if (currentRotation == ROTATION_2 && nextRotation == ROTATION_3) {
        transition = TRANS_2_TO_3;
    } else if (currentRotation == ROTATION_3 && nextRotation == ROTATION_2) {
        transition = TRANS_3_TO_2;
    } else if (currentRotation == ROTATION_3 && nextRotation == ROTATION_0) {
        transition = TRANS_3_TO_0;
    } else { // currentRotation == ROTATION_0 && nextRotation == ROTATION_3
        transition = TRANS_0_TO_3;
    }
    
    const SRSKickData& kickData = SRS_KICK_DATA_JLSTZ[transition];
    
    for (int i = 0; i < NumOfKickTest; i++) {
        mCurrentBlock->UpdatePos();
        if (clockwise) {
            mCurrentBlock->Rotate();
        } else {
            mCurrentBlock->RotateCounterClockwise();
        }
        
        int kickX = kickData.kicks[i].x;
        int kickY = kickData.kicks[i].y;
        
        int newX = originalX + kickX;
        int newY = originalY - kickY;
        
        mCurrentBlock->SetPosition(newX, newY);
        
        if (!CheckCollision(mCurrentBlock)) {
            return true;
        }
        
        mCurrentBlock->rollback();
    }
    
    return false;
}

void TetrisBoard::SetFallSpeed(float speed)
{
    mFallSpeed = std::clamp(speed, mFallMinSpeed, mFallMaxSpeed);
}
