#include <easyx.h>
#include <ctime>
#include<cstdlib>
#include "Tetris.h"
#include "Block.h"
#include "graphics.h"
#include <windows.h>
#include <algorithm>
#include<conio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include <sstream>
#include<direct.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
struct recordData {
	char Name[256];
	int line;
	int scores;
};
HWND hwndEdit;
HWND hwndButton;
char Tetris::userName[256] = { '\0' };
HWND hwndEdit1;
HWND hwndButton1;
 int Tetris::speed = 500;

void Tetris::drawSetting()
{
	//��ʼ����ͼ���ڲ����ñ��������ı�
	SetWindowText(initgraph(474, 640), L"����˹����_���ý���");
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	switch (backGroundNum)
	{
	case 1:
		loadimage(&picture, L"picture1.jpg");
		break;
	case 2:
		loadimage(&picture, L"picture2.jpg");
		break;
	case 3:
		loadimage(&picture, L"picture3.jpg");
		break;
	case 4:
		loadimage(&picture, L"picture4.jpg");
		break;
	default:
		loadimage(&picture, L"picture1.jpg");
		break;
	}
	putimage(0, 0, &picture);
	TCHAR s[50] = L"��ʼ����Ϸ";
	TCHAR s1[50] = L"�����浵��Ϸ";
	TCHAR s2[50] = L"���а�";
	TCHAR s3[50] = L"��Ϸ����";
	TCHAR s4[50] = L"��Ϸ����";

	button(145, 230, 180, 50, s);
	button(145, 310, 180, 50, s1);
	button(145, 390, 180, 50, s2);
	button(145, 470, 180, 50, s3);
	button(145, 550, 180, 50, s4);
	settextstyle(60, 0, L"����");
	settextcolor(BLACK);
	outtextxy(85, 100, L"����˹����");
}

void Tetris::button(int x, int y, int w, int h, TCHAR* text)const
{
	//ָ��ͼ�������������ʱ�ı���ģʽ��TRANSPARENT��ʾ������͸����
	setbkmode(TRANSPARENT);
	setfillcolor(LIGHTGRAY);
	fillroundrect(x, y, x + w, y + h, 10, 10);
	// ����ַ�����MBCS �ַ�����
	settextstyle(30, 0, L"����");
	TCHAR s[50] = L"hello";

	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	outtextxy(tx, ty, text);
}

