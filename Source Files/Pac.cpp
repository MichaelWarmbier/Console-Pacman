#include <iostream>
#include <windows.h>
#include <conio.h>
HDC hdc = GetDC(GetConsoleWindow());
using namespace std;
enum input {UP, DOWN, LEFT, RIGHT, START, NONE};
input playerInput = NONE, lastPlayerInput = NONE;
int f_count = 0;
void drawEntity(int sprite[16][16]), input(), logic();
COLORREF yellow = RGB(255,255,0);
int playerX = 40, playerY = 40;
int PacMan_F1[16][16] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
int PacMan_F2[16][16] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
int PacMan_F3[16][16] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
int main() {
	while (true) {
		if (f_count == 1)
			drawEntity(PacMan_F1);
		if (f_count == 2)
			drawEntity(PacMan_F2);
		if (f_count == 0)
			drawEntity(PacMan_F3);
		input();
		logic();
		Sleep(50);
		system("CLS");
	}
	return 0;
}
void input() {
	if (playerInput != NONE && playerInput != START)
		lastPlayerInput = playerInput;
	playerInput = NONE;
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':
			playerInput = LEFT;
			break;
		case 'd':
			playerInput = RIGHT;
			break;
		case 'w':
			playerInput = UP;
			break;
		case 's':
			playerInput = DOWN;
			break;
		case 13:
			playerInput = START;
		}
	}
	if (playerInput != NONE && playerInput != START)
		lastPlayerInput = playerInput;
}
void logic() {
	if (f_count == 2)
		f_count = 0;
	else
		f_count ++;
	switch (playerInput) {
	case LEFT:
		playerX -= 5;
		break;
	case RIGHT:
		playerX += 5;
		break;
	case UP:
		playerY -= 5;
		break;
	case DOWN:
		playerY += 5;
		break;
	}

	
}
void drawEntity(int sprite[16][16]) {
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 16; x++) {
			switch (lastPlayerInput) {
			case RIGHT:
			case NONE:
				if (sprite[y][x] == 1)
					SetPixel(hdc, playerX + x, playerY + y, yellow);
				break;
			case DOWN:
				if (sprite[y][x] == 1)
					SetPixel(hdc, playerX + y, playerY + x, yellow);
				break;
			case LEFT:
				if (sprite[y][x] == 1) {
					if (x < 8)
						SetPixel(hdc, playerX + x + ((8 - x) * 2), playerY + y, yellow);
					if (x >= 8)
						SetPixel(hdc, playerX + x - ((x - 8) * 2), playerY + y, yellow);
				}
				break;
			case UP:
				if (sprite[y][x] == 1) {
					if (sprite[y][x] == 1) {
						if (x < 8)
							SetPixel(hdc, playerX + y + ((8 - y) * 2), playerY + x + ((8 - x) * 2), yellow);
						if (x >= 8)
							SetPixel(hdc, playerX + y - ((y - 8) * 2), playerY + x - ((x - 8) * 2), yellow);
					}

				}
			}
		}
	}
}
