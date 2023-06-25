#ifndef BLOCK_H
#define BLOCK_H
#include <easyx.h>
#include <ctime>
#include "graphics.h"
#include <windows.h>
#define MAP_WIDTH 10
#define MAP_HEIGHT 20
#define BLOCK_WIDTH 25
#endif BLOCK_H
using namespace std;
class Block
{
public:
	Block(Tetris& game, const int& x = (MAP_WIDTH - 4) / 2, const int& y = 0);
	//�����������ƶ����ո���ף�0 ��ʾ����һ��1 ��ʾ����һ��2 ��ʾ����һ�񣬵����Ƽ�⵽��ײʱ���� true
	bool move(const int& moveDirection = 0);
	//������һ����
	void drawNextBlock()const;
	//���block[4][4]������
	void clearBlock() const;
	//�Ѵ��׷�����뷽����
	void addMap() const;
	//���ϼ���ת
	void rotate();
	//����Ƿ�����ײ
	bool checkCollision() const;
	//����7�ַ������ݣ�����
	static void generateBlockData();
	//���跽������
	void setPos(const int& x = (MAP_WIDTH - 4) / 2, const int& y = 0);
	//���ת��һ�ַ���
	void randType();
	//����=�Ա㷽��֮�丳ֵ
	void operator=(const Block& block);
	//�浵������Ϣ
	void store(const char[256]);
	void read(const char name[256]);

private:
	//block[0][0]��x����
	int x;
	//block[0][0]��y����
	int y;
	//�������ͣ���7��
	int blockType;
	//��ת����0˳ʱ����ת��1��ʱ����ת
	bool direction;
	//һ����������
	bool block[4][4];
	//һ�ַ������ɫ��7�ָ��в�ͬ����ɫ
	COLORREF color;
	//7�ַ�������ݣ�����
	static bool blockData[7][4][4];
	Tetris& game;
};