//LRESULT �ǳ��򷵻ص�Windows������ֵ�� ������������ض���Ϣ����Ӧ�� ��ֵ�ĺ���ȡ������Ϣ���롣 CALLBACK �Ǻ����ĵ���Լ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)hwndButton)
		{
			// ���ȷ����ť
			char buffer[256];
			// ��ȡ������е��ı�
			GetWindowTextA(hwndEdit, buffer, sizeof(buffer));
			// ��ֵ��Tetris������ݳ�Ա
			strcpy_s(Tetris::userName, buffer);
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
	return LRESULT();
}
void Tetris::drawSetUser()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"MyClass";
	RegisterClass(&wc);
	// ��ȡ����̨���ھ��
	HWND hwndConsole = GetConsoleWindow();
	// ���ؿ���̨����
	ShowWindow(hwndConsole, SW_HIDE);
	HWND hwnd = CreateWindow(
		L"MyClass",
		L"�����û���",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 640,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	//HWND_TOP��ʾ���������ڶ�����SWP_NOSIZE��־��ʾ���ڵĴ�С�������仯��SWP_NOZORDER��־��ʾ���ڵ� Z �򲻷����仯��
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	hwndEdit = CreateWindow(
		L"edit",
		NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
		10, 10, 300, 30,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	hwndButton = CreateWindow(
		L"button",
		L"ȷ���û���",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		320, 10, 100, 30,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Tetris::playNewGame(bool continueFlag)
{
	//����ָ�����ڵı��������ı�������Ϊ��Ҫ�������ı��Ĵ��ڣ��µı����ı���L��ʾ������ʹ�������ֽڵ� unicode �ַ���
	//initgraphΪ��ʼ����ͼ����
	SetWindowText(initgraph(480, 640), L"����˹����_��Ϸ����");
	HWND hwnd = GetHWnd();
	//����ָ�����ڵ�λ�úͳߴ�
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//���ñ���ɫ
	setbkcolor(WHITE);
	//ʹ�õ�ǰ����ɫ��ջ�ͼ�豸
	cleardevice();
	IMAGE picture;
	switch (backGroundNum)
	{
	case 1:
		loadimage(&picture, L"picture1.jpg");
		break;
	case 2:
		loadimage(&picture, L"picture2.jpg");
		break;
	case 3:
		loadimage(&picture, L"picture3.jpg");
		break;
	case 4:
		loadimage(&picture, L"picture4.jpg");
		break;
	default:
		loadimage(&picture, L"picture1.jpg");
		break;
	}
	putimage(0, 0, &picture);
	setlinecolor(BLACK);
	srand(time(NULL));
	Block::generateBlockData();
	drawMap();
	drawMapMessage();
	Block b(*this);
	if(continueFlag)
	b.read(Tetris::userName);
	Block nextBlock(*this, 11, 2);
	// ʱ�ӿ�ʼʱ�䡢����ʱ��
	clock_t start = 0;
	clock_t end;
	//���ڱ��������Ϣ�Ľṹ��
	ExMessage msg;
	nextBlock.drawNextBlock();

	//��ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ�������ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
	BeginBatchDraw();
	while (true)
	{
		b.clearBlock();
		//��վ�������
		clearrectangle(20, 20, 270, 520);
		putimage(0, 0, &picture);
		nextBlock.drawNextBlock();
		drawMap();
		//peekmessage���ڻ�ȡһ����Ϣ�����������ء�EM_KEY��ʾ������Ϣ��WM_KEYDOWN��ʾ����������Ϣ
		while ((peekmessage(&msg, EM_KEY) && msg.message == WM_KEYDOWN))
		{
			//vkcode��ʾ������������룬������Ϣ�������Ϊ EX_KEY ʱ��Ч
			switch (msg.vkcode)
			{
				// �ϼ���ת
			case VK_UP:
				b.rotate();
				break;

				// �¼���������
			case VK_DOWN:
				if (b.move())
				{
					// �������÷���
					b.addMap();
					b = nextBlock;
					b.setPos();
					nextBlock.clearBlock();
					nextBlock.setPos(11, 2);
					nextBlock.randType();
					nextBlock.drawNextBlock();

					// ������ո�����ȴ��������ײ��˵����Ϸ����
					if (b.checkCollision())
					{
						//MessageBox��ʾһ��ģʽ�Ի�����Ϣ�򷵻�һ������ֵ����ֵָʾ�û������İ�ť��GetHWnd���ڻ�ȡ��ͼ���ھ��
						PlaySound(TEXT("MusicEx_Lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
						replayMusic = 1;
						MessageBox(GetHWnd(), L"��Ϸ������", L"��ʾ", MB_ICONWARNING);
						
						goto exit;
					}
				}
				break;

				// ����ƶ�
			case VK_LEFT:
				b.move(1);
				break;

				// �Ҽ��ƶ�
			case VK_RIGHT:
				b.move(2);
				break;

				//P����ͣ
			case 0x50:
				while (1) 
				{
					
					if (peekmessage(&msg, EM_KEY) && msg.message == WM_KEYDOWN && msg.vkcode == 0x50)
						break;
				}
				break;
			case 0x4f:
				store();
				b.store(Tetris::userName);
				MessageBox(GetHWnd(), L"�浵�ɹ���", L"��ʾ", MB_ICONWARNING);
				goto exit;
				break;
				// Esc ���˳�����
			case VK_ESCAPE:
				goto exit;
				break;
			}
		}

		// �ж�ÿ����speed��������һ��
		end = clock();
		if (end - start >= Tetris::speed)
		{
			if (b.move())
			{
				// �������÷���
				b.addMap();
				b = nextBlock;
				b.setPos();
				nextBlock.clearBlock();
				nextBlock.setPos(11, 2);
				nextBlock.randType();
				nextBlock.drawNextBlock();

				// �������ո�����ȴ��������ײ��˵����Ϸ����
				if (b.checkCollision())
				{
					PlaySound(TEXT("MusicEx_Lose.wav"), NULL, SND_FILENAME | SND_ASYNC );
					MessageBox(GetHWnd(), L"��Ϸ������", L"��ʾ", MB_ICONWARNING);
					replayMusic = 1;
					goto exit;
				}
			}
			//start=����ʱ�䣬�����¼���ʱ��
			start = clock();
			addGameTime();
		}
		b.drawNextBlock();
		clearLine();
		// ˢ�»�����
		FlushBatchDraw();
		// Sleep��������ʹ��������򣨽��̣�������̣߳��������ߣ�ʹ����һ��ʱ���ڴ��ڷǻ״̬��ÿ 50 �������һ�ΰ���
		Sleep(40);
	}
exit:
	//�����������ƣ���ִ��δ��ɵĻ�������
	EndBatchDraw();
	closegraph();
}

void Tetris::drawMap()const
{
	//��������Բ�Ǿ��Ρ�����Ϊ��Բ�Ǿ����� x ���꣬���� y ���꣬�Ҳ� x ���꣬�ײ� y ����
	//roundrect(10, 10, 470, 430, 10, 10);
	//�����ɫ
	setbkmode(TRANSPARENT);
	setfillcolor(DARKGRAY);
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			int left = 20 + BLOCK_WIDTH * i, top = 20 + BLOCK_WIDTH * j;
			//���ƾ��Σ��þ���ʹ�õ�ǰ����������ʹ�õ�ǰ������䡣����Ϊ���������Ͻǵ� x ���꣬���Ͻǵ� y ���꣬���½ǵ� x ���꣬���½ǵ� y ����
			rectangle(left, top, left + BLOCK_WIDTH, top + BLOCK_WIDTH);
			if (map[i][j])
			{
				//���б߿��������
				fillrectangle(left, top, left + BLOCK_WIDTH, top + BLOCK_WIDTH);
			}
		}
	}
}

void Tetris::drawMapMessage() const
{
	setbkmode(TRANSPARENT);
	// �������������
	LOGFONT f;
	//��ȡ��ǰ������ʽ
	gettextstyle(&f);
	//ָ���߶����ݳ�Ա
	f.lfHeight = 30;
	//���ƾ����������Ƶ�lfFaceName
	wcscpy_s(f.lfFaceName, L"����");
	//ָ�����ֵ��������Ϊ�����������Ҫ
	f.lfQuality = DEFAULT_QUALITY;
	//���õ�ǰ������ʽ
	settextstyle(&f);
	settextcolor(BLACK);

	// ���� ����һ���顱 ��ʾ
	outtextxy(295, 20, L"��һ����");
	rectangle(295, 70, 395, 170);

	// ���Ʒ����������������ٶȡ���ͣ�浵��ʾ
	f.lfHeight = 25;
	settextstyle(&f);
	//ʵ�ֶ���Ŀ��ַ�����
	wchar_t str[10];
	wchar_t str1[10];
	wchar_t str2[10];
	//������д��ָ���Ļ���������ʽ�淶��%��ͷ��u��ʾ�޷�������������
	wsprintf(str, L"����������%u", clearLineNum);
	outtextxy(295, 200, str);
	wsprintf(str1, L"�÷֣�%u", score);
	outtextxy(295, 230, str1);
	wsprintf(str2, L"�ٶȣ�%u", Tetris::speed);
	outtextxy(295, 260, str2);
	outtextxy(20, 550, L"P����ͣ/������O���浵");


	//���ƴ浵��ť
	//TCHAR s[50] = L"�浵��Ϸ������";
	//button(230, 540, 220, 40, s);

	//����ʱ��
	//wsprintf(str, L"ʱ�䣺%u��", gameTime / 1000);
	//outtextxy(230, 175, str);

	// ���� ������˹���顱
	f.lfHeight = 40;
	settextstyle(&f);
	settextcolor(RED);
	//outtextxy(234, 250, L"����˹");
	//outtextxy(250, 290, L"����");
	//outtextxy(240, 330, L"Tetris");

	// ���� �����ߡ�
	f.lfHeight = 25;
	settextstyle(&f);
	//outtextxy(234, 395, L"TONY");
}

bool Tetris::checkLine(const unsigned& line) const
{
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		if (map[i][line] == 0)
		{
			return false;
		}
	}
	return true;
}

void Tetris::clearLine()
{
	int line = -1;
	// �ж���һ������
	for (int j = 0; j < MAP_HEIGHT; j++)
	{
		if (checkLine(j))
		{
			line = j;
			break;
		}
	}

	if (line != -1)
	{
		// ����һ����������
		for (int j = line; j > 0; j--)
		{
			for (int i = 0; i < MAP_WIDTH; i++)
			{
				map[i][j] = map[i][j - 1];
			}
		}
		// ����Ϸ����
		score += MAP_WIDTH;
		clearLineNum += 1;
	}
	drawMapMessage();
}

void Tetris::updateMap(const int& x, const int& y, const bool& value)
{
	map[x][y] = value;
}

const bool& Tetris::getMap(const int& x, const int& y) const
{
	return map[x][y];
}

void Tetris::addGameTime()
{
	gameTime += 500;
}

void Tetris::drawRecord()
{
	HWND hwnd = initgraph(474, 640);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	switch (backGroundNum)
	{
	case 1:
		loadimage(&picture, L"picture1.jpg");
		break;
	case 2:
		loadimage(&picture, L"picture2.jpg");
		break;
	case 3:
		loadimage(&picture, L"picture3.jpg");
		break;
	case 4:
		loadimage(&picture, L"picture4.jpg");
		break;
	default:
		loadimage(&picture, L"picture1.jpg");
		break;
	}
	putimage(0, 0, &picture);
	TCHAR s[50] = L"ȷ������";
	TCHAR s1[50] = L"������";
	button(40, 470, 180, 50, s);
	button(260, 470, 180, 50, s1);
	settextstyle(50, 0, L"����");
	settextcolor(BLACK);
	outtextxy(20, 100, L"�浵�������а��");
}

//���ڱȽϵ÷�
bool cmp(const recordData& a, const recordData& b) {
	return a.scores > b.scores;
}
void Tetris::record()
{
	fstream outfile("data.txt", ios::out | ios::binary | ios::app | ios::in);
	recordData data{ '\0',0,0};
	outfile.seekg(0, ios::end);
	int end = outfile.tellg();
	outfile.seekg(0, ios::beg);
	//�����Ƿ��Ѿ������û���
	if(outfile.tellg() != end)
	outfile.read((char*)&data, sizeof(data));
	while ((strcmp(data.Name, userName) != 0) && (outfile.tellg() != end))
		outfile.read((char*)&data, sizeof(data));
	//�������ҷ������ߣ��򸲸Ǵ���
	if (strcmp(data.Name, userName) == 0 && data.scores > score)
	{
		//���tellgָ��
		streampos pos = outfile.tellg();
		//����seekpָ��
		outfile.seekp(pos);
		outfile.seekp(-static_cast<streamoff>(sizeof(recordData)), ios::cur);
		
	}
	//�������ڣ���������
	if (strcmp(data.Name, userName) != 0)
	{
		streampos pos = outfile.tellg();
		outfile.seekp(pos);
		strcpy_s(data.Name, userName);
		data.line = clearLineNum;
		data.scores = score;
		outfile.write((char*)&data, sizeof(data));
	}
	outfile.close();
	fstream outfile1("data.txt", ios::out | ios::binary | ios::in);
	//��ʼ����
	// ��ȡ�ļ�����
	outfile1.seekg(0, ios::end);
	streampos length = outfile1.tellg();
	int count = length / sizeof(recordData);
	// ��������ռ䣬����ȡ�ļ�����������
	recordData* data1 = new recordData[count];
	outfile1.seekg(0, ios::beg);
	outfile1.read(reinterpret_cast<char*>(data1), length);
	// ���� scores ����
	sort(data1, data1 + count, cmp);
	// ���ź��������д�ص��ļ���
	outfile1.seekp(0, ios::beg);
	outfile1.write(reinterpret_cast<const char*>(data1), length);
	// �ͷ�����ռ䣬���ر��ļ�
	delete[] data1;
	outfile1.close();
	
}

void Tetris::showRecord()
{
	SetWindowText(initgraph(480, 640), L"���а�");
	HWND hwnd = GetHWnd();
	//����ָ�����ڵ�λ�úͳߴ�
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ifstream infile("data.txt", ios::in | ios::binary);
	// ��ȡ�ļ�����
	infile.seekg(0, ios::end);
	streampos length = infile.tellg();
	int count = length / sizeof(recordData);
	// ��������ռ䣬����ȡ�ļ�����������
	recordData* data = new recordData[count];
	infile.seekg(0, ios::beg);
	infile.read(reinterpret_cast<char*>(data), length);
	// �ڴ�������ʾ���а�
	setbkmode(TRANSPARENT);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	IMAGE picture;
	switch (backGroundNum)
	{
	case 1:
		loadimage(&picture, L"picture1.jpg");
		break;
	case 2:
		loadimage(&picture, L"picture2.jpg");
		break;
	case 3:
		loadimage(&picture, L"picture3.jpg");
		break;
	case 4:
		loadimage(&picture, L"picture4.jpg");
		break;
	default:
		loadimage(&picture, L"picture1.jpg");
		break;
	}
	putimage(0, 0, &picture);
	for (int i = 0; i < count; i++) 
	{
		wchar_t buffer[256];
		wsprintf(buffer,L"%d. %S: %d��", i + 1,data[i].Name, data[i].scores);
		outtextxy(20, 20 + i * 20, buffer);
	}
	infile.close();
	//������ա����ذ�ť
	TCHAR s[50] = L"����";
	TCHAR s1[50] = L"���";
	button(100, 540, 80, 40, s1);
	button(300, 540, 80, 40, s);
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 300 && msg.x <= 300 + 80 && msg.y >= 540 && msg.y <= 540 + 40)
				{
					goto back;
				}
				if (msg.x >= 100 && msg.x <= 100 + 80 && msg.y >= 540 && msg.y <= 540 + 40)
				{
					ofstream outfile("data.txt", ios::trunc);
					outfile.close();
					cleardevice();
					putimage(0, 0, &picture);
					button(100, 540, 80, 40, s1);
					button(300, 540, 80, 40, s);
				}
				break;
			default:
				break;
			}
		}
	}
