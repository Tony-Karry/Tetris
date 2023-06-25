#ifndef  TETRIS_H
#define TETRIS_H
#include <easyx.h>
#include <ctime>
#include "graphics.h"
#include <windows.h>
#define MAP_WIDTH 10	
#define MAP_HEIGHT 20	
#define BLOCK_WIDTH 25
#endif  TETRIS_H
using namespace std;
class Tetris
{
public:
	//绘制设置页面
	void drawSetting();
	//绘制设置页面的按钮
	void button(int x, int y, int w, int h, TCHAR* text)const;
	//绘制输入用户名界面
	void drawSetUser();
	//开始游戏
	void playNewGame(bool continueFlag);
	//将方块盘中已经有方块的格子填色
	void drawMap() const;
	//绘制游戏界面的其他信息
	void drawMapMessage()const;
	//检查是否填满一行
	bool checkLine(const unsigned& line) const;
	//清除填满的行
	void clearLine();
	// 设置地图
	void updateMap(const int& x, const int& y, const bool& value);
	// 获取地图
	const bool& getMap(const int& x, const int& y) const;
	// 添加游戏时间,没用	
	void addGameTime();
	//绘制是否存档计入排行榜
	void drawRecord();
	//计入排行榜
	void record();
	//展示排行榜
	void showRecord();
	//展示游戏规则
	void showRule();
	//存档游戏数据
	void store();
	void read();
	//清楚方块盘数据
	void cleanMap();
	//绘制游戏设置界面
	void drawGameSetting();
	//绘制修改速度输入框界面
	void drawSetSpeed();
	//绘制背景设置界面
	void drawSetPicture();
	//输入用户名按钮设置
    friend LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//输入速度
	friend LRESULT WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//播放音乐
	void playMusic();
	//绘制选择音乐界面
	void drawSetMusic();
private:
	//用户名
	static char userName[256];
	//方块盘
	bool map[MAP_WIDTH][MAP_HEIGHT] = { 0 };
	//已消行数
	int clearLineNum = 0;
	//分数
	int score = 0;
	//下落速度
	static int speed;
	// 游戏时间
	unsigned gameTime = 0;		
	//背景图片代号
	int backGroundNum=1;
	//背景音乐代号
	int music = 1;
	//是否重播音乐
	bool replayMusic = 1;
};
