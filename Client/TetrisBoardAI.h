#pragma once
#include <queue>
#include <vector>
#include <memory>
#include "TetrisBoard.h"

class TetrisBoardAI: public TetrisBoard
{
public:
	enum class Level
	{
		Easy,			// 쉬움
		Normal,			// 보통
		Hard			// 어려움
	};

	TetrisBoardAI(ConsoleRenderer& r, int x, int y, int w, int h,
				  InputManager* im, EventManager* em = nullptr)
		: TetrisBoard(r, x, y, w, h, im, em)
	{
		RebuildDelays();
	}

	void SetLevel(Level s)
	{
		mSkill = s; RebuildDelays();
	}

	void HandleInput() override;

private:
	enum class Action
	{
		RotateCW, MoveLeft, MoveRight, HardDrop
	};

	struct Eval
	{
		int targetX = 0; int rotateCW = 0; int score = 0;
	};

	struct DelayRanges
	{
		int rotMin = 12, rotMax = 20;  
		int moveMin = 6, moveMax = 12;
		int dropMin = 0, dropMax = 4;
		int postPlan = 10;
		int preDrop = 18;
	} mDelay;

	std::queue<Action> mPlan;
	int  mCurrentActionDelay = 0; 
	bool mJustPlanned = false;
	int  mPostPlanDelayFrames = 10; 
	int  mPostPlanCounter = 0;
	int  mPreHardDropDelayFrames = 18;
	int  mPreHardDropCounter = 0;
	Level mSkill = Level::Easy;

private:
	void RebuildDelays();

	bool MakePlan();
	Eval FindBestPlacement();
	int  EvaluatePlacement(std::unique_ptr<Block>& tmp,
						   const std::vector<std::vector<bool>>& boardSnapshot);

	void SimulateHardDrop(std::unique_ptr<Block>& tmp);
};
