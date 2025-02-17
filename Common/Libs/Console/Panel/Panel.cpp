#define NOMINMAX
#include <cassert>
#include <algorithm>
#include <execution>
#include "Panel.h"

Panel::Panel(std::size_t top, std::size_t left,
			 std::size_t height, std::size_t width,
			 std::shared_ptr<Console> parent)
	: mTop(top)
	, mLeft(left)
	, mHeight(height)
	, mWidth(width)
	, mParentConsole(parent)
{
	mBuffer.resize(mHeight * mWidth, console::DefaultCell);
	mDirtyCells.resize(mHeight * mWidth, true);
}

Panel::~Panel()
{
	ClosePanel();
}


Panel::Rect Panel::GetInfo() const
{
    return { mTop, mLeft, mHeight, mWidth };
}

std::vector<Panel::Cell>& Panel::GetBuffer()
{
    return mBuffer;
}

std::vector<uint8_t>& Panel::GetDirtyFlags()
{
    return mDirtyCells;
}

void Panel::ClosePanel()
{
	if (mClosed)
	{
		return;
	}

	mClosed = true;

	if (auto parent = mParentConsole.lock())
	{
        // panel이 없어진 위치에 Console의 DirtyFlag를 켜주기 위해 호출
        Clear();
        parent->WriteRegion(GetInfo(), mBuffer, mDirtyCells);

		parent->RemovePanel(shared_from_this());
		mParentConsole.reset();
	}
}

void Panel::Clear()
{
    std::fill(mBuffer.begin(), mBuffer.end(), console::DefaultCell);
    std::fill(mDirtyCells.begin(), mDirtyCells.end(), true);
}

void Panel::SetCell(std::size_t row, std::size_t column, 
                    wchar_t character, Color fgColor, Color bgColor)
{
    if (row >= mHeight || column >= mWidth)
    {
        return;
    }

	mBuffer[row * mWidth + column] = {character, fgColor, bgColor};
    mDirtyCells[row * mWidth + column] = true;
}

void Panel::SetCell(std::size_t row, std::size_t column, const Cell& cell)
{
    if (row >= mHeight || column >= mWidth)
    {
        return;
    }

    mBuffer[row * mWidth + column] = cell;
    mDirtyCells[row * mWidth + column] = true;
}

void Panel::PrintAt(std::size_t row, std::size_t column,
                    std::wstring_view text, Color fgColor, Color bgColor)
{
    if (row >= mHeight || column >= mWidth)
        return;

	const std::size_t startPosition = row * mWidth + column;
    const std::size_t emptySpace = mWidth - column;
    const std::size_t printSize = std::min(text.size(), emptySpace);

    std::transform
	(std::execution::unseq,
     text.begin(), text.begin() + printSize, mBuffer.begin() + startPosition,
	 [fgColor, bgColor](wchar_t character) -> Cell
	 {
         return {character, fgColor, bgColor};
	 });
	std::fill(mDirtyCells.begin() + startPosition,
              mDirtyCells.begin() + startPosition + printSize,
              true);
}

void Panel::PrintLines(std::size_t row, std::wstring_view text,
                      Color fgColor, Color bgColor)
{
    if (row >= mHeight)
        return;

    const std::size_t startPosition = row * mWidth;
    const std::size_t printSize = std::min(mBuffer.size() - row * mWidth, text.size());

	std::transform
	(std::execution::unseq,
	 text.begin(), text.begin() + printSize, mBuffer.begin() + startPosition,
	 [fgColor, bgColor](wchar_t character) -> Cell
	 {
		 return {character, fgColor, bgColor};
	 });
	std::fill(mDirtyCells.begin() + startPosition,
              mDirtyCells.begin() + startPosition + printSize,
              true);
}

void Panel::ClearLines(std::size_t startRow, std::size_t numLines)
{
    if (startRow >= mHeight)
        return;

    const std::size_t printNumLines = std::min(numLines, mHeight - startRow);
    const std::size_t printSize = printNumLines * mWidth;

	{
		auto startPosition = mBuffer.begin() + startRow * mWidth;
		std::fill(startPosition, startPosition + printSize, console::DefaultCell);
	}

	{
		auto startPosition = mDirtyCells.begin() + startRow * mWidth;
		std::fill(startPosition, startPosition + printSize, true);
	}
}

