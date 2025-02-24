#pragma once

#include <memory>
#include <vector>
#include "Console.h"
#include "ConsoleTypes.h"

class Console;

class Panel : public std::enable_shared_from_this<Panel>
{
public:
	using Cell = console::Cell;
	using Rect = console::Rect;
	using Color = console::eColor;

	Panel(std::size_t top, std::size_t left,
		  std::size_t height, std::size_t width,
		  std::shared_ptr<Console> parent);
	~Panel();

	Rect GetInfo() const;
	std::vector<Cell>& GetBuffer();
	std::vector<uint8_t>& GetDirtyFlags();
	void ClosePanel();

	void Clear();

	void SetCell(std::size_t row, std::size_t column, wchar_t character,
				 Color fgColor = Color::Default,
				 Color bgColor = Color::Black);
	void SetCell(std::size_t row, std::size_t column, const Cell& cell);

	// 특정 줄 위치에 text를 표시합니다. (텍스트가 길어도 최대 1줄만 출력됩니다)
	void PrintAt(std::size_t row, std::size_t column, std::wstring_view text,
				 Color fgColor = Color::Default,
				 Color bgColor = Color::Black);

	// row 위치부터 텍스트를 출력합니다. 내용이 길 경우 여러줄에 걸쳐 출력합니다.
	void PrintLines(std::size_t row, std::wstring_view text,
				   Color fgColor = Color::Default,
				   Color bgColor = Color::Black);

	void ClearLines(std::size_t startRow, std::size_t numLines);

	void SetBackground(Color bgColor);
	void Scroll(std::size_t numLines);

	// 제일 아래 영역에 text 내용을 추가합니다. (기존 내용은 위로 밀려납니다)
	void AppendLines(std::wstring_view text,
					Color fgColor = Color::Default,
					Color bgColor = Color::Black);

	void FillRectangle(Rect rect, const Cell& cell);
	void FillRectangle(Rect rect, wchar_t character,
					   Color fgColor = Color::Default,
					   Color bgColor = Color::Black);

	void DrawRectangle(Rect rect, const Cell& cell);
	void DrawRectangle(Rect rect, wchar_t character,
					   Color fgColor = Color::Default,
					   Color bgColor = Color::Black);

protected:
	bool mClosed = false;
	std::size_t mTop;
	std::size_t mLeft;
	std::size_t mHeight;
	std::size_t mWidth;
	std::weak_ptr<Console> mParentConsole;
	std::vector<Cell> mBuffer;
	std::vector<uint8_t> mDirtyCells;
};