back:
	// �ͷ�����ռ䣬���ر��ļ�
	delete[] data;
	infile.close();
	closegraph();
}

void Tetris::showRule()
{
	SetWindowText(initgraph(480, 640), L"��Ϸ����");
	HWND hwnd = GetHWnd();
	//����ָ�����ڵ�λ�úͳߴ�
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setbkmode(TRANSPARENT);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	switch (backGroundNum)
	{
	case 1:
		loadimage(&picture, L"picture1.jpg");
		break;
	case 2:
		loadimage(&picture, L"picture2.jpg");
		break;
	case 3:
		loadimage(&picture, L"picture3.jpg");
		break;
	case 4:
		loadimage(&picture, L"picture4.jpg");
		break;
	default:
		loadimage(&picture, L"picture1.jpg");
		break;
	}
	putimage(0, 0, &picture);
	// �������������
	LOGFONT f;
	//��ȡ��ǰ������ʽ
	gettextstyle(&f);
	//ָ���߶����ݳ�Ա
	f.lfHeight = 30;
	//���ƾ����������Ƶ�lfFaceName
	wcscpy_s(f.lfFaceName, L"����");
	//ָ�����ֵ��������Ϊ�����������Ҫ
	f.lfQuality = DEFAULT_QUALITY;
	//���õ�ǰ������ʽ
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(150, 30, L"��Ϸ����");
	settextstyle(16, 0, L"����");	
	outtextxy(5, 80, L"ʹ�ü��̡����ƶ��������Σ����������䣬O���浵��P����ͣ");
	outtextxy(5, 100, L"��С������ɵĲ�ͬ��״�İ��½������Ļ�Ϸ������������ͨ");
	outtextxy(5, 120, L"����������λ�úͷ���ʹ��������Ļ�ײ�ƴ��������һ����");
	outtextxy(5, 140, L"��������Щ�����ĺ������漴��ʧ�������������İ���ڳ��ռ䣬");
	outtextxy(5, 160, L"���ͬʱ����ҵõ�ÿ��һ��10�ֽ�����û�б��������ķ��鲻");
	outtextxy(5, 180, L"�϶ѻ�������һ���ѵ���Ļ���ˣ���ұ���䣬��Ϸ������");
	//���Ʒ��ذ�ť
	TCHAR s[50] = L"����";
	button(200, 540, 80, 40, s);
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 230 && msg.x <= 230 + 50 && msg.y >= 540 && msg.y <= 540 + 40)
				{
					goto back;
				}
				break;
			default:
				break;
			}
		}
	}
