#include "TetrisBoard.h"
#include "ConsoleColor.h"

TetrisBoard::TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height)
    : mWidth(width)
    ,mHeight(height)
    ,mCurrentBlock(nullptr)
    ,mRenderer(renderer)
    ,colorManager(nullptr)
{
    mFrame = renderer.AddFrame(x, y, width, height);
    isFilled = std::vector<std::vector<bool>>(height, std::vector<bool>(width,false));

    rowCounts = std::vector<int>(height, 0);
    maxVerticalPixels = height - 1;
    maxHorizontalPixels = width - 2;

    colorManager = new ColorManager();
    colorManager->AddBrightColors();

    Init();
}

TetrisBoard::~TetrisBoard()
{
    delete mCurrentBlock; // 블록 제거
    mRenderer.RemoveFrame(mFrame);
}

void TetrisBoard::Init()
{
    mFrame->Clear();
    mFrame->DrawRectangle(0, 0, mWidth, mHeight, Cell::borderCell);
    mFrame->FillRectangle(1, 1, mWidth - 2, mHeight - 2, Cell::emptyCell);
    InitBoard(0, 0, mWidth, mHeight);
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

void TetrisBoard::Update(InputManager* im)
{
    Instantiate();
   
    if (mIsButtonHeld == true
        && im->IsKeyPressed(VK_LEFT) == false
        && im->IsKeyPressed(VK_RIGHT) == false
        && im->IsKeyPressed(VK_UP) == false
        && im->IsKeyPressed(VK_DOWN) == false
        )
    {
        mIsButtonHeld = false;
    }

    if (mIsButtonHeld == false)
    {
        mContinuousInputFramesLeft = 0;
    }

    if (mFirstInputFramesLeft <= 0 && mContinuousInputFramesLeft <= 0)
    {
        // Input Block Move
        mCurrentBlock->UpdatePos();

        if (im->IsKeyPressed(VK_LEFT)) mCurrentBlock->MoveLeft();
        else if (im->IsKeyPressed(VK_RIGHT)) mCurrentBlock->MoveRight();
        else if (im->IsKeyPressed(VK_UP)) mCurrentBlock->Rotate();
        else if (im->IsKeyPressed(VK_DOWN)) {
            while (!CheckCollision()) {
                mCurrentBlock->UpdatePos();
                mCurrentBlock->MoveDown();
            }
        }

        if (CheckCollision())
            mCurrentBlock->rollback();

        mFirstInputFramesLeft = mFirstInputDelayFrames;
        if (mIsButtonHeld == false)
        {
			mContinuousInputFramesLeft = mContinuousInputDelayFrames;
			mIsButtonHeld = true;
        }
    }
    else
    {
		if (mFirstInputFramesLeft > 0)
		{
			--mFirstInputFramesLeft;
		}
        if (mContinuousInputFramesLeft > 0)
        {
			--mContinuousInputFramesLeft;
        }
    }
   
 
    if (mFramesUntilUpdate <= 0)
    {
        // Update Basic Move
        mCurrentBlock->UpdatePos();
        mCurrentBlock->Update();

        if (CheckCollision()) {

            mCurrentBlock->rollback();
            LockBlock();
            mCurrentBlock = nullptr;
        }
        mFramesUntilUpdate = mUpdateInterval;
    }
    else
    {
        --mFramesUntilUpdate;
    }

    CheckLines();
}

void TetrisBoard::Draw()
{
    DrawBoard();
    DrawBlock();
}

bool TetrisBoard::CheckCollision()
{
    int worldX = mCurrentBlock->GetX();
    int worldY = mCurrentBlock->GetY();
    const int size = mCurrentBlock->GetMatrixSize();

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            
            if(mCurrentBlock->GetValue(i, j) != 0) {
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
                }
            }
        }
    }
}

void TetrisBoard::Instantiate()
{
    if(mCurrentBlock == nullptr)
    {
        mCurrentBlock = new Block(mWidth / 2 - 2, 0, colorManager->GetRandomColor());
        mCurrentBlock->Initalize();
    }
}


void TetrisBoard::ClearLine(int row)
{
    rowCounts[row] = 0;
}

void TetrisBoard::CheckLines()
{
    for(int i = maxVerticalPixels - 1; i >= 0; i--)
        if(rowCounts[i] == maxHorizontalPixels)
            ClearLine(i);

    MoveLines();
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

            rowCounts[sourceLine] = 0;
            std::fill(isFilled[sourceLine].begin() + 1,isFilled[sourceLine].end() - 1,false);
             
            targetLine--;
            sourceLine--;
        } else if(!rowCounts[sourceLine])
            sourceLine--;
    }
}



void TetrisBoard::DrawBoard()
{
    //출력 테스트용 임시 방편
    mFrame->FillRectangle(1,0,mWidth -2,mHeight - 1,Cell::emptyCell);
    for(int j = 0; j < mHeight; ++j)
    {
        for(int i = 0; i < mWidth; ++i)
        {
            if (isFilled[j][i]) mFrame->SetCell(i, j, Cell::borderCell);
        }
    }
}

void TetrisBoard::DrawBlock()
{
    if (mCurrentBlock == nullptr)
        return;

    int startX = mCurrentBlock->GetX();
    int startY = mCurrentBlock->GetY();
    int size = mCurrentBlock->GetMatrixSize();
    const Cell blockCell = Cell(Cell::Type::Block, L'\u263A',
        static_cast<WORD>(mCurrentBlock->GetTexture()) << 4);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (mCurrentBlock->GetValue(i, j) != 0)
            {
                int boardX = startX + j;
                int boardY = startY + i;

                if (boardX >= 1 && boardX < mWidth - 1 && boardY >= 1 && boardY < mHeight - 1)
                {
                    mFrame->SetCell(boardX, boardY, blockCell);
                }
            }
        }
    }
}
