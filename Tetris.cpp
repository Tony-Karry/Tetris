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
	//初始化绘图窗口并设置标题栏的文本
	SetWindowText(initgraph(474, 640), L"俄罗斯方块_设置界面");
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
	TCHAR s[50] = L"开始新游戏";
	TCHAR s1[50] = L"继续存档游戏";
	TCHAR s2[50] = L"排行榜";
	TCHAR s3[50] = L"游戏设置";
	TCHAR s4[50] = L"游戏规则";

	button(145, 230, 180, 50, s);
	button(145, 310, 180, 50, s1);
	button(145, 390, 180, 50, s2);
	button(145, 470, 180, 50, s3);
	button(145, 550, 180, 50, s4);
	settextstyle(60, 0, L"黑体");
	settextcolor(BLACK);
	outtextxy(85, 100, L"俄罗斯方块");
}

void Tetris::button(int x, int y, int w, int h, TCHAR* text)const
{
	//指定图案填充和文字输出时的背景模式，TRANSPARENT表示背景是透明的
	setbkmode(TRANSPARENT);
	setfillcolor(LIGHTGRAY);
	fillroundrect(x, y, x + w, y + h, 10, 10);
	// 输出字符串（MBCS 字符集）
	settextstyle(30, 0, L"黑体");
	TCHAR s[50] = L"hello";

	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	outtextxy(tx, ty, text);
}

