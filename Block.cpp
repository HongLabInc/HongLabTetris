#include "Block.h"
#include <iostream>

mat4x4 blockModel[28] =
{
	/* ㅁ
	   ㅁㅁㅁ  */
	{
		{0,0,0,0},
		{1,0,0,0},
		{1,1,1,0},
		{0,0,0,0}
	},
	{
		{0,1,0,0},
	{0,1,0,0},
	{1,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,1,0},
	{0,0,1,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{1,0,0,0}
	},

	/*     ㅁ
		ㅁㅁㅁ  */
	{
		{0,0,0,0},
	{0,0,1,0},
	{1,1,1,0},
	{0,0,0,0}
	},
	{
		{1,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,1,0},
	{1,0,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,0,0,0},
	{1,0,0,0},
	{1,1,0,0}
	},

	/*   ㅁ
	   ㅁㅁㅁ */
	{
		{0,0,0,0},
	{0,1,0,0},
	{1,1,1,0},
	{0,0,0,0}
	},
	{
		{0,1,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,1,0},
	{0,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,0,0,0},
	{1,1,0,0},
	{1,0,0,0}
	},

	/* ㅁ
	   ㅁ
	   ㅁ
	   ㅁ */
	{
		{1,0,0,0},
	{1,0,0,0},
	{1,0,0,0},
	{1,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}
	},
	{
		{1,0,0,0},
	{1,0,0,0},
	{1,0,0,0},
	{1,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}
	},

	/* ㅁㅁ
	   ㅁㅁ */
	{
		{0,0,0,0},
	{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0}
	},

	/*   ㅁㅁ
	   ㅁㅁ   */
	{
		{0,0,0,0},
	{0,1,1,0},
	{1,1,0,0},
	{0,0,0,0}
	},
	{
		{1,0,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{0,1,1,0},
	{1,1,0,0},
	{0,0,0,0}
	},
	{
		{1,0,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}
	},

	/* ㅁㅁ
		 ㅁㅁ */
	{
		{0,0,0,0},
	{1,1,0,0},
	{0,1,1,0},
	{0,0,0,0}
	},
	{
		{0,1,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{0,0,0,0}
	},
	{
		{0,0,0,0},
	{1,1,0,0},
	{0,1,1,0},
	{0,0,0,0}
	},
	{
		{0,1,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{0,0,0,0}
	}
};

Block::Block(int posX = 0, int posY = 0):
	x(posX), y(posY){
	memset(block,0,sizeof(block));
}

void Block::Initalize() {

	std::random_device rd;
	std::mt19937 gen(rd());

	int totalShapes = sizeof(blockModel) / sizeof(mat4x4);

	std::uniform_int_distribution<> distr(0,totalShapes - 1);

	int randomNumber = distr(gen);

	memcpy(block,blockModel[randomNumber],sizeof(blockModel[randomNumber]));

}

void Block::Update()
{
	y++;
}


int Block::GetX() 
{
	return x;
}
int Block::GetY() 
{
	return y;
}

void Block::MoveBlockLeft()
{
	x--;
}
void Block::MoveBlockRight()
{
	x++;
}
void Block::MoveBlockDown()
{
	y = 15;
}



void Block::Rotate() {

	mat4x4 temp;
	memset(temp,0,sizeof(temp));
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block[i][j] == 1) {
				temp[3 - j][i] = block[i][j];
			}
		}
	}

	memcpy(block, temp, sizeof(temp));
}

void Block::rollback()
{
	x = prevX;
	y = prevY;
}

void Block::UpdatePos() {
	prevX = x;
	prevY = y;
}

const mat4x4& Block::GetShape() const
{
	return block;
}

