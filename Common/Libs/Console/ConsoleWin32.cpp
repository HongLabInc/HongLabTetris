#ifdef _WIN32
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <Windows.h>
#include <locale>
#include <cassert>
#include <iostream>
#include "Console.h"

namespace
{
	WORD toColor32(console::eColor color);
	WORD toBackgroundColor32(console::eColor color);
}

ConsoleWin32::ConsoleWin32()
{
	mConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (mConsoleOutput == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("Failed to get console output handle");
	}
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
}

ConsoleWin32::~ConsoleWin32()
{
}

void Console::PrintDirtyBuffer()
{
	// Cell -> CHAR_INFO 변환
	const std::size_t bufferSize = mWidth * mHeight;
	std::vector<CHAR_INFO> buffer(bufferSize);

	std::transform
	(mBuffer.begin(), mBuffer.end(), buffer.begin(),
	 [this](const Cell& cell) -> CHAR_INFO
	 {
		 CHAR_INFO charInfo{};

		 charInfo.Char.UnicodeChar = cell.character;
		 charInfo.Attributes = toColor32(cell.fgColor) | toBackgroundColor32(cell.bgColor);
		 return charInfo;
	 });

	// DirtyCell로 꽉찬 줄이 연속으로 있는 경우 모아서 블록으로 한번에 출력
	// [blockStartRow, blockEndRow) 범위를 출력
	std::size_t blockStartRow = 0;
	std::size_t blockEndRow = 0;

	auto currentRowIter = mDirtyCells.begin();

	for (std::size_t row = 0; row < mHeight; ++row, currentRowIter += mWidth)
	{
		if (isRowFullyDirty(currentRowIter))
		{
			// 현재 행 전체가 DirtyCell이면 블록에 포함시키고 넘어가기
			++blockEndRow;
			continue;
		}
		printDirtyBlock(blockStartRow, blockEndRow, buffer);
		blockStartRow = row;
		blockEndRow = row;
		printDirtyCellsInRow(row, currentRowIter, buffer);
	}

	printDirtyBlock(blockStartRow, blockEndRow, buffer);
}

bool Console::isRowFullyDirty(std::vector<uint8_t>::iterator rowBegin)
{
	[[maybe_unused]] const std::size_t index = rowBegin - mDirtyCells.begin();
	assert((index % mWidth) == 0 && "rowBegin is not aligned to row start");

	return (std::find(rowBegin, rowBegin + mWidth, false) == rowBegin + mWidth);
}

void Console::printBufferRect(std::span<const CHAR_INFO> buffer, Rect rect)
{
	if (rect.height == 0 || rect.width == 0)
	{
		return;
	}

	SMALL_RECT writeRegion;
	writeRegion.Top = static_cast<SHORT>(rect.top);
	writeRegion.Left = static_cast<SHORT>(rect.left);
	writeRegion.Bottom = static_cast<SHORT>(rect.top + rect.height - 1);
	writeRegion.Right = static_cast<SHORT>(rect.left + rect.width - 1);

	COORD rectSize;
	rectSize.X = static_cast<SHORT>(rect.width);
	rectSize.Y = static_cast<SHORT>(rect.height);

	COORD startPosition = {0, 0};

	if (!WriteConsoleOutputW(
		mConsoleOutput, // 콘솔 출력 핸들
		buffer.data(), // CHAR_INFO 배열의 포인터
		rectSize, // 버퍼 크기 (전체 행/열)
		startPosition, // 버퍼 내 시작 좌표 (보통 (0,0))
		&writeRegion // 출력할 콘솔 영역 (전체 화면)
	))
	{
	}
}

void Console::printDirtyBlock(std::size_t rowStart, std::size_t rowEnd,
							  const std::vector<CHAR_INFO>& buffer)
{
	std::span<const CHAR_INFO> bufferSpan(buffer.data() + rowStart * mWidth,
										  (rowEnd - rowStart) * mWidth);
	printBufferRect(bufferSpan,
					{.top = rowStart,
					 .left = 0,
					 .height = rowEnd - rowStart,
					 .width = mWidth
					});
}

void Console::printDirtyLine(std::size_t row, std::size_t start, std::size_t length,
					const std::vector<CHAR_INFO>& buffer)
{
	auto startIter = buffer.data() + row * mWidth + start;
	std::span<const CHAR_INFO> bufferSpan(startIter, startIter + length);
	printBufferRect(bufferSpan,
					{.top = row,
					 .left = start,
					 .height = 1,
					 .width = length
					});
}

void Console::printDirtyCellsInRow(std::size_t row,
								   const std::vector<uint8_t>::iterator currentRowIter,
								   const std::vector<CHAR_INFO>& buffer)
{
	auto startIndex = std::find(currentRowIter, currentRowIter + mWidth, true);
	while (startIndex < currentRowIter + mWidth)
	{
		auto endIndex = std::find(startIndex, currentRowIter + mWidth, false);
		const std::size_t startPosition = startIndex - currentRowIter;
		const std::size_t length = endIndex - startIndex;
		printDirtyLine(row, startPosition, length, buffer);
		startIndex = std::find(endIndex, currentRowIter + mWidth, true);
	}
}

