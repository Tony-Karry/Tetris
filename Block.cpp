#include <easyx.h>
#include <ctime>
#include "graphics.h"
#include <windows.h>
#include "Tetris.h"
#include "Block.h"
#include<iostream>
#include<fstream>
using namespace std;
bool Block::blockData[7][4][4] = { 0 };


Block::Block(Tetris& game, const int& x, const int& y) : game(game), x(x), y(y)
{
	blockType = rand() % 7;
	direction = 1;

	// 设置方块填充颜色
	switch (blockType)
	{
	case 0:
		color = BLUE;
		break;

	case 1:
		color = GREEN;
		break;

	case 2:
		color = CYAN;
		break;

	case 3:
		color = RED;
		break;

	case 4:
		color = MAGENTA;
		break;

	case 5:
		color = BROWN;
		break;

	case 6:
		color = YELLOW;
	}

	// 复制方块数据
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			block[i][j] = blockData[blockType][i][j];
		}
	}
}

bool Block::move(const int& moveDirection)
{
	switch (moveDirection)
	{
	case 0:
		y++;
		if (checkCollision())
		{
			y--;
			return true;
		}
		break;

	case 1:
		x--;
		if (checkCollision())
		{
			x++;
		}
		break;

	case 2:
		x++;
		if (checkCollision())
		{
			x--;
		}
		break;
	}
	return false;
}

void Block::drawNextBlock() const
{
	setbkmode(TRANSPARENT);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[i][j])
			{
				// 绘制方块，Y 坐标为负时不绘制
				if (j + y >= 0)
				{
					setfillcolor(color);
					int left = 20 + BLOCK_WIDTH * (x + i), top = 20 + BLOCK_WIDTH * (j + y);
					fillrectangle(left, top, left + BLOCK_WIDTH, top + BLOCK_WIDTH);
				}
			}
		}
	}
}

void Block::clearBlock() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[i][j])
			{
				// 清除方块
				int left = 20 + BLOCK_WIDTH * (x + i), top = 20 + BLOCK_WIDTH * (j + y);
				clearrectangle(left, top, left + BLOCK_WIDTH, top + BLOCK_WIDTH);
			}
		}
	}
}

void Block::addMap() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[i][j])
			{
				// 添加方块
				game.updateMap(x + i, y + j, block[i][j]);
			}
		}
	}
}

void Block::rotate()
{
	// 田型方块不旋转
	if (blockType != 4)
	{
		// 只旋转左上角 3 * 3 区域
		bool temp;
		// 顺时针旋转
		if (direction == 0)
		{
			// 四个角旋转
			temp = block[0][0];
			block[0][0] = block[0][2];
			block[0][2] = block[2][2];
			block[2][2] = block[2][0];
			block[2][0] = temp;

			// 十字上下左右四个位置旋转旋转
			temp = block[1][0];
			block[1][0] = block[0][1];
			block[0][1] = block[1][2];
			block[1][2] = block[2][1];
			block[2][1] = temp;
		}
		// 逆时针旋转
		else
		{
			// 角旋转
			temp = block[0][0];
			block[0][0] = block[2][0];
			block[2][0] = block[2][2];
			block[2][2] = block[0][2];
			block[0][2] = temp;

			// 十字上下左右四个位置旋转旋转
			temp = block[1][0];
			block[1][0] = block[2][1];
			block[2][1] = block[1][2];
			block[1][2] = block[0][1];
			block[0][1] = temp;
		}

		// 处理其它方块特殊情况
		switch (blockType)
		{
			// 长条方块只会在block[][1]列或block[1][]行
		case 0:
			if (block[1][3])
			{
				block[1][3] = 0;
				block[3][1] = 1;
			}
			else
			{
				block[1][3] = 1;
				block[3][1] = 0;
			}
			break;

			// Z 字方块（正）和 Z 字方块（反）
		case 5:
		case 6:
			// 没有发生碰撞才执行此操作
			if (!checkCollision())
			{
				direction = !direction;
			}
		}

		// 碰撞检查，如果发生碰撞则往相反方向旋转一次
		if (checkCollision())
		{
			// 相反方向旋转
			if (direction == 1)	// 顺时针旋转
			{
				// 角旋转
				temp = block[0][0];
				block[0][0] = block[0][2];
				block[0][2] = block[2][2];
				block[2][2] = block[2][0];
				block[2][0] = temp;

				// 十字中心旋转
				temp = block[1][0];
				block[1][0] = block[0][1];
				block[0][1] = block[1][2];
				block[1][2] = block[2][1];
				block[2][1] = temp;
			}
			else	// 逆时针旋转
			{
				// 角旋转
				temp = block[0][0];
				block[0][0] = block[2][0];
				block[2][0] = block[2][2];
				block[2][2] = block[0][2];
				block[0][2] = temp;

				// 十字中心旋转
				temp = block[1][0];
				block[1][0] = block[2][1];
				block[2][1] = block[1][2];
				block[1][2] = block[0][1];
				block[0][1] = temp;
			}
			if (blockType == 0)
			{
				if (block[1][3])
				{
					block[1][3] = 0;
					block[3][1] = 1;
				}
				else
				{
					block[1][3] = 1;
					block[3][1] = 0;
				}
			}
		}
	}
}

