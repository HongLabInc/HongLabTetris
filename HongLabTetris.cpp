#include "Engine.h"
#include "GameMode.h"
#include "ConsoleRenderer.h"

#include <iostream>
#include <windows.h>

int main()
{
    ConsoleRenderer renderer(60, 40, 240.0f); // 가로, 세로, 주사율
    Engine engine(renderer);
    engine.Initailize();
    engine.Run();

    return 0;
}
