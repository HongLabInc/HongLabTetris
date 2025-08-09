#include "TetrisBoardAI.h"
#include <algorithm>
#include <limits>
#include <random>

static inline int RandomInRange(int min, int max)
{
	static std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}

static inline void PushKey(InputManager* im, int vk)
{
	if (im) im->EnqueueInput(vk);
}

static constexpr int BASE_ROT_MIN = 12, BASE_ROT_MAX = 20;
static constexpr int BASE_MOV_MIN = 6, BASE_MOV_MAX = 12;
static constexpr int BASE_DROP_MIN = 0, BASE_DROP_MAX = 4;
static constexpr int BASE_POSTPLAN = 10;
static constexpr int BASE_PREDROP = 18;

void TetrisBoardAI::RebuildDelays()
{
	double mul = 1.0;
	switch (mSkill)
	{
		case Level::Easy:       
			mul = 2.3;  
			break;
		case Level::Normal: 
			mul = 2.0;  
			break;
		case Level::Hard:       
			mul = 1.5; 
			break;
	}

	auto scale = [mul](int v)
	{
		return static_cast<int>(std::lround(v * mul));
	};

	mDelay.rotMin = scale(BASE_ROT_MIN);
	mDelay.rotMax = scale(BASE_ROT_MAX);
	mDelay.moveMin = scale(BASE_MOV_MIN);
	mDelay.moveMax = scale(BASE_MOV_MAX);
	mDelay.dropMin = scale(BASE_DROP_MIN);
	mDelay.dropMax = scale(BASE_DROP_MAX);
	mDelay.postPlan = scale(BASE_POSTPLAN);
	mDelay.preDrop = scale(BASE_PREDROP);

	mPostPlanDelayFrames = mDelay.postPlan;
	mPreHardDropDelayFrames = mDelay.preDrop;
}

void TetrisBoardAI::HandleInput()
{
	if (!mIsBlockActive)
	{
		TetrisBoard::HandleInput();
		return;
	}

	if (mPlan.empty())
	{
		MakePlan();
		mJustPlanned = true;
		mPostPlanCounter = mPostPlanDelayFrames;
		mPreHardDropCounter = 0;
	}

	if (mJustPlanned)
	{
		if (mPostPlanCounter-- > 0)
		{
			TetrisBoard::HandleInput();
			return;
		}
		mJustPlanned = false;
	}

	if (mCurrentActionDelay > 0)
	{
		--mCurrentActionDelay;
		TetrisBoard::HandleInput();
		return;
	}

	if (!mPlan.empty())
	{
		Action a = mPlan.front();

		if (a == Action::HardDrop)
		{
			if (mPreHardDropCounter < mPreHardDropDelayFrames)
			{
				++mPreHardDropCounter;
				TetrisBoard::HandleInput();
				return;
			}
		}

		mPlan.pop();

		switch (a)
		{
			case Action::RotateCW:
				mCurrentActionDelay = RandomInRange(mDelay.rotMin, mDelay.rotMax);
				PushKey(mInputManager, VK_UP);
				break;

			case Action::MoveLeft:
				mCurrentActionDelay = RandomInRange(mDelay.moveMin, mDelay.moveMax);
				PushKey(mInputManager, VK_LEFT);
				break;

			case Action::MoveRight:
				mCurrentActionDelay = RandomInRange(mDelay.moveMin, mDelay.moveMax);
				PushKey(mInputManager, VK_RIGHT);
				break;

			case Action::HardDrop:
				mCurrentActionDelay = RandomInRange(mDelay.dropMin, mDelay.dropMax);
				PushKey(mInputManager, VK_SPACE);
				break;
		}
	}

	TetrisBoard::HandleInput();
}

