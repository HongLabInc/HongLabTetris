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
	memcpy(prevShape,blockModel[randomNumber],sizeof(blockModel[randomNumber]));

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

	memcpy(prevShape,block,sizeof(prevShape));

	mat4x4 temp = {0};  // 임시 배열 초기화

	// 90도 시계 방향 회전 로직
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			temp[j][3 - i] = block[i][j];
		}
	}

	// 회전 결과를 block에 복사
	memcpy(block,temp,sizeof(temp));
}

void Block::rollback()
{
	memcpy(block,prevShape,sizeof(prevShape));
	x = prevX;
	y = prevY;
}

void Block::UpdatePos() {
	prevX = x;
	prevY = y;
	memcpy(prevShape,block,sizeof(prevShape));  // 현재 블록 상태 저장
}

const mat4x4& Block::GetShape() const
{
	return block;
}