back:
	closegraph();
}

//��¼˳�򣺷����̿���������������������÷֡��ٶȡ�����������
void Tetris::store()
{
	int wide = MAP_WIDTH, height = MAP_HEIGHT, blockwide = BLOCK_WIDTH;
	//�����ļ���
	char filename[256];
	strcpy_s(filename, Tetris::userName);
	strcat_s(filename, ".txt");
	fstream outfile(filename, ios::out | ios::binary);
	outfile.write((char*)&wide, sizeof(wide));
	outfile.write((char*)&height, sizeof(height));
	outfile.write((char*)&blockwide, sizeof(blockwide));
	outfile.write((char*)&clearLineNum, sizeof(clearLineNum));
	outfile.write((char*)&score, sizeof(score));
	outfile.write((char*)&speed, sizeof(speed));
	for(int i=0;i<wide;i++)
		for (int j = 0; j < height; j++)
			outfile.write((char*)&map[i][j], sizeof(map[i][j]));
	outfile.close();
}

void Tetris::read()
{
	//�����ļ���
	char filename[256];
	strcpy_s(filename, Tetris::userName);
	strcat_s(filename, ".txt");
	ifstream infile(filename, ios::in | ios::binary);
	int wide , height , blockwide;
	infile.read((char*)&wide, sizeof(wide));
	infile.read((char*)&height, sizeof(height));
	infile.read((char*)&blockwide, sizeof(blockwide));
	infile.read((char*)&clearLineNum, sizeof(clearLineNum));
	infile.read((char*)&score, sizeof(score));
	infile.read((char*)&speed, sizeof(speed));
	for (int i = 0; i < wide; i++)
		for (int j = 0; j < height; j++)
			infile.read((char*)&map[i][j], sizeof(map[i][j]));
	infile.close();
}

