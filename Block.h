#pragma once

#include <random>
#include "InputManager.h"


typedef char mat4x4[4][4];

extern mat4x4 blockModel[28];


class Block {
	mat4x4 block;
	
	int x,y;


public:
	Block(int posX, int posY);
	~Block() = default;

	void Initalize();
	void Update(InputManager* InputMgr);
	
	int GetX ();
	int GetY ();

	void MoveBlockLeft();
	void MoveBlockRight();
	void MoveBlockDown();
	
	void Rotate();

	const mat4x4& GetShape() const;
};