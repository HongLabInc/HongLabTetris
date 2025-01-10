#include "TetrisBoard.h"
#include "ConsoleColor.h"

TetrisBoard::TetrisBoard(ConsoleRenderer& renderer, int x, int y, int width, int height)
    : mWidth(width), mHeight(height), mCurrentBlock(nullptr)
{
    mFrame = renderer.AddFrame(x, y, width, height);
    Init();
}

TetrisBoard::~TetrisBoard()
{
    delete mCurrentBlock; // 블록 제거
}

void TetrisBoard::Init()
{
    mFrame->Clear();
    mFrame->DrawRectangle(0, 0, mWidth, mHeight, Cell::borderCell);
    mFrame->FillRectangle(1, 1, mWidth - 2, mHeight - 2, Cell::emptyCell);
}

void TetrisBoard::Update(InputManager* im)
{
    if (mCurrentBlock == nullptr)
    {
        mCurrentBlock = new Block(mWidth / 2 - 2, 0);
        mCurrentBlock->Initalize();
    }

    mCurrentBlock->Update(im);

    if (mCurrentBlock->GetY() >= mHeight - 2)
    {
        delete mCurrentBlock;
        mCurrentBlock = nullptr;
    }
}

void TetrisBoard::Draw()
{
    DrawBoard();
    DrawBlock();
}

void TetrisBoard::DrawBoard()
{
    //출력 테스트용 임시 방편
	mFrame->FillRectangle(1, 0, mWidth -2, mHeight - 1, Cell::emptyCell);
}

void TetrisBoard::DrawBlock()
{
    if (mCurrentBlock == nullptr)
        return;

    int startX = mCurrentBlock->GetX();
    int startY = mCurrentBlock->GetY();
    const mat4x4& shape = mCurrentBlock->GetShape();
    const Cell blockCell = Cell(Cell::Type::Block, L'\u263A',
        static_cast<WORD>(ConsoleColor::BrightBlue) << 4);

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (shape[i][j] != 0)
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
