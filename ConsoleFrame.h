#pragma once
#include <vector>
#include <string>
#include "Cell.h"

class ConsoleRenderer;

class ConsoleFrame
{
public:
    ConsoleFrame(int x, int y, int width, int height);
    ~ConsoleFrame() = default;

    void Clear();
    const Cell& GetCell(int x, int y) const;
    void SetCell(int x, int y, const Cell& cell);

	void DrawRectangle(int x, int y, int width, int height, const Cell& borderCell);
	void FillRectangle(int x, int y, int width, int height, const Cell& fillCell);
    void SetText(int x, int y, const std::wstring& text,
		WORD CollorAttr = static_cast<WORD>(ConsoleColor::BrightWhite));

    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;

protected:
    int mX;      // 콘솔 상에서 Frame의 시작 X 좌표
    int mY;      // 콘솔 상에서 Frame의 시작 Y 좌표
    int mWidth;  // Frame 너비
    int mHeight; // Frame 높이
    std::vector<Cell> mCells; // Frame 내부의 Cell 배열
};
