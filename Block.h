#pragma once

#include <random>
#include "ConsoleColor.h"
#include "ModelPointer.h"

extern mat4x4 blockModel[28];


class Block {

	ShapeType type;          
	RotationState rotation; 
	RotationState prevRotate;

	ConsoleColor texture;
	const ModelPointer* currentShape;
	
	int prevX,prevY;
	int x,y;


public:
	Block(int posX, int posY, ConsoleColor texture);
	~Block() = default;

	void Initalize();
	void Update();
	
	

	void MoveLeft();
	void MoveRight();
	void MoveDown();


	void Rotate();
	void rollback();

	void UpdatePos();


#pragma region Getter
	const ModelPointer* GetShapeMatrix() const;
	int GetMatrixSize() const;
	char GetValue(int i,int j);

	int GetX ();
	int GetY ();

	ConsoleColor GetTexture();
#pragma endregion


};

int SetRandNum();

