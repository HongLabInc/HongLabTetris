#include <cassert>
#include "ConsoleFrame.h"
#include "ConsoleRenderer.h"
#include "Managers/ConsoleColor.h"

ConsoleFrame::ConsoleFrame(int x, int y, int width, int height)
    : mX(x)
    , mY(y)
    , mWidth(width)
    , mHeight(height)
{
    mCells.resize(mWidth * mHeight, Cell::emptyCell);
}

void ConsoleFrame::Clear()
{
    std::fill(mCells.begin(), mCells.end(), Cell::emptyCell);
}

const Cell& ConsoleFrame::GetCell(int x, int y) const
{
    assert(0 <= x && x < mWidth);
    assert(0 <= y && y < mHeight);

    return mCells[y * mWidth + x];
}

void ConsoleFrame::SetCell(int x, int y, const Cell& cell)
{
    if(x < 0 || mWidth <= x || y < 0 || mHeight <= y)
        return;

	mCells[y * mWidth + x] = cell;
}

void ConsoleFrame::DrawRectangle(int x, int y, int width, int height, const Cell& borderCell)
{
	if(x >= mWidth || y >= mHeight)
		return;
	if(x + width <= 0 || y + height <= 0)
		return;

    for (int i = 0; i < width; ++i)
    {
        SetCell(x + i, y, borderCell);
        SetCell(x + i, y + height - 1, borderCell);
    }
    for (int j = 0; j < height; ++j)
    {
        SetCell(x, y + j, borderCell);
        SetCell(x + width - 1, y + j, borderCell);
    }
}

void ConsoleFrame::FillRectangle(int x, int y, int width, int height, const Cell& fillCell)
{
	if(x >= mWidth || y >= mHeight)
		return;
	if(x + width <= 0 || y + height <= 0)
		return;

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            SetCell(x + i, y + j, fillCell);
        }
    }
}

void ConsoleFrame::SetText(int x, int y, const std::wstring & text,
	WORD CollorAttr)
{
    if(x + text.length() <= 0 || mWidth <= x || y < 0 || mHeight <= y)
        return;

    Cell cell(Cell::Type::Empty, Cell::CHAR_EMPTY, CollorAttr);
    for (int i = 0; i < text.length(); ++i)
    {
        if (x + i < mWidth)
        {
            cell.SetChar(text[i]);
            SetCell(x + i, y, cell);
        }
    }
}

int ConsoleFrame::GetX() const
{
    return mX;
}

int ConsoleFrame::GetY() const
{
    return mY;
}

int ConsoleFrame::GetWidth() const
{
    return mWidth;
}

int ConsoleFrame::GetHeight() const
{
    return mHeight;
}
