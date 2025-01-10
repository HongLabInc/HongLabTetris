#include "Engine.h"
#include "GameMode.h"
#include "ConsoleRenderer.h"

#include <iostream>
#include <windows.h>

int main()
{
    ConsoleRenderer renderer(60, 40); // 가로, 세로
    Engine engine(renderer);
    engine.Run();

    return 0;
}
