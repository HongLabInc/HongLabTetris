#pragma once

#include <Windows.h>
#include "Managers/ConsoleColor.h"

class Cell
{
public:
	enum class Type
	{
		None = -1,
		Empty,
		Border,
		Block
	};

	Cell() = default;
	Cell(Type type);
	Cell(Type type, WCHAR c);
	Cell(Type type, WCHAR c, WORD colorAttr);
	Cell(WORD colorAttr);
	~Cell() = default;

	CHAR_INFO ToCharInfo() const; // WinAPI 호출에 필요한 CHAR_INFO로 변환

	Type GetType() const;
	void SetType(Type type);

	WCHAR GetChar() const;
	void  SetChar(WCHAR c);

	WORD GetAttributes() const;
	void SetAttributes(WORD ColorAttr);
	void SetForegroundColor(ConsoleColor color);
	void SetBackgroundColor(ConsoleColor color);

	static const Cell emptyCell;
	static const Cell borderCell;
	static const Cell blockCell;

	static constexpr WCHAR CHAR_EMPTY = L' '; // 공백
	static constexpr WCHAR CHAR_BLOCK = L' '; // 현재는 BLOCK도 공백으로 사용중

private:
	Type  mType = Type::Block; //셀 타입(상태)
	WCHAR mChar = CHAR_BLOCK; // 표시문자
	WORD  mColorAttr = static_cast<WORD>(ConsoleColor::BrightWhite) << 4; // 셀 색상
};