void Tetris::cleanMap()
{
	for (int i = 0; i < MAP_WIDTH; i++)
		for (int j = 0; j < MAP_HEIGHT; j++)
			map[i][j] = 0;
	score = 0;
	gameTime = 0;
	clearLineNum = 0;
	char ch[256] = { '\0' };
	strcpy_s(Tetris::userName, ch);
}

void Tetris::drawGameSetting()
{
	//��ʼ����ͼ���ڲ����ñ��������ı�
	SetWindowText(initgraph(480, 640), L"����˹����_��Ϸ���ý���");
	HWND hwnd = GetHWnd();
	//����ָ�����ڵ�λ�úͳߴ�
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	switch (backGroundNum)
	{
	case 1:
		loadimage(&picture, L"picture1.jpg");
		break;
	case 2:
		loadimage(&picture, L"picture2.jpg");
		break;
	case 3:
		loadimage(&picture, L"picture3.jpg");
		break;
	case 4:
		loadimage(&picture, L"picture4.jpg");
		break;
	default:
		loadimage(&picture, L"picture1.jpg");
		break;
	}
	putimage(0, 0, &picture);
	TCHAR s[50] = L"�޸��ٶ�";
	TCHAR s1[50] = L"�޸ı���ͼƬ";
	TCHAR s2[50] = L"������������";
	TCHAR s3[50] = L"����";

	button(145, 20, 180, 50, s);
	button(145, 100, 180, 50, s1);
	button(145, 180, 180, 50, s2);
	button(145, 260, 180, 50, s3);
}
//�ַ���ת��Ϊ����
int str2int(string s)
{
	int d;
	stringstream ss;
	ss << s;
	ss >> setprecision(16) >> d;
	ss.clear();
	return d;
}

//LRESULT �ǳ��򷵻ص�Windows������ֵ�� ������������ض���Ϣ����Ӧ�� ��ֵ�ĺ���ȡ������Ϣ���롣 CALLBACK �Ǻ����ĵ���Լ����
LRESULT CALLBACK WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)hwndButton1)
		{
			// ���ȷ����ť
			char buffer[256];
			// ��ȡ������е��ı�
			GetWindowTextA(hwndEdit1, buffer, sizeof(buffer));
			int thespeed = str2int(buffer);
			// ��ֵ��Tetris������ݳ�Ա
			Tetris::speed = thespeed;
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
	return LRESULT();
}
void Tetris::drawSetSpeed()
{
	WNDCLASS wc1 = { 0 };
	wc1.lpfnWndProc = WndProc1;
	wc1.hInstance = GetModuleHandle(NULL);
	wc1.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc1.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc1.lpszClassName = L"Class";
	RegisterClass(&wc1);
	// ��ȡ����̨���ھ��
	HWND hwndConsole = GetConsoleWindow();
	// ���ؿ���̨����
	ShowWindow(hwndConsole, SW_HIDE);
	HWND hwnd = CreateWindow(
		L"Class",
		L"�޸��ٶ�",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 640,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	//HWND_TOP��ʾ���������ڶ�����SWP_NOSIZE��־��ʾ���ڵĴ�С�������仯��SWP_NOZORDER��־��ʾ���ڵ� Z �򲻷����仯��
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	wstring text = L"�ٶȵ�λΪ���룬����������������ǰ�ٶȣ�" + to_wstring(Tetris::speed);
	HWND hwndText = CreateWindow(
		L"static",
		//������ת��Ϊ���ϵ�
		text.c_str(),
		WS_VISIBLE | WS_CHILD,
		10, 50, 400, 20,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	
	hwndEdit1 = CreateWindow(
		L"edit",
		NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
		10, 10, 300, 30,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	hwndButton1 = CreateWindow(
		L"button",
		L"ȷ���ٶ�",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		320, 10, 100, 30,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Tetris::drawSetPicture()
{
	//��ʼ����ͼ���ڲ����ñ��������ı�
	SetWindowText(initgraph(480, 640), L"�޸ı���ͼƬ");
	HWND hwnd = GetHWnd();
	//����ָ�����ڵ�λ�úͳߴ�
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	loadimage(&picture, L"picture5.jpg");
	putimage(0, 0, &picture);
	setbkmode(TRANSPARENT);
	// �������������
	LOGFONT f;
	//��ȡ��ǰ������ʽ
	gettextstyle(&f);
	//ָ���߶����ݳ�Ա
	f.lfHeight = 30;
	//���ƾ����������Ƶ�lfFaceName
	wcscpy_s(f.lfFaceName, L"����");
	//ָ�����ֵ��������Ϊ�����������Ҫ
	f.lfQuality = DEFAULT_QUALITY;
	//���õ�ǰ������ʽ
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(100, 50, L"����ѡ�񱳾�ͼƬ");
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 0 && msg.x <= 120 && msg.y >= 0 && msg.y <= 640)
					backGroundNum = 1;
				if (msg.x >= 120 && msg.x <= 240 && msg.y >= 0 && msg.y <= 640)
					backGroundNum = 2;
				if (msg.x >= 240 && msg.x <= 360 && msg.y >= 0 && msg.y <= 640)
					backGroundNum = 3;
				if (msg.x >= 360 && msg.x <= 480 && msg.y >= 0 && msg.y <= 640)
					backGroundNum = 4;
				MessageBox(GetHWnd(), L"�޸ĳɹ���", L"��ʾ", MB_ICONWARNING);
				goto back1;
				break;
			default:
				break;
			}
		}
	}
back1:
	closegraph();
}

void Tetris::playMusic()
{
	if (replayMusic)
	{
		if(music==1)
			PlaySound(NULL, NULL, SND_FILENAME);
		if(music==2)
			PlaySound(TEXT("MusicEx_Welcome.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		if(music==3)
			PlaySound(TEXT("maliao.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		if(music==4)
			PlaySound(TEXT("tetris.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	replayMusic = 0;
}

void Tetris::drawSetMusic()
{
	//��ʼ����ͼ���ڲ����ñ��������ı�
	SetWindowText(initgraph(480, 640), L"�޸ı�������");
	HWND hwnd = GetHWnd();
	//����ָ�����ڵ�λ�úͳߴ�
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	loadimage(&picture, L"picture6.jpg");
	putimage(0, 0, &picture);
	setbkmode(TRANSPARENT);
	// �������������
	LOGFONT f;
	//��ȡ��ǰ������ʽ
	gettextstyle(&f);
	//ָ���߶����ݳ�Ա
	f.lfHeight = 30;
	//���ƾ����������Ƶ�lfFaceName
	wcscpy_s(f.lfFaceName, L"����");
	//ָ�����ֵ��������Ϊ�����������Ҫ
	f.lfQuality = DEFAULT_QUALITY;
	//���õ�ǰ������ʽ
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(100, 120, L"��");
	outtextxy(310, 120, L"������");
	outtextxy(70, 480, L"�����");
	outtextxy(320, 480, L"����");
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 0 && msg.x <= 240 && msg.y >= 0 && msg.y <= 320)
					music = 1;
				if (msg.x >= 240 && msg.x <= 480 && msg.y >= 0 && msg.y <= 320)
					music = 2;
				if (msg.x >= 0 && msg.x <= 240 && msg.y >= 320 && msg.y <= 640)
					music = 3;
				if (msg.x >= 240 && msg.x <= 480 && msg.y >= 320 && msg.y <= 640)
					music = 4;
				replayMusic = 1;
				MessageBox(GetHWnd(), L"�޸ĳɹ���", L"��ʾ", MB_ICONWARNING);
				goto back2;
				break;
			default:
				break;
			}
		}
	}
back2:
	closegraph();
}
