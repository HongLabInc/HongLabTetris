#include "GameManager.h"
#include "TetrisBoard.h"
#include "TetrisBoardAI.h"
#include "Managers/SceneManager.h"

GameManager::GameManager(ConsoleRenderer& renderer, SceneManager* sceneMgr, InputManager* inputMgr)
	: mRenderer(renderer)
	, mSceneManager(sceneMgr)
	, mInputManager(inputMgr)
	, mBoards()
	, mTimeScale(1.0f)
	, mInGameState(EInGameState::Playing)
{
	// Empty
}

void GameManager::Start(GameModeType mode)
{
	switch (mode)
	{
		case GameModeType::Single:
			SetupSingleMode();
			break;
		case GameModeType::Multiplayer:
			SetupMultiplayerMode();
	}
}

void GameManager::Exit()
{
	mSceneManager->RequestSceneChange(SceneType::MainMenu);
}

void GameManager::Update(float dt)
{
	switch (mInGameState)
	{
		case EInGameState::Playing:
			OnUpdate(dt);
			break;
		case EInGameState::Pause:
			OnPause();
			break;
		case EInGameState::GameOver:
			OnGameOver();
			break;
	}
}

void GameManager::Draw()
{
	DrawBoards();
}

void GameManager::OnUpdate(float dt)
{
	UpdateBoards(dt);
}

void GameManager::OnPause()
{
	// mTimeScale = 0.0f;
}

void GameManager::OnGameOver()
{
	Exit();
}

void GameManager::UpdateBoards(float dt)
{
	// 보드 업데이트
	for (auto& board : mBoards)
	{
		board->Update(dt);
		if (board->IsFull())
		{
			mInGameState = EInGameState::GameOver;
		}
	}
}

void GameManager::DrawBoards()
{
	for (auto& board : mBoards)
	{
		board->Draw();
	}
}

void GameManager::SetupSingleMode()
{
	mBoards.emplace_back(std::make_shared<TetrisBoard>(mRenderer, 3, 3, 12, 24, mInputManager));
	mBoards.emplace_back(std::make_shared<TetrisBoardAI>(mRenderer, 30, 3, 12, 24, mInputManager));
}

void GameManager::SetupMultiplayerMode()
{
	// Empty
}