//LRESULT 是程序返回到Windows的整数值。 它包含程序对特定消息的响应。 此值的含义取决于消息代码。 CALLBACK 是函数的调用约定。
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)hwndButton)
		{
			// 点击确定按钮
			char buffer[256];
			// 获取输入框中的文本
			GetWindowTextA(hwndEdit, buffer, sizeof(buffer));
			// 赋值给Tetris类的数据成员
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
	// 获取控制台窗口句柄
	HWND hwndConsole = GetConsoleWindow();
	// 隐藏控制台窗口
	ShowWindow(hwndConsole, SW_HIDE);
	HWND hwnd = CreateWindow(
		L"MyClass",
		L"输入用户名",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 640,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	//HWND_TOP表示将窗口置于顶部，SWP_NOSIZE标志表示窗口的大小不发生变化，SWP_NOZORDER标志表示窗口的 Z 序不发生变化。
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
		L"确定用户名",
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
	//更改指定窗口的标题栏的文本。参数为：要更改其文本的窗口，新的标题文本。L告示编译器使用两个字节的 unicode 字符集
	//initgraph为初始化绘图窗口
	SetWindowText(initgraph(480, 640), L"俄罗斯方块_游戏界面");
	HWND hwnd = GetHWnd();
	//更改指定窗口的位置和尺寸
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//设置背景色
	setbkcolor(WHITE);
	//使用当前背景色清空绘图设备
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
	// 时钟开始时间、结束时间
	clock_t start = 0;
	clock_t end;
	//用于保存鼠标消息的结构体
	ExMessage msg;
	nextBlock.drawNextBlock();

	//开始批量绘图。执行后，任何绘图操作都将暂时不输出，直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出。
	BeginBatchDraw();
	while (true)
	{
		b.clearBlock();
		//清空矩形区域
		clearrectangle(20, 20, 270, 520);
		putimage(0, 0, &picture);
		nextBlock.drawNextBlock();
		drawMap();
		//peekmessage用于获取一个消息，并立即返回。EM_KEY表示按键消息。WM_KEYDOWN表示按键按下消息
		while ((peekmessage(&msg, EM_KEY) && msg.message == WM_KEYDOWN))
		{
			//vkcode表示按键的虚拟键码，仅当消息所属类别为 EX_KEY 时有效
			switch (msg.vkcode)
			{
				// 上键旋转
			case VK_UP:
				b.rotate();
				break;

				// 下键加速下落
			case VK_DOWN:
				if (b.move())
				{
					// 重新设置方块
					b.addMap();
					b = nextBlock;
					b.setPos();
					nextBlock.clearBlock();
					nextBlock.setPos(11, 2);
					nextBlock.randType();
					nextBlock.drawNextBlock();

					// 若方块刚刚生成却发生了碰撞，说明游戏结束
					if (b.checkCollision())
					{
						//MessageBox显示一个模式对话框，消息框返回一个整数值，该值指示用户单击的按钮。GetHWnd用于获取绘图窗口句柄
						PlaySound(TEXT("MusicEx_Lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
						replayMusic = 1;
						MessageBox(GetHWnd(), L"游戏结束！", L"提示", MB_ICONWARNING);
						
						goto exit;
					}
				}
				break;

				// 左键移动
			case VK_LEFT:
				b.move(1);
				break;

				// 右键移动
			case VK_RIGHT:
				b.move(2);
				break;

				//P键暂停
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
				MessageBox(GetHWnd(), L"存档成功！", L"提示", MB_ICONWARNING);
				goto exit;
				break;
				// Esc 键退出程序
			case VK_ESCAPE:
				goto exit;
				break;
			}
		}

		// 判断每过了speed方块下移一格
		end = clock();
		if (end - start >= Tetris::speed)
		{
			if (b.move())
			{
				// 重新设置方块
				b.addMap();
				b = nextBlock;
				b.setPos();
				nextBlock.clearBlock();
				nextBlock.setPos(11, 2);
				nextBlock.randType();
				nextBlock.drawNextBlock();

				// 如果方块刚刚生成却发生了碰撞，说明游戏结束
				if (b.checkCollision())
				{
					PlaySound(TEXT("MusicEx_Lose.wav"), NULL, SND_FILENAME | SND_ASYNC );
					MessageBox(GetHWnd(), L"游戏结束！", L"提示", MB_ICONWARNING);
					replayMusic = 1;
					goto exit;
				}
			}
			//start=现在时间，即重新计算时间
			start = clock();
			addGameTime();
		}
		b.drawNextBlock();
		clearLine();
		// 刷新缓冲区
		FlushBatchDraw();
		// Sleep函数可以使计算机程序（进程，任务或线程）进入休眠，使其在一段时间内处于非活动状态。每 50 毫秒接收一次按键
		Sleep(40);
	}
exit:
	//结束批量绘制，并执行未完成的绘制任务
	EndBatchDraw();
	closegraph();
}

void Tetris::drawMap()const
{
	//画无填充的圆角矩形。参数为：圆角矩形左部 x 坐标，顶部 y 坐标，右部 x 坐标，底部 y 坐标
	//roundrect(10, 10, 470, 430, 10, 10);
	//填充颜色
	setbkmode(TRANSPARENT);
	setfillcolor(DARKGRAY);
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			int left = 20 + BLOCK_WIDTH * i, top = 20 + BLOCK_WIDTH * j;
			//绘制矩形，该矩形使用当前笔轮廓，并使用当前画笔填充。参数为：矩形左上角的 x 坐标，左上角的 y 坐标，右下角的 x 坐标，右下角的 y 坐标
			rectangle(left, top, left + BLOCK_WIDTH, top + BLOCK_WIDTH);
			if (map[i][j])
			{
				//画有边框的填充矩形
				fillrectangle(left, top, left + BLOCK_WIDTH, top + BLOCK_WIDTH);
			}
		}
	}
}

void Tetris::drawMapMessage() const
{
	setbkmode(TRANSPARENT);
	// 定义字体的属性
	LOGFONT f;
	//获取当前文字样式
	gettextstyle(&f);
	//指定高度数据成员
	f.lfHeight = 30;
	//复制具体字体名称到lfFaceName
	wcscpy_s(f.lfFaceName, L"宋体");
	//指定文字的输出质量为输出质量不重要
	f.lfQuality = DEFAULT_QUALITY;
	//设置当前文字样式
	settextstyle(&f);
	settextcolor(BLACK);

	// 绘制 “下一方块” 提示
	outtextxy(295, 20, L"下一方块");
	rectangle(295, 70, 395, 170);

	// 绘制分数、已消行数、速度、暂停存档提示
	f.lfHeight = 25;
	settextstyle(&f);
	//实现定义的宽字符类型
	wchar_t str[10];
	wchar_t str1[10];
	wchar_t str2[10];
	//将数据写入指定的缓冲区。格式规范以%开头，u表示无符号整数参数。
	wsprintf(str, L"已消行数：%u", clearLineNum);
	outtextxy(295, 200, str);
	wsprintf(str1, L"得分：%u", score);
	outtextxy(295, 230, str1);
	wsprintf(str2, L"速度：%u", Tetris::speed);
	outtextxy(295, 260, str2);
	outtextxy(20, 550, L"P键暂停/继续，O键存档");


	//绘制存档按钮
	//TCHAR s[50] = L"存档游戏并返回";
	//button(230, 540, 220, 40, s);

	//绘制时间
	//wsprintf(str, L"时间：%u秒", gameTime / 1000);
	//outtextxy(230, 175, str);

	// 绘制 “俄罗斯方块”
	f.lfHeight = 40;
	settextstyle(&f);
	settextcolor(RED);
	//outtextxy(234, 250, L"俄罗斯");
	//outtextxy(250, 290, L"方块");
	//outtextxy(240, 330, L"Tetris");

	// 绘制 “作者”
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
	// 判断哪一行满行
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
		// 将上一行移至满行
		for (int j = line; j > 0; j--)
		{
			for (int i = 0; i < MAP_WIDTH; i++)
			{
				map[i][j] = map[i][j - 1];
			}
		}
		// 加游戏分数
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
	TCHAR s[50] = L"确定计入";
	TCHAR s1[50] = L"不计入";
	button(40, 470, 180, 50, s);
	button(260, 470, 180, 50, s1);
	settextstyle(50, 0, L"宋体");
	settextcolor(BLACK);
	outtextxy(20, 100, L"存档计入排行榜否？");
}

//用于比较得分
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
	//检索是否已经存在用户名
	if(outfile.tellg() != end)
	outfile.read((char*)&data, sizeof(data));
	while ((strcmp(data.Name, userName) != 0) && (outfile.tellg() != end))
		outfile.read((char*)&data, sizeof(data));
	//若存在且分数更高，则覆盖存入
	if (strcmp(data.Name, userName) == 0 && data.scores > score)
	{
		//获得tellg指向
		streampos pos = outfile.tellg();
		//设置seekp指向
		outfile.seekp(pos);
		outfile.seekp(-static_cast<streamoff>(sizeof(recordData)), ios::cur);
		
	}
	//若不存在，在最后存入
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
	//开始排序
	// 获取文件长度
	outfile1.seekg(0, ios::end);
	streampos length = outfile1.tellg();
	int count = length / sizeof(recordData);
	// 分配数组空间，并读取文件中所有数据
	recordData* data1 = new recordData[count];
	outfile1.seekg(0, ios::beg);
	outfile1.read(reinterpret_cast<char*>(data1), length);
	// 按照 scores 排序
	sort(data1, data1 + count, cmp);
	// 将排好序的数据写回到文件中
	outfile1.seekp(0, ios::beg);
	outfile1.write(reinterpret_cast<const char*>(data1), length);
	// 释放数组空间，并关闭文件
	delete[] data1;
	outfile1.close();
	
}

