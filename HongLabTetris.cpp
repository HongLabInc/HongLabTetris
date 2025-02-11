#include "Engine.h"
#include "ConsoleRenderer.h"

#include <iostream>
#include <windows.h>

int main()
{
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow) {
        RECT windowRect;
        if (GetWindowRect(consoleWindow, &windowRect)) {
            std::wcout << L"Console Window Position: ("
                << windowRect.left << L", "
                << windowRect.top << L")" << std::endl;
        }
    }

    ConsoleRenderer renderer(1280, 720, 240.0f); // 가로, 세로, 주사율
    Engine engine(renderer);
    engine.Initailize();
    engine.Run();

    return 0;
}
