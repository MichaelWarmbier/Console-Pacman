
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>

using namespace std;

HDC hdc = GetDC(GetConsoleWindow());

void drawEntity(int sprite[16][16]), input(), logic();
bool keyIsDown(char key, bool pressed = true, bool held = true);
double getTime(); //Returns timer with nanosecond precision
double getTimeSince(double); //Returns timer with nanosecond precision since a point in time
double wait(double); //Pauses for the given amount of seconds, returns how much extra time was waited

enum keyboardInput {UP, DOWN, LEFT, RIGHT, START, NONE};
keyboardInput playerInput = NONE, lastPlayerInput = NONE;
int f_count = 0;

COLORREF yellow = RGB(255, 255, 0), COLORREF blue = RGB(0, 0, 255);
int playerX = 40, playerY = 40;

double FPS = 1.0 / 60.0;
double timer = 0, dt = 0;

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
int wall[16][16] = {
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
};
int main() {
	while (timer += (dt = FPS + wait(FPS))) {
		if (f_count == 1)
			drawEntity(PacMan_F1);
		if (f_count == 2)
			drawEntity(PacMan_F2);
		if (f_count == 0)
			drawEntity(PacMan_F3);
		input();
		logic();
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

bool keyIsDown(char key, bool pressed, bool held) {
	int keyState = GetAsyncKeyState(static_cast<int>(key));
	return (pressed && (keyState & 1)) || (held && (keyState & 0xA000));
}

double getTime() {
	return chrono::time_point_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now()).time_since_epoch().count() / 1e9;
}

double getTimeSince(double startTime) {
	return chrono::time_point_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now()).time_since_epoch().count() / 1e9 - startTime;
}

double wait(double waitTime) {
	double startTime = getTime();

	while (waitTime > getTimeSince(startTime)) {}

	return getTimeSince(startTime + waitTime);
}
