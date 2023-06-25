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
	//��������ҳ��
	void drawSetting();
	//��������ҳ��İ�ť
	void button(int x, int y, int w, int h, TCHAR* text)const;
	//���������û�������
	void drawSetUser();
	//��ʼ��Ϸ
	void playNewGame(bool continueFlag);
	//�����������Ѿ��з���ĸ�����ɫ
	void drawMap() const;
	//������Ϸ�����������Ϣ
	void drawMapMessage()const;
	//����Ƿ�����һ��
	bool checkLine(const unsigned& line) const;
	//�����������
	void clearLine();
	// ���õ�ͼ
	void updateMap(const int& x, const int& y, const bool& value);
	// ��ȡ��ͼ
	const bool& getMap(const int& x, const int& y) const;
	// �����Ϸʱ��,û��	
	void addGameTime();
	//�����Ƿ�浵�������а�
	void drawRecord();
	//�������а�
	void record();
	//չʾ���а�
	void showRecord();
	//չʾ��Ϸ����
	void showRule();
	//�浵��Ϸ����
	void store();
	void read();
	//�������������
	void cleanMap();
	//������Ϸ���ý���
	void drawGameSetting();
	//�����޸��ٶ���������
	void drawSetSpeed();
	//���Ʊ������ý���
	void drawSetPicture();
	//�����û�����ť����
    friend LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//�����ٶ�
	friend LRESULT WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//��������
	void playMusic();
	//����ѡ�����ֽ���
	void drawSetMusic();
private:
	//�û���
	static char userName[256];
	//������
	bool map[MAP_WIDTH][MAP_HEIGHT] = { 0 };
	//��������
	int clearLineNum = 0;
	//����
	int score = 0;
	//�����ٶ�
	static int speed;
	// ��Ϸʱ��
	unsigned gameTime = 0;		
	//����ͼƬ����
	int backGroundNum=1;
	//�������ִ���
	int music = 1;
	//�Ƿ��ز�����
	bool replayMusic = 1;
};