void Tetris::showRecord()
{
	SetWindowText(initgraph(480, 640), L"排行榜");
	HWND hwnd = GetHWnd();
	//更改指定窗口的位置和尺寸
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ifstream infile("data.txt", ios::in | ios::binary);
	// 获取文件长度
	infile.seekg(0, ios::end);
	streampos length = infile.tellg();
	int count = length / sizeof(recordData);
	// 分配数组空间，并读取文件中所有数据
	recordData* data = new recordData[count];
	infile.seekg(0, ios::beg);
	infile.read(reinterpret_cast<char*>(data), length);
	// 在窗口中显示排行榜
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
		wsprintf(buffer,L"%d. %S: %d分", i + 1,data[i].Name, data[i].scores);
		outtextxy(20, 20 + i * 20, buffer);
	}
	infile.close();
	//绘制清空、返回按钮
	TCHAR s[50] = L"返回";
	TCHAR s1[50] = L"清空";
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
	// 释放数组空间，并关闭文件
	delete[] data;
	infile.close();
	closegraph();
}

void Tetris::showRule()
{
	SetWindowText(initgraph(480, 640), L"游戏规则");
	HWND hwnd = GetHWnd();
	//更改指定窗口的位置和尺寸
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
	// 定义字体的属性
	LOGFONT f;
	//获取当前文字样式
	gettextstyle(&f);
	//指定高度数据成员
	f.lfHeight = 30;
	//复制具体字体名称到lfFaceName
	wcscpy_s(f.lfFaceName, L"宋体");
	//指定文字的输出质量为输出质量不重要
	f.lfQuality = DEFAULT_QUALITY;
	//设置当前文字样式
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(150, 30, L"游戏规则");
	settextstyle(16, 0, L"宋体");	
	outtextxy(5, 80, L"使用键盘←→移动，↑变形，↓加速下落，O键存档，P键暂停");
	outtextxy(5, 100, L"由小方块组成的不同形状的板块陆续从屏幕上方落下来，玩家通");
	outtextxy(5, 120, L"过调整板块的位置和方向，使它们在屏幕底部拼出完整的一条或");
	outtextxy(5, 140, L"几条。这些完整的横条会随即消失，给新落下来的板块腾出空间，");
	outtextxy(5, 160, L"与此同时，玩家得到每消一行10分奖励。没有被消除掉的方块不");
	outtextxy(5, 180, L"断堆积起来，一旦堆到屏幕顶端，玩家便告输，游戏结束。");
	//绘制返回按钮
	TCHAR s[50] = L"返回";
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

//记录顺序：方块盘宽、长、方块宽、已消行数、得分、速度、方块盘数据
void Tetris::store()
{
	int wide = MAP_WIDTH, height = MAP_HEIGHT, blockwide = BLOCK_WIDTH;
	//构造文件名
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
	//构造文件名
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
	//初始化绘图窗口并设置标题栏的文本
	SetWindowText(initgraph(480, 640), L"俄罗斯方块_游戏设置界面");
	HWND hwnd = GetHWnd();
	//更改指定窗口的位置和尺寸
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
	TCHAR s[50] = L"修改速度";
	TCHAR s1[50] = L"修改背景图片";
	TCHAR s2[50] = L"更换背景音乐";
	TCHAR s3[50] = L"返回";

	button(145, 20, 180, 50, s);
	button(145, 100, 180, 50, s1);
	button(145, 180, 180, 50, s2);
	button(145, 260, 180, 50, s3);
}
//字符串转换为整型
int str2int(string s)
{
	int d;
	stringstream ss;
	ss << s;
	ss >> setprecision(16) >> d;
	ss.clear();
	return d;
}

//LRESULT 是程序返回到Windows的整数值。 它包含程序对特定消息的响应。 此值的含义取决于消息代码。 CALLBACK 是函数的调用约定。
LRESULT CALLBACK WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)hwndButton1)
		{
			// 点击确定按钮
			char buffer[256];
			// 获取输入框中的文本
			GetWindowTextA(hwndEdit1, buffer, sizeof(buffer));
			int thespeed = str2int(buffer);
			// 赋值给Tetris类的数据成员
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
	// 获取控制台窗口句柄
	HWND hwndConsole = GetConsoleWindow();
	// 隐藏控制台窗口
	ShowWindow(hwndConsole, SW_HIDE);
	HWND hwnd = CreateWindow(
		L"Class",
		L"修改速度",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 640,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	//HWND_TOP表示将窗口置于顶部，SWP_NOSIZE标志表示窗口的大小不发生变化，SWP_NOZORDER标志表示窗口的 Z 序不发生变化。
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	wstring text = L"速度单位为毫秒，请输入正整数，当前速度：" + to_wstring(Tetris::speed);
	HWND hwndText = CreateWindow(
		L"static",
		//将类型转换为符合的
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
		L"确定速度",
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
	//初始化绘图窗口并设置标题栏的文本
	SetWindowText(initgraph(480, 640), L"修改背景图片");
	HWND hwnd = GetHWnd();
	//更改指定窗口的位置和尺寸
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	loadimage(&picture, L"picture5.jpg");
	putimage(0, 0, &picture);
	setbkmode(TRANSPARENT);
	// 定义字体的属性
	LOGFONT f;
	//获取当前文字样式
	gettextstyle(&f);
	//指定高度数据成员
	f.lfHeight = 30;
	//复制具体字体名称到lfFaceName
	wcscpy_s(f.lfFaceName, L"宋体");
	//指定文字的输出质量为输出质量不重要
	f.lfQuality = DEFAULT_QUALITY;
	//设置当前文字样式
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(100, 50, L"请点击选择背景图片");
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
				MessageBox(GetHWnd(), L"修改成功！", L"提示", MB_ICONWARNING);
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
	//初始化绘图窗口并设置标题栏的文本
	SetWindowText(initgraph(480, 640), L"修改背景音乐");
	HWND hwnd = GetHWnd();
	//更改指定窗口的位置和尺寸
	MoveWindow(hwnd, 0, 0, 480, 640, TRUE);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	IMAGE picture;
	loadimage(&picture, L"picture6.jpg");
	putimage(0, 0, &picture);
	setbkmode(TRANSPARENT);
	// 定义字体的属性
	LOGFONT f;
	//获取当前文字样式
	gettextstyle(&f);
	//指定高度数据成员
	f.lfHeight = 30;
	//复制具体字体名称到lfFaceName
	wcscpy_s(f.lfFaceName, L"宋体");
	//指定文字的输出质量为输出质量不重要
	f.lfQuality = DEFAULT_QUALITY;
	//设置当前文字样式
	settextstyle(&f);
	settextcolor(BLACK);
	outtextxy(100, 120, L"无");
	outtextxy(310, 120, L"斗地主");
	outtextxy(70, 480, L"马里奥");
	outtextxy(320, 480, L"经典");
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
				MessageBox(GetHWnd(), L"修改成功！", L"提示", MB_ICONWARNING);
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
