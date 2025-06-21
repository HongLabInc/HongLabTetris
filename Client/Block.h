#pragma once

#include <random>
#include "Managers/ConsoleColor.h"
#include "ModelPointer.h"

class Block {

	ShapeType type;          
	RotationState rotation; 
	RotationState prevRotate;

	ConsoleColor texture;
	const ModelPointer* currentShape;
	
	int prevX,prevY;
	int x,y;


public:
	Block(int posX = 0, int posY = 0, ConsoleColor bTexture = ConsoleColor::Black);
	~Block() = default;

	void Initalize();
	void CopyFrom(const Block& other);
	void Update();
	
	

	void MoveLeft();
	void MoveRight();
	void MoveDown();


	void Rotate();
	void RotateCounterClockwise();
	void rollback();

	void UpdatePos();
	
	void SetPosition(int newX, int newY);
	ShapeType GetType() const { return type; }
	RotationState GetRotation() const { return rotation; }
	RotationState GetNextRotation(bool clockwise = true) const;


#pragma region Getter
	const ModelPointer* GetShapeMatrix() const;
	int GetMatrixSize() const;
	char GetValue(int i,int j);

	void SetX(int row);
	void SetY(int column);
	int GetX () const;
	int GetY () const;

	ConsoleColor GetTexture() const;
#pragma endregion

	void SetTexture(ConsoleColor tex);


};

int SetRandNum();