bool Block::checkCollision() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// 判断方块是否与地图发生碰撞，顶部不判断
			if ((game.getMap(x + i, y + j) || 20 + BLOCK_WIDTH * (x + i) < 20 || 20 + BLOCK_WIDTH * (x + i) + BLOCK_WIDTH > 270 || 20 + BLOCK_WIDTH * (j + y) + BLOCK_WIDTH > 520) && block[i][j])
			{
				return true;
			}
		}
	}
	return false;
}

void Block::generateBlockData()
{
	// 长条方块
	blockData[0][1][0] = blockData[0][1][1] = blockData[0][1][2] = blockData[0][1][3] = true;
	// L字方块（正）
	blockData[2][2][0] = blockData[2][1][0] = blockData[2][1][1] = blockData[2][1][2] = true;
	// L字方块（反）
	blockData[1][0][0] = blockData[1][1][0] = blockData[1][1][1] = blockData[1][1][2] = true;
	// T字方块
	blockData[3][1][0] = blockData[3][0][1] = blockData[3][1][1] = blockData[3][2][1] = true;
	// 田字方块
	blockData[4][0][0] = blockData[4][1][0] = blockData[4][0][1] = blockData[4][1][1] = true;
	// Z 字方块（正）
	blockData[6][2][0] = blockData[6][1][0] = blockData[6][1][1] = blockData[6][0][1] = true;
	// Z 字方块（反）
	blockData[5][0][0] = blockData[5][1][0] = blockData[5][1][1] = blockData[5][2][1] = true;
}

void Block::setPos(const int& x, const int& y)
{
	this->x = x;
	this->y = y;
}

void Block::randType()
{
	blockType = rand() % 7;
	direction = 1;

	// 设置方块填充颜色
	switch (blockType)
	{
	case 0:
		color = BLUE ;
		break;

	case 1:
		color = GREEN;
		break;

	case 2:
		color = CYAN;
		break;

	case 3:
		color = RED;
		break;

	case 4:
		color = MAGENTA;
		break;

	case 5:
		color = MAGENTA;
		break;

	case 6:
		color = YELLOW;
	}

	// 复制方块数据
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			block[i][j] = blockData[blockType][i][j];
		}
	}
}

void Block::operator=(const Block& block)
{
	x = block.x;
	y = block.y;
	blockType = block.blockType;
	direction = block.direction;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->block[i][j] = block.block[i][j];
		}
	}
	color = block.color;
}

//本方块x坐标、y坐标、类型、选转方向、方块数据、颜色
void Block::store(const char name[256])
{
	//构造文件名
	char filename[256];
	strcpy_s(filename, name);
	strcat_s(filename, ".txt");
	fstream outfile(filename, ios::out | ios::binary|ios::app);
	outfile.write((char*)&x, sizeof(x));
	outfile.write((char*)&y, sizeof(y));
	outfile.write((char*)&blockType, sizeof(blockType));
	outfile.write((char*)&direction, sizeof(direction));
	for(int i=0;i<4;i++)
		for (int j = 0; j < 4; j++)
			outfile.write((char*)&block[i][j], sizeof(block[i][j]));
	outfile.write((char*)&color, sizeof(color));
	outfile.close();
}

void Block::read(const char name[256])
{
	int wide = MAP_WIDTH, height = MAP_HEIGHT, blockwide = BLOCK_WIDTH;
	//构造文件名
	char filename[256];
	strcpy_s(filename, name);
	strcat_s(filename, ".txt");
	fstream infile(filename, ios::in | ios::binary);
	infile.seekg(sizeof(wide)+sizeof(height)+sizeof(blockwide)+sizeof(int)*3+sizeof(bool)*wide*height, ios::beg);
	infile.read((char*)&x, sizeof(x));
	infile.read((char*)&y, sizeof(y));
	infile.read((char*)&blockType, sizeof(blockType));
	infile.read((char*)&direction, sizeof(direction));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			infile.read((char*)&block[i][j], sizeof(block[i][j]));
	infile.read((char*)&color, sizeof(color));
	infile.close();
}
