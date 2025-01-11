#include "Cell.h"

const Cell Cell::emptyCell(Cell::Type::Empty, Cell::CHAR_EMPTY, static_cast<WORD>(ConsoleColor::Black));
const Cell Cell::borderCell(Cell::Type::Border);
const Cell Cell::blockCell(Cell::Type::Block);

Cell::Cell(Type type)
	: mType(type) {}

Cell::Cell(Type type, WCHAR c)
	: mType(type)
	, mChar(c) {}

Cell::Cell(Type type, WCHAR c, WORD colorAttr)
	: mType(type)
	, mChar(c)
	, mColorAttr(colorAttr) {}

Cell::Cell(WORD colorAttr)
	: mColorAttr(colorAttr) {}

CHAR_INFO Cell::ToCharInfo() const
{
	CHAR_INFO info = {};

	info.Char.UnicodeChar = mChar;
	info.Attributes = mColorAttr;

	return info;
}


Cell::Type Cell::GetType() const
{
	return mType;
}

void Cell::SetType(Type type)
{
	mType = type;
}

WCHAR Cell::GetChar() const
{
	return mChar;
}

void Cell::SetChar(WCHAR c)
{
	mChar = c;
}

WORD Cell::GetAttributes() const
{
	return mColorAttr;
}

void Cell::SetAttributes(WORD ColorAttr)
{
	mColorAttr = ColorAttr;
}
