#pragma once

#include "SceneType.h"
#include "../ConsoleFrame.h"
#include "../ConsoleRenderer.h"
#include "../Managers/SceneManager.h"

class InputManager;
class UIManager;
class SceneManager;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    SceneType GetPendingSceneChange() const;

    void ClearPendingSceneChange();

protected:
    // 필요하다면 protected 생성자로 제한
    Scene(
        ConsoleRenderer& renderer, 
        InputManager* inputManager,
        UIManager* uiManager,
        SceneManager* sceneManager
    );

    ConsoleRenderer&   mRenderer;
	ConsoleFrame*      mFrame;
    InputManager*      mInputManager;
    UIManager*         mUIManager;
    SceneManager*      mSceneManager;
    SceneType          mPendingSceneChange;
};