#include "ColorManager.h"

ColorManager::ColorManager()
    : mRandomEngine(std::random_device{}())
{
}

void ColorManager::AddColorToTable(ConsoleColor color)
{
    auto it = std::find(mColorTable.begin(), mColorTable.end(), color);

    if (it == mColorTable.end())
    {
        mColorTable.push_back(color);
    }
}

void ColorManager::RemoveColorFromTable(ConsoleColor color)
{
    auto it = std::find(mColorTable.begin(), mColorTable.end(), color);

    if (it != mColorTable.end())
    {
        mColorTable.erase(it);
    }
}

void ColorManager::ClearColors()
{
    mColorTable.clear();
}

void ColorManager::AddAllColors()
{
    for (int i = 0; i <= static_cast<int>(ConsoleColor::BrightWhite); ++i)
    {
        AddColorToTable(static_cast<ConsoleColor>(i));
    }
}

void ColorManager::AddBrightColors()
{
    AddColorToTable(ConsoleColor::BrightBlue);
    AddColorToTable(ConsoleColor::BrightGreen);
    AddColorToTable(ConsoleColor::BrightRed);
    AddColorToTable(ConsoleColor::BrightCyan);
    AddColorToTable(ConsoleColor::BrightMagenta);
    AddColorToTable(ConsoleColor::BrightYellow);
    AddColorToTable(ConsoleColor::BrightWhite);
}

void ColorManager::AddDarkColors()
{
    AddColorToTable(ConsoleColor::Black);
    AddColorToTable(ConsoleColor::Blue);
    AddColorToTable(ConsoleColor::Green);
    AddColorToTable(ConsoleColor::Red);
    AddColorToTable(ConsoleColor::Cyan);
    AddColorToTable(ConsoleColor::Magenta);
    AddColorToTable(ConsoleColor::Yellow);
    AddColorToTable(ConsoleColor::White);
}

ConsoleColor ColorManager::GetRandomColor() const
{
    if (mColorTable.empty())
    {
        return ConsoleColor::BrightWhite;
    }
    std::uniform_int_distribution<size_t> dist(0, mColorTable.size() - 1);
    return mColorTable[dist(mRandomEngine)];
}

WORD ColorManager::ToWord(ConsoleColor fg, ConsoleColor bg)
{
    return static_cast<WORD>(fg) | (static_cast<WORD>(bg) << 4);
}
