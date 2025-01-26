#include "Engine.h"
#include "ConsoleRenderer.h"

#include <iostream>
#include <windows.h>

int main()
{
    ConsoleRenderer renderer(300, 200, 240.0f); // 가로, 세로, 주사율
    Engine engine(renderer);
    engine.Initailize();
    engine.Run();

    return 0;
}