void Panel::SetBackground(Color bgColor)
{
	std::for_each
    (std::execution::unseq,
     mBuffer.begin(), mBuffer.end(),
	 [bgColor](Cell& cell)
     {
		cell.bgColor = bgColor;
     });
    std::fill(mDirtyCells.begin(), mDirtyCells.end(), true);
}

void Panel::Scroll(std::size_t numLines)
{
    if (numLines >= mHeight)
    {
        Clear();
        return;
    }

    auto first = mBuffer.begin();
    auto middle = mBuffer.begin() + numLines * mWidth;
    auto last = mBuffer.end();

    std::fill(std::move(middle, last, first), mBuffer.end(), console::DefaultCell);
    std::fill(mDirtyCells.begin(), mDirtyCells.end(), true);
}

void Panel::AppendLines(std::wstring_view text, Color fgColor, Color bgColor)
{
    const std::size_t numLines = std::min(mHeight, text.size() / mWidth + 1);
    const std::size_t printSize = std::min(text.size(), mWidth * numLines);

    auto startPosition = mBuffer.begin() + (mHeight - numLines) * mWidth;

    Scroll(numLines);
    std::transform
    (std::execution::unseq,
     text.begin(), text.begin() + printSize, startPosition,
     [fgColor, bgColor](wchar_t character) -> Cell
     {
         return {character, fgColor, bgColor};
     });
}

void Panel::FillRectangle(Rect rect, const Cell& cell)
{
	const std::size_t numFillRows = std::min(rect.height, mHeight - rect.top);
	const std::size_t numFillColumns = std::min(rect.width, mWidth - rect.left);

	for (std::size_t row = 0; row < numFillRows; ++row)
	{
		{
			auto startPosition = mBuffer.begin() + (rect.top + row) * mWidth + rect.left;
			std::fill(startPosition, startPosition + numFillColumns, cell);
		}
		{
			auto startPosition = mDirtyCells.begin() + (rect.top + row) * mWidth + rect.left;
			std::fill(startPosition, startPosition + numFillColumns, true);
		}
	}
}

void Panel::FillRectangle(Rect rect, wchar_t character, Color fgColor, Color bgColor)
{
    FillRectangle(rect, {character, fgColor, bgColor});
}

void Panel::DrawRectangle(Rect rect, const Cell& cell)
{
	const std::size_t numFillRows = std::min(rect.height, mHeight - rect.top);
	const std::size_t numFillColumns = std::min(rect.width, mWidth - rect.left);

    if (numFillRows == 0 || numFillColumns == 0)
    {
        return;
    }

    {
		auto startPosition = mBuffer.begin() + rect.top * mWidth + rect.left;
		std::fill(startPosition, startPosition + numFillColumns, cell); //위
		startPosition = mBuffer.begin() + (rect.top + numFillRows - 1) * mWidth + rect.left;
		std::fill(startPosition, startPosition + numFillColumns, cell); //아래
    }
    {
		auto startPosition = mDirtyCells.begin() + rect.top * mWidth + rect.left;
		std::fill(startPosition, startPosition + numFillColumns, true); //위
		startPosition = mDirtyCells.begin() + (rect.top + numFillRows - 1) * mWidth + rect.left;
		std::fill(startPosition, startPosition + numFillColumns, true); //아래
    }

    if (numFillRows <= 2)
    {
        return;
    }

    for (std::size_t row = rect.top + 1; row < rect.top + numFillRows - 1; ++row)
    {
        mBuffer[row * mWidth + rect.left] = cell; //왼쪽
        mBuffer[row * mWidth + rect.left + numFillColumns - 1] = cell; //오른쪽

		//파편화에 의한 API 호출 증가 방지를 위해 DirtyFlag는 중간지점도 모두 채움
        auto lineStart = mDirtyCells.begin() + row * mWidth + rect.left;
        std::fill(lineStart, lineStart + numFillColumns, true);
    }
}

void Panel::DrawRectangle(Rect rect, wchar_t character, Color fgColor, Color bgColor)
{
    DrawRectangle(rect, {character, fgColor, bgColor});
}
