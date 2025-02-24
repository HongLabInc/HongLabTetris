#pragma once

#ifdef _WIN32
  #define CONSOLE_CLASS ConsoleWin32
  #include "ConsoleWin32.h"
#elif defined __unix__
  #define CONSOLE_CLASS ConsoleUnix
  #include "ConsoleUnix.h"
//#elif defined __linux__
//#elif defined __APLLE__
#endif

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <span>
#include "ConsoleTypes.h"

class Panel;

class Console : public std::enable_shared_from_this<Console>, CONSOLE_CLASS 
{
public:
	using Color = console::eColor;
	using Cell = console::Cell;
	using Rect = console::Rect;

	Console(std::size_t height, std::size_t width);
	virtual ~Console() = default;

	std::shared_ptr<Panel> AddPanel(std::size_t top, std::size_t left,
									std::size_t height, std::size_t width);

	void RemovePanel(const std::shared_ptr<Panel>& panel);

	void WriteRegion(Rect rect, const std::vector<Cell>& buffer,
					 std::vector<uint8_t>& dirtyFlags);
	void Refresh();
	void Clear();

	void RefreshWithDirtyMap(); // DEBUG용

protected:
	std::size_t mHeight;
	std::size_t mWidth;
	std::vector<Cell> mBuffer;
	std::vector<uint8_t> mDirtyCells; //bool 대신 uint8_t 사용
	std::vector<std::shared_ptr<Panel> > mPanels;

	// 아래부터 플랫폼별 별도 구현을 해야하는 low-level 함수들입니다.
public:
	void PrintDirtyBuffer();

	void GetScreenSize(int& rows, int& columns);
	void ClearScreen();

	void MoveCursor(int row, int column);

	void SetDefaultColor(Color color, Color backGroundColor = Color::Black);
	void WriteString(std::string_view text);
	void WriteColoredString(std::string_view text, Color color,
							Color backGroundColor = Color::Black);
	void PrintDebugDirtyMap();

private:
#ifdef _WIN32
	void printBufferRect(std::span<const CHAR_INFO> buffer, Rect rect);
	bool isRowFullyDirty(std::vector<uint8_t>::iterator rowBegin);
	void printDirtyBlock(std::size_t rowStart, std::size_t rowEnd,
						 const std::vector<CHAR_INFO>& buffer);
	void printDirtyLine(std::size_t row, std::size_t start, std::size_t end,
						const std::vector<CHAR_INFO>& buffer);
	void printDirtyCellsInRow(std::size_t row,
							  const std::vector<uint8_t>::iterator currentRowIter,
							  const std::vector<CHAR_INFO>& buffer);
#endif
};