bool TetrisBoardAI::MakePlan()
{
	if (!mCurrentBlock) return false;

	Eval best = FindBestPlacement();

	for (int i = 0; i < best.rotateCW; ++i)
		mPlan.push(Action::RotateCW);

	int curX = mCurrentBlock->GetX();
	if (best.targetX < curX)
	{
		for (int i = 0; i < (curX - best.targetX); ++i) mPlan.push(Action::MoveLeft);
	}
	else if (best.targetX > curX)
	{
		for (int i = 0; i < (best.targetX - curX); ++i) mPlan.push(Action::MoveRight);
	}

	mPlan.push(Action::HardDrop);
	return true;
}

TetrisBoardAI::Eval TetrisBoardAI::FindBestPlacement()
{
	auto board = isFilled;

	Eval best;
	best.targetX = mCurrentBlock ? mCurrentBlock->GetX() : 0;
	best.rotateCW = 0;
	best.score = std::numeric_limits<int>::lowest();

	for (int rot = 0; rot < 4; ++rot)
	{
		auto tmp = std::make_unique<Block>(0, 0, mCurrentBlock->GetTexture());
		tmp->CopyFrom(*mCurrentBlock);
		for (int i = 0; i < rot; ++i) tmp->Rotate();

		const int size = tmp->GetMatrixSize();
		int minX = 1 - size; 
		int maxX = mWidth - 2; 

		for (int x = minX; x <= maxX; ++x)
		{
			auto t = std::make_unique<Block>(0, 0, tmp->GetTexture());
			t->CopyFrom(*tmp);
			t->SetPosition(x, 0);

			if (CheckCollision(t)) continue;

			SimulateHardDrop(t);

			int sc = EvaluatePlacement(t, board);
			if (sc > best.score)
			{
				best.score = sc;
				best.targetX = x;
				best.rotateCW = rot;
			}
		}
	}
	return best;
}

int TetrisBoardAI::EvaluatePlacement(std::unique_ptr<Block>& tmp,
									 const std::vector<std::vector<bool>>& boardSnapshot)
{
	auto grid = boardSnapshot;

	int wx = tmp->GetX();
	int wy = tmp->GetY();
	int sz = tmp->GetMatrixSize();

	for (int i = 0; i < sz; ++i)
	{
		for (int j = 0; j < sz; ++j)
		{
			if (tmp->GetValue(i, j) != 0)
			{
				int gx = wx + j;
				int gy = wy + i;
				if (gx >= 0 && gx < mWidth && gy >= 0 && gy < mHeight)
				{
					grid[gy][gx] = true;
				}
			}
		}
	}

	int fullLines = 0;
	for (int r = 1; r < mHeight - 1; ++r)
	{
		int filled = 0;
		for (int c = 1; c < mWidth - 1; ++c)
			filled += grid[r][c] ? 1 : 0;
		if (filled == (mWidth - 2)) fullLines++;
	}

	int holes = 0;
	for (int c = 1; c < mWidth - 1; ++c)
	{
		bool seenBlock = false;
		for (int r = 1; r < mHeight - 1; ++r)
		{
			if (grid[r][c]) seenBlock = true;
			else if (seenBlock && !grid[r][c]) holes++;
		}
	}

	std::vector<int> heights(mWidth, mHeight);
	for (int c = 1; c < mWidth - 1; ++c)
	{
		int h = 0;
		for (int r = 1; r < mHeight - 1; ++r)
		{
			if (grid[r][c])
			{
				h = mHeight - r; break;
			}
		}
		heights[c] = h;
	}
	int bump = 0, sumH = 0;
	for (int c = 1; c < mWidth - 2; ++c)
	{
		bump += std::abs(heights[c] - heights[c + 1]);
		sumH += heights[c];
	}

	int score = 0;
	score += fullLines * 1000;
	score -= holes * 50;
	score -= bump * 3;
	score -= sumH * 2;
	score += wy * 1;

	return score;
}

void TetrisBoardAI::SimulateHardDrop(std::unique_ptr<Block>& tmp)
{
	while (!CheckCollision(tmp))
	{
		tmp->UpdatePos();
		tmp->MoveDown();
	}
	tmp->rollback();
}
