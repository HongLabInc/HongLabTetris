#pragma once

class SceneManager;

#include "ConsoleFrame.h"
#include "ConsoleRenderer.h"
#include "InputManager.h"


class Scene {
public:
    virtual ~Scene() = default;

    virtual void Update() = 0;
    virtual void Draw() = 0;

protected:
    // 필요하다면 protected 생성자로 제한
    Scene(ConsoleRenderer& renderer, InputManager* inputManager, SceneManager* sceneManager);
    ConsoleRenderer&   mRenderer;
	ConsoleFrame*      mFrame;
    InputManager*      mInputManager;
    SceneManager*      mSceneManager;
};