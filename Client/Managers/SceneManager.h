#pragma once

#include <memory>
#include "../Scenes/Scene.h"
#include "../Scenes/SceneType.h"
#include "../ConsoleRenderer.h"
#include "InputManager.h"
#include "UIManager.h"

class Scene;

// SceneManager: 현재 Scene을 보유하며, Scene 전환, 업데이트, 렌더링을 담당
class SceneManager {
public:
    SceneManager(
        ConsoleRenderer& renderer,
        InputManager* inputManager,
        UIManager* uiManager
    );
    ~SceneManager() = default;

    // Scene 전환 요청
    void RequestSceneChange(SceneType type);

    // 현재 Scene 업데이트/렌더링
    void Update();
    void Draw();

private:
    ConsoleRenderer&     mRenderer;
    InputManager*        mInputManager;
    UIManager*           mUIManager;
    std::unique_ptr<Scene> mCurrentScene;  // 현재 활성화된 Scene
};