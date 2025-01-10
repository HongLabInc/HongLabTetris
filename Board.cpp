#include <cassert>
#include "Board.h"

Board::Board(int width, int height)
    : _width(width),
      _height(height)
{
    _board.resize(_width * _height, Cell::emptyCell);
    Init();
}

void Board::Init()
{

    // 왼쪽, 오른쪽 벽 설정
    for (int line = 0; line < _height; ++line)
    {
        SetCell(0, line, Cell::borderCell);  // 왼쪽 벽
        SetCell(_width - 1, line, Cell::borderCell);  // 오른쪽 벽
    }

    // 바닥 설정
    for(int i = 1; i < _width - 1; ++i)
    {
        SetCell(i, _height - 1, Cell::borderCell);
    }
}

void Board::Clear()
{
    std::fill(_board.begin(),_board.end(),Cell::emptyCell);
}

void Board::SetCell(int x, int y, const Cell& cell)
{
    assert(0 <= x && x < _width);
    assert(0 <= y && y < _height);

    _board[y * _width + x] = cell;
}

Cell Board::GetCell(int x, int y) const
{
    assert(0 <= x && x < _width);
    assert(0 <= y && y < _height);

    return _board[y * _width + x];
}

Cell * Board::GetCellPtr(int x, int y)
{
    assert(0 <= x && x < _width);
    assert(0 <= y && y < _height);

    return &_board[y * _width + x];
}

const Cell* Board::GetCellPtr(int x, int y) const
{
    assert(0 <= x && x < _width);
    assert(0 <= y && y < _height);

    return &_board[y * _width + x];
}

int Board::CheckFullLines()
{
    const int bottom = _height - 2;

    int count = 0;
	for(int line = bottom; line >= 0; --line)
    {
        // 한 줄이 Block으로 꽉찼다면 Count + 1
        if (isFullLine(line))
        {
            ++count;
            removeLine(line); // 한 줄을 지우고
            moveBlockDown(line); // y줄 위의 block을 한 칸씩 내리기
        }
    }

    return count;
}

bool Board::isFullLine(int line)
{
    for (int i = 1; i < _width - 1; ++i)
    {
        const Cell* cell = GetCellPtr(line, i);
        if (cell->GetType() != Cell::Type::Block)
            return false;
    }
    return true;
}

void Board::removeLine(int line)
{
    for(int i = 1; i < _width - 1; ++i)
    {
		SetCell(i, line, Cell::emptyCell);
    }
}

void Board::moveBlockDown(int bottomLine)
{
    // y번째 줄 위로 모두 한 칸씩 내리기
    for (int line = bottomLine; line > 0; --line)
    {
        for (int i = 1; i < _width - 1; ++i)
        {
            // 윗줄의 상태를 현재 줄로 복사
			const Cell* cell = GetCellPtr(i, line - 1);
            SetCell(i, line, *cell);
        }
    }

    // 최상단 줄은 비움
    for (int i = 1; i < _width - 1; ++i)
    {
        SetCell(i, 0, Cell::emptyCell);
    }
}
