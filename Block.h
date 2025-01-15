#pragma once

#include <random>
#include "InputManager.h"
#include "ModelPointer.h"

extern mat4x4 blockModel[28];


class Block {

	ShapeType type;          
	RotationState rotation; 
	RotationState prevRotate;


	const ModelPointer* currentShape;
	
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


	const ModelPointer* GetShapeMatrix() const;
	int GetMatrixSize() const;
	char GetValue(int i,int j);

};

int SetRandNum();

