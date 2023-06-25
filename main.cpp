#include <easyx.h>
#include <ctime>
#include "graphics.h"
#include <windows.h>
#include "Tetris.h"
#include "Block.h"
using namespace std;
int main()
{
	Tetris game;
begin:
	game.playMusic();
	while (1) 
	{
		game.drawSetting();
		ExMessage msg;
		while (true)
		{
			if (peekmessage(&msg, EM_MOUSE))
			{
				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					//开始新游戏
					if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 230 && msg.y <= 230 + 50)
					{
						closegraph();
						game.drawSetUser();
						game.playNewGame(0);
						game.drawRecord();
						while (true)
						{
							if (peekmessage(&msg, EM_MOUSE))
							{
								switch (msg.message)
								{
								case WM_LBUTTONDOWN:
									if (msg.x >= 40 && msg.x <= 40 + 180 && msg.y >= 470 && msg.y <= 470 + 50)
									{
										game.record();
										game.cleanMap();
										goto begin;
									}
									if (msg.x >= 260 && msg.x <= 260 + 180 && msg.y >= 470 && msg.y <= 470 + 50)
									{
										game.cleanMap();
										goto begin;
									}
									break;
								default:
									break;
								}
							}
						}
					}
					//继续存档游戏
					if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 310 && msg.y <= 310 + 50)
					{
						closegraph();
						game.drawSetUser();
						game.read();
						game.playNewGame(1);
						game.drawRecord();
						while (true)
						{
							if (peekmessage(&msg, EM_MOUSE))
							{
								switch (msg.message)
								{
								case WM_LBUTTONDOWN:
									if (msg.x >= 40 && msg.x <= 40 + 180 && msg.y >= 470 && msg.y <= 470 + 50)
									{
										game.record();
										game.cleanMap();
										goto begin;
									}
									if (msg.x >= 260 && msg.x <= 260 + 180 && msg.y >= 470 && msg.y <= 470 + 50)
									{
										game.cleanMap();
										goto begin;
									}
									break;
								default:
									break;
								}
							}
						}
					}
					//排行榜
					if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 390 && msg.y <= 390 + 50)
					{
						game.showRecord();
						goto begin;
					}
					//游戏设置
					if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 470 && msg.y <= 470 + 50)
					{
						game.drawGameSetting();
						while (true)
						{
							if (peekmessage(&msg, EM_MOUSE))
							{
								switch (msg.message)
								{
								case WM_LBUTTONDOWN:
									if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 20 && msg.y <= 20 + 50)
									{
										closegraph();
										game.drawSetSpeed();
										goto begin;
									}
									if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 100 && msg.y <= 100 + 50)
									{
										closegraph();
										game.drawSetPicture();
										goto begin;
									}
									if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 180 && msg.y <= 180 + 50)
									{
										closegraph();
										game.drawSetMusic();
										goto begin;
									}
									if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 260 && msg.y <= 260 + 50)
										goto begin;
									break;
								default:
									break;
								}
							}
						}
					}
					if (msg.x >= 145 && msg.x <= 145 + 180 && msg.y >= 550 && msg.y <= 550 + 50)
					{
						game.showRule();
						goto begin;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	return 0;
}
