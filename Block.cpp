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

	// ���÷��������ɫ
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

	// ���Ʒ�������
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
				// ���Ʒ��飬Y ����Ϊ��ʱ������
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
				// �������
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
				// ��ӷ���
				game.updateMap(x + i, y + j, block[i][j]);
			}
		}
	}
}

void Block::rotate()
{
	// ���ͷ��鲻��ת
	if (blockType != 4)
	{
		// ֻ��ת���Ͻ� 3 * 3 ����
		bool temp;
		// ˳ʱ����ת
		if (direction == 0)
		{
			// �ĸ�����ת
			temp = block[0][0];
			block[0][0] = block[0][2];
			block[0][2] = block[2][2];
			block[2][2] = block[2][0];
			block[2][0] = temp;

			// ʮ�����������ĸ�λ����ת��ת
			temp = block[1][0];
			block[1][0] = block[0][1];
			block[0][1] = block[1][2];
			block[1][2] = block[2][1];
			block[2][1] = temp;
		}
		// ��ʱ����ת
		else
		{
			// ����ת
			temp = block[0][0];
			block[0][0] = block[2][0];
			block[2][0] = block[2][2];
			block[2][2] = block[0][2];
			block[0][2] = temp;

			// ʮ�����������ĸ�λ����ת��ת
			temp = block[1][0];
			block[1][0] = block[2][1];
			block[2][1] = block[1][2];
			block[1][2] = block[0][1];
			block[0][1] = temp;
		}

		// �������������������
		switch (blockType)
		{
			// ��������ֻ����block[][1]�л�block[1][]��
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

			// Z �ַ��飨������ Z �ַ��飨����
		case 5:
		case 6:
			// û�з�����ײ��ִ�д˲���
			if (!checkCollision())
			{
				direction = !direction;
			}
		}

		// ��ײ��飬���������ײ�����෴������תһ��
		if (checkCollision())
		{
			// �෴������ת
			if (direction == 1)	// ˳ʱ����ת
			{
				// ����ת
				temp = block[0][0];
				block[0][0] = block[0][2];
				block[0][2] = block[2][2];
				block[2][2] = block[2][0];
				block[2][0] = temp;

				// ʮ��������ת
				temp = block[1][0];
				block[1][0] = block[0][1];
				block[0][1] = block[1][2];
				block[1][2] = block[2][1];
				block[2][1] = temp;
			}
			else	// ��ʱ����ת
			{
				// ����ת
				temp = block[0][0];
				block[0][0] = block[2][0];
				block[2][0] = block[2][2];
				block[2][2] = block[0][2];
				block[0][2] = temp;

				// ʮ��������ת
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
			// �жϷ����Ƿ����ͼ������ײ���������ж�
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
	// ��������
	blockData[0][1][0] = blockData[0][1][1] = blockData[0][1][2] = blockData[0][1][3] = true;
	// L�ַ��飨����
	blockData[2][2][0] = blockData[2][1][0] = blockData[2][1][1] = blockData[2][1][2] = true;
	// L�ַ��飨����
	blockData[1][0][0] = blockData[1][1][0] = blockData[1][1][1] = blockData[1][1][2] = true;
	// T�ַ���
	blockData[3][1][0] = blockData[3][0][1] = blockData[3][1][1] = blockData[3][2][1] = true;
	// ���ַ���
	blockData[4][0][0] = blockData[4][1][0] = blockData[4][0][1] = blockData[4][1][1] = true;
	// Z �ַ��飨����
	blockData[6][2][0] = blockData[6][1][0] = blockData[6][1][1] = blockData[6][0][1] = true;
	// Z �ַ��飨����
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

	// ���÷��������ɫ
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

	// ���Ʒ�������
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

//������x���ꡢy���ꡢ���͡�ѡת���򡢷������ݡ���ɫ
void Block::store(const char name[256])
{
	//�����ļ���
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
	//�����ļ���
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
