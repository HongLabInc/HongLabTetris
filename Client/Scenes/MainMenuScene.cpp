#include "MainMenuScene.h"
#include <iostream>

void MainMenuScene::InitializeButtons()
{
    int screenWidth = mFrame->GetWidth();
    int screenHeight = mFrame->GetHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // Single 버튼 (위쪽)
    mSingleMode->SetPosition(centerX, centerY - 3);
    mSingleMode->resize(10, 3);
    mSingleMode->setText(L"Single");
    

    // Multi 버튼 (아래쪽)
    mMultiMode->SetPosition(centerX, centerY + 3);  // 중앙 기준
    mMultiMode->resize(10, 3);  // 여백 포함 크기
    mMultiMode->setText(L"Multi");

    // 싱글 모드 버튼 이벤트 설정
    mSingleMode->SetOnHoverEnter([this]() {
        // 호버 효과 (선택적)
    });

    mSingleMode->SetOnClick([this]() {
        mPendingSceneChange = SceneType::Playing;
    });


    // 멀티 모드 버튼 이벤트 설정
    mMultiMode->SetOnHoverEnter([this]() {
        // 호버 효과 (선택적)
    });

    mMultiMode->SetOnClick([this]() {
        mPendingSceneChange = SceneType::Playing;
    });


    
}

MainMenuScene::MainMenuScene(
    ConsoleRenderer& renderer,
    InputManager* inputManager,
    UIManager* uiManager,
    SceneManager* sceneManager)
    : Scene(renderer, inputManager, uiManager, sceneManager)
{
    mRenderer.Clear();
    mFrame = mRenderer.AddFrame(0, 0, 40, 30);
    mFrame->Clear();

    try {
        mSingleMode = std::make_unique<Button>(inputManager, mFrame);
        mMultiMode = std::make_unique<Button>(inputManager, mFrame);
        InitializeButtons();
    } catch (const std::exception& e) {
        std::cerr << "Error initializing buttons: " << e.what() << std::endl;
        // 적절한 에러 처리
    }

    mFrame->SetText(5, 5, L"Welcome to Tetris!", static_cast<WORD>(ConsoleColor::BrightGreen));
    mFrame->SetText(5, 6, L"Press Enter to Start", static_cast<WORD>(ConsoleColor::BrightWhite));
}

void MainMenuScene::Update(float deltaTime) {

    // Enter 키 입력 처리
    int key = mInputManager->DequeueInput();
    if (key == VK_RETURN) {
        mPendingSceneChange = SceneType::Playing;
    }

    // 버튼들의 Update 호출 추가
    mSingleMode->Update();
    mMultiMode->Update();
}

void MainMenuScene::Draw() {
    mSingleMode->draw();
    mMultiMode->draw();
    mRenderer.Render();
}

   