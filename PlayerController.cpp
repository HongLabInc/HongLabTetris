#include "PlayerController.h"
#include <Windows.h>
#include <iostream>

PlayerController::PlayerController(GameMode* gm)
{
    gameMode = gm;
}

void PlayerController::ProcessInput() {
    if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
        gameMode->MoveBlockLeft();
    }
    else if(GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        gameMode->MoveBlockRight();
    }
    else if(GetAsyncKeyState(VK_SPACE) & 0x8000) {
        gameMode->MoveBlockDown();
    }
}