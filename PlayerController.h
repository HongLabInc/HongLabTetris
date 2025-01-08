#pragma once
#include "GameMode.h"

class PlayerController {
public:
    explicit PlayerController(GameMode* gm);
    ~PlayerController();

    void ProcessInput();   // 입력 처리

private:
    GameMode* gameMode;
};