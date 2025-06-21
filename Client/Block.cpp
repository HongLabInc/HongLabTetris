#include "Block.h"
#include <iostream>

Block::Block(int posX, int posY, ConsoleColor bTexture)
	: x(posX)
	, y(posY)
	, texture(bTexture)
{
}

int SetRandNum(int size) {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distr(0, size - 1);

	return distr(gen);
}

void Block::Initalize() {
	type = static_cast<ShapeType>(SetRandNum(SHAPE_COUNT));
	rotation = ROTATION_0;
	currentShape = &modelPointers[type][0];
}

void Block::CopyFrom(const Block& other)
{
	type      = other.type;
	rotation  = other.rotation;
	prevRotate= other.prevRotate;
	currentShape = other.currentShape;

	prevX     = other.prevX;
	prevY     = other.prevY;
	x         = other.x;
	y         = other.y;

	this->texture   = other.texture;
}

void Block::Update()
{
	y++;
}

void Block::MoveLeft()
{
	x--;
}
void Block::MoveRight()
{
	x++;
}
void Block::MoveDown()
{
	y ++;
}


void Block::Rotate() {

	rotation = static_cast<RotationState>((rotation + 1) % ROTATION_COUNT);
}

void Block::RotateCounterClockwise() {
	rotation = static_cast<RotationState>((rotation + 3) % ROTATION_COUNT);
}

void Block::rollback()
{
	rotation = prevRotate;
	x = prevX;
	y = prevY;
}

void Block::UpdatePos() {

	prevRotate = rotation;
	prevX = x;
	prevY = y;
}

void Block::SetPosition(int newX, int newY) {
	x = newX;
	y = newY;
}

RotationState Block::GetNextRotation(bool clockwise) const {
	if (clockwise) {
		return static_cast<RotationState>((rotation + 1) % ROTATION_COUNT);
	} else {
		return static_cast<RotationState>((rotation + 3) % ROTATION_COUNT);
	}
}

void Block::SetTexture(ConsoleColor tex) {
	texture = tex;
}


#pragma region Getter
const ModelPointer* Block::GetShapeMatrix() const {
	
	return currentShape;
}

int Block::GetMatrixSize() const
{
	if(type == SHAPE_O)
		return 2;
	 else if(type == SHAPE_I) 
		return 4;
	return 3;
}

char Block::GetValue(int i,int j) 
{
	if(type == SHAPE_O) {
		return (*currentShape[rotation].mat2)[i][j]; // mat2x2 접근
	} else if(type == SHAPE_I) {
		return (*currentShape[rotation].mat4)[i][j]; // mat4x4 접근
	} else {
		return (*currentShape[rotation].mat3)[i][j]; // mat3x3 접근
	}
}

void Block::SetX(int row)
{
	x = row;
}

void Block::SetY(int column)
{
	y = column;
}

int Block::GetX() const
{
	return x;
}
int Block::GetY() const
{
	return y;
}
ConsoleColor Block::GetTexture() const
{
	return texture;
}
#pragma endregion