void Console::GetScreenSize(int& rows, int& columns)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(mConsoleOutput, &bufferInfo))
	{
		rows = columns = 0;
		return;
	}
	columns = bufferInfo.dwSize.X;
	rows = bufferInfo.dwSize.Y;
}

void Console::ClearScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	DWORD cellCount;
	DWORD writtenCount;
	COORD startPosition = {0, 0};

	if (!GetConsoleScreenBufferInfo(mConsoleOutput, &bufferInfo))
	{
		return;
	}

	cellCount = bufferInfo.dwSize.X * bufferInfo.dwSize.Y;

	// 화면을 공백 문자로 채움
	FillConsoleOutputCharacterA(mConsoleOutput, ' ', cellCount,
								startPosition, &writtenCount);
	// 기존 색상 속성으로 채움
	FillConsoleOutputAttribute(mConsoleOutput, bufferInfo.wAttributes,
							   cellCount, startPosition, &writtenCount);
	// 커서를 좌상단으로 이동
	SetConsoleCursorPosition(mConsoleOutput, startPosition);
}

void Console::MoveCursor(int row, int column)
{
	COORD coord;
	coord.X = static_cast<SHORT>(column);
	coord.Y = static_cast<SHORT>(row);
	SetConsoleCursorPosition(mConsoleOutput, coord);
}

void Console::SetDefaultColor(Color color, Color backGroundColor)
{
	WORD colorWin32 = toColor32(color);
	WORD colorBackWin32 = toBackgroundColor32(backGroundColor);
	SetConsoleTextAttribute(mConsoleOutput, colorWin32 | colorBackWin32);
}

void Console::WriteString(std::string_view text)
{
	if (text.empty())
	{
		return;
	}

	int numCharacters = MultiByteToWideChar(CP_UTF8, 0, text.data(),
											static_cast<int>(text.size()), NULL, 0);
	if (numCharacters <= 0)
	{
		return;
	}

	std::wstring buffer;
	buffer.resize(numCharacters);

	MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()),
						buffer.data(), numCharacters);

	DWORD written;
	WriteConsoleW(mConsoleOutput, buffer.data(), numCharacters, &written, nullptr);
}

void Console::WriteColoredString(std::string_view text, Color color, Color backGroundColor)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(mConsoleOutput, &bufferInfo))
	{
		return;
	}

	WORD originalColor = bufferInfo.wAttributes;

	SetConsoleTextAttribute(mConsoleOutput,
							toColor32(color) | toBackgroundColor32(backGroundColor));
	WriteString(text);
	SetConsoleTextAttribute(mConsoleOutput, originalColor);
}

void Console::PrintDebugDirtyMap()
{
	int printRow = static_cast<int>(mHeight + 1);
	MoveCursor(printRow, 0);
	std::vector<char> buffer(mWidth);

	assert(mDirtyCells.size() % mWidth == 0 && "DirtyCells size mismatch");
	for (auto startIter = mDirtyCells.begin(); startIter < mDirtyCells.end();
		 startIter += mWidth)
	{
		auto endIter = startIter + mWidth;
		std::transform
		(startIter, endIter, buffer.begin(),
		 [](uint8_t isDirty) -> wchar_t
		 {
			 return (isDirty ? 'T' : 'F');
		 });
		WriteString(std::string(buffer.data(), 0, buffer.size()));
		MoveCursor(++printRow, 0);
	}

	std::vector<wchar_t> wBuffer(mWidth);
	printRow = static_cast<int>(2 * mHeight + 2);
	MoveCursor(printRow, 0);
	for (auto startIter = mBuffer.begin(); startIter < mBuffer.end();
		 startIter += mWidth)
	{
		auto endIter = startIter + mWidth;
		std::transform
		(startIter, endIter, wBuffer.begin(),
		 [](Cell cell) -> wchar_t
		 {
			 return cell.character;
		 });
		DWORD written;
		WriteConsoleW(mConsoleOutput, wBuffer.data(),
					  static_cast<DWORD>(wBuffer.size()),
					  &written, nullptr);
		MoveCursor(++printRow, 0);
	}
	MoveCursor(0, 0);
}

std::mutex& Console::GetMutex()
{
	return mMutex;
}

namespace
{
	WORD toColor32(console::eColor color)
	{
		switch (color)
		{
			case console::eColor::Black:
				return 0;
			case console::eColor::Red:
				return FOREGROUND_RED | FOREGROUND_INTENSITY;
			case console::eColor::Green:
				return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case console::eColor::Yellow:
				return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case console::eColor::Blue:
				return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case console::eColor::Magenta:
				return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case console::eColor::Cyan:
				return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case console::eColor::White:
				return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case console::eColor::Default:
			default:
				return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}

	WORD toBackgroundColor32(console::eColor color)
	{
		switch (color)
		{
			case console::eColor::Black:
				return 0;
			case console::eColor::Red:
				return BACKGROUND_RED | BACKGROUND_INTENSITY;
			case console::eColor::Green:
				return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			case console::eColor::Yellow:
				return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			case console::eColor::Blue:
				return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			case console::eColor::Magenta:
				return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			case console::eColor::Cyan:
				return BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			case console::eColor::White:
				return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			case console::eColor::Default:
			default:
				return 0;
		}
	}
}
#endif