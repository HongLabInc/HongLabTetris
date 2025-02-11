#pragma once

#include "ConsoleColor.h"
#include <vector>
#include <random>

class ColorManager
{
public:
    ColorManager();
    ~ColorManager() = default;

    void AddColorToTable(ConsoleColor color);
    void RemoveColorFromTable(ConsoleColor color);
    void ClearColors();
    void AddAllColors();
    void AddBrightColors();
    void AddDarkColors();

    ConsoleColor GetRandomColor() const;

    static WORD ToWord(ConsoleColor fg, ConsoleColor bg = ConsoleColor::Black);

private:
    std::vector<ConsoleColor> mColorTable;
    mutable std::mt19937 mRandomEngine;
};
