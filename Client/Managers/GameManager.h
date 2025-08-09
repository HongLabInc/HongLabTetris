#pragma once

#include <memory>
#include <vector>

class TetrisBoard;
class ConsoleRenderer;
class SceneManager;
class InputManager;
enum class GameModeType;

enum class EInGameState
{
	Playing,
	Pause,
	GameOver,
};

// GameManager: 현재 진행중인 게임의 설정값, 스코어, 난이도 등을 담당
class GameManager final
{
protected:
	GameManager(GameManager&) = delete;
	void operator=(GameManager&) = delete;

public:
	GameManager(ConsoleRenderer& renderer, SceneManager* sceneMgr, InputManager* inputMgr);
	~GameManager() = default;

	void Start(GameModeType mode);	// 게임 시작
	void Exit();					// 게임 종료

	// 현재 게임 업데이트/렌더링
	void Update(float dt);
	void Draw();

	void OnUpdate(float dt);
	void OnPause();
	void OnGameOver();

protected:
	void UpdateBoards(float deltaTime);
	void DrawBoards();

	void SetupSingleMode();
	void SetupMultiplayerMode();

private:
	ConsoleRenderer& mRenderer;
	SceneManager* mSceneManager;
	InputManager* mInputManager;

	std::vector<std::shared_ptr<TetrisBoard>> mBoards;

	float mTimeScale;
	EInGameState mInGameState;
};