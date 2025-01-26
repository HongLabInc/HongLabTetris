#include "MainMenuScene.h"
#include "SceneManager.h" 
#include <iostream>

 MainMenuScene::MainMenuScene(ConsoleRenderer & renderer, InputManager * inputManager, SceneManager* sceneManager)
    : Scene(renderer, inputManager, sceneManager) {
    
     mRenderer.Clear();
     mFrame = mRenderer.AddFrame(10, 5, 40, 30);
	 mFrame->Clear();
     mFrame->SetText(10, 5, L"Welcome to Tetris!", static_cast<WORD>(ConsoleColor::BrightGreen));
     mFrame->SetText(10, 6, L"Press Enter to Start", static_cast<WORD>(ConsoleColor::BrightWhite));
}

void MainMenuScene::Update() {
    // Enter 키 입력 시 상태 전환
    int key = mInputManager->DequeueInput();
    if (key == VK_RETURN) {
        mSceneManager->RequestSceneChange(SceneType::Playing);
    }
}

void MainMenuScene::Draw() {
    // 메인 메뉴 화면 텍스트
    mRenderer.Render();
}
