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
	//方块左右下移动，空格落底，0 表示下移一格，1 表示左移一格，2 表示右移一格，当下移检测到碰撞时返回 true
	bool move(const int& moveDirection = 0);
	//绘制下一方块
	void drawNextBlock()const;
	//清除block[4][4]中数据
	void clearBlock() const;
	//把触底方块加入方块盘
	void addMap() const;
	//按上键旋转
	void rotate();
	//检测是否有碰撞
	bool checkCollision() const;
	//生成7种方块数据，共用
	static void generateBlockData();
	//重设方块坐标
	void setPos(const int& x = (MAP_WIDTH - 4) / 2, const int& y = 0);
	//随机转换一种方块
	void randType();
	//重载=以便方块之间赋值
	void operator=(const Block& block);
	//存档方块信息
	void store(const char[256]);
	void read(const char name[256]);

private:
	//block[0][0]的x坐标
	int x;
	//block[0][0]的y坐标
	int y;
	//方块类型，共7种
	int blockType;
	//旋转方向，0顺时针旋转，1逆时针旋转
	bool direction;
	//一个方块数据
	bool block[4][4];
	//一种方块的颜色，7种各有不同的颜色
	COLORREF color;
	//7种方块的数据，共用
	static bool blockData[7][4][4];
	Tetris& game;
};
