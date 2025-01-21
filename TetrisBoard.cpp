#include "TetrisBoard.h"
#include "ConsoleColor.h"

TetrisBoard::TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height)
    : mWidth(width)
    , mHeight(height)
    , mCurrentBlock(nullptr)
    , mRenderer(renderer)
    , colorManager(std::make_unique<ColorManager>())
    , mFrame(renderer.AddFrame(x, y, width, height))
    , isFilled(std::vector<std::vector<bool>>(height, std::vector<bool>(width, false)))
    , rowCounts(std::vector<int>(height, 0))
    , maxVerticalPixels(height - 1)
    , maxHorizontalPixels(width - 2)
{
    colorManager->AddBrightColors();

    mFrame->Clear();
    mFrame->DrawRectangle(0, 0, mWidth, mHeight, Cell::borderCell);
    mFrame->FillRectangle(1, 1, mWidth - 2, mHeight - 2, Cell::emptyCell);

    InitBoard(0, 0, mWidth, mHeight);
}

TetrisBoard::~TetrisBoard()
{
    delete mCurrentBlock; // 블록 제거
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

void TetrisBoard::Update(InputManager* im)
{
    Instantiate();
   
    HandleInput(im);

    MoveBlockDown();

    CheckLines();
}

void TetrisBoard::Instantiate()
{
    if(mCurrentBlock == nullptr)
    {
        mCurrentBlock = new Block(mWidth / 2 - 2, 0, colorManager->GetRandomColor());
        mCurrentBlock->Initalize();

        mGhostBlock = new Block(mWidth / 2 - 2,0, ConsoleColor::Black);
        mGhostBlock->CopyFrom(*mCurrentBlock);
        mGhostBlock->SetTexture(ConsoleColor::Cyan);
        mIsBlockActive = true;
    }
}

void TetrisBoard::HandleInput(InputManager* im)
{
	// Input Block Move
	mCurrentBlock->UpdatePos();

	int key = im->DequeueInput();
	if (key != -1) {
		if (key == VK_LEFT) mCurrentBlock->MoveLeft();
		else if (key == VK_RIGHT) mCurrentBlock->MoveRight();
		else if (key == VK_UP) mCurrentBlock->Rotate();
		else if (key == VK_DOWN) {
			while (!CheckCollision(mCurrentBlock)) {
				mCurrentBlock->UpdatePos();
				mCurrentBlock->MoveDown();
			}
		}
	}

	if (CheckCollision(mCurrentBlock))
		mCurrentBlock->rollback();

	UpdateGhostBlock();
}

void TetrisBoard::MoveBlockDown()
{
	if (mFramesUntilUpdate <= 0)
	{
		// Update Basic Move
		mCurrentBlock->UpdatePos();
		mCurrentBlock->Update();

		if (CheckCollision(mCurrentBlock)) {

			mCurrentBlock->rollback();
			LockBlock();
			delete mCurrentBlock;
			mCurrentBlock = nullptr;
			mIsBlockActive = false;
		}
		mFramesUntilUpdate = mUpdateInterval;
	} else
	{
		--mFramesUntilUpdate;
	}
}

bool TetrisBoard::CheckCollision(Block* block)
{
    int worldX = block->GetX();
    int worldY = block->GetY();
    const int size = block->GetMatrixSize();

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
    rowCounts[row] = 0;
}

void TetrisBoard::CheckLines()
{
    for(int i = maxVerticalPixels - 1; i >= 0; i--)
        while(rowCounts[i] == maxHorizontalPixels)
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

void TetrisBoard::Draw()
{
    ClearBlockImage();

    if (mIsBlockActive)
    {
        Cell blockCell(static_cast<WORD>(ConsoleColor::Black) << 4);
        blockCell.SetForegroundColor(ConsoleColor::White);

        blockCell.SetChar(L'\u25A0');
        DrawBlock(mGhostBlock, blockCell);

        blockCell.SetChar(L'\u263A'); //스마일
        blockCell.SetForegroundColor(ConsoleColor::Black);
        blockCell.SetBackgroundColor(mCurrentBlock->GetTexture());
        DrawBlock(mCurrentBlock, blockCell);
    }
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

void TetrisBoard::DrawBlock(Block* block, const Cell& blockCell)
{
    if (mCurrentBlock == nullptr)
        return;

    int startX = block->GetX();
    int startY = block->GetY();
    int size = block->GetMatrixSize();

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
