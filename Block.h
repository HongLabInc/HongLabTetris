#pragma once

#include <random>
#include "InputManager.h"


typedef char mat4x4[4][4];

extern mat4x4 blockModel[28];


class Block {
	mat4x4 prevShape;
	mat4x4 block;
	
	
	int prevX,prevY;
	int x,y;


public:
	Block(int posX, int posY);
	~Block() = default;

	void Initalize();
	void Update();
	
	int GetX ();
	int GetY ();

	void MoveLeft();
	void MoveRight();
	void MoveDown();



	void Rotate();
	void rollback();

	void UpdatePos();

	const mat4x4& GetShape() const;
};