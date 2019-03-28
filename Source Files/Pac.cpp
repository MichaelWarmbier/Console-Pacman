#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <chrono>

#pragma comment(lib, "winmm.lib") // Support for playing audio

using namespace std;

HDC hdc = GetDC(GetConsoleWindow());

enum keyboardInput { UP, DOWN, LEFT, RIGHT, START, NONE };
keyboardInput playerInput = NONE, lastPlayerInput = NONE;

void drawEntity(int sprite[16][16]);
void drawClearEntity(keyboardInput playerInput);
void input();
void logic();
bool keyIsDown(char key, bool pressed = true, bool held = true);
double getTime(); //Returns timer with nanosecond precision
double getTimeSince(double); //Returns timer with nanosecond precision since a point in time
double wait(double); //Pauses for the given amount of seconds, returns how much extra time was waited
void ShowConsoleCursor(bool);
void rotate(int sprite[16][16], int);
void rotateTile(int sprite[8][8], int, int, int);
void drawMap();

int f_count = 0;
int Xshift = 40, Yshift = 40;
bool EXIT_PROGRAM = false;

COLORREF yellow = RGB(255, 255, 0);
COLORREF blue = RGB(0, 0, 255);
COLORREF black = RGB(12, 12, 12);

int playerX = 40, playerY = 40;
const int mapWidth = 12, mapHeight = 10;

double FPS = 1.0 / 30.0;
double timer = 0, dt = 0;
// MAP DRAWING GUIDE
// a = TOP LEFT CORNER, b = TOP RIGHT CORNER, c = BOTTOM LEFT CORNER, d = BOTTOM RIGHT CORNER
// t = TOP WALL, f = BOTTOM WALL, l = LEFT WALL, r = RIGHT WALL
char map[mapHeight][mapWidth]{
	"atttttttttb",
	"l         r",
	"l         r",
	"l         r",
	"l         r",
	"l         r",
	"l         r",
	"l         r",
	"l         r",
	"cfffffffffd"
};
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

int wall[8][8] = {
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
	0,0,0,0,2,0,0,2,
};

int wall_corner[8][8] = {
	2,0,0,2,0,0,0,0,
	2,0,0,2,0,0,0,0,
	2,0,0,2,0,0,0,0,
	2,0,0,0,2,0,0,0,
	0,2,0,0,0,2,2,2,
	0,2,0,0,0,0,0,0,
	0,0,2,2,0,0,0,0,
	0,0,0,0,2,2,2,2,
};
int main() {
	int gameWidth = 69, gameHeight = 21;
	system(("MODE " + to_string(gameWidth) + ", " + to_string(gameHeight)).c_str());
	ShowConsoleCursor(false);

	// PlaySound("Sounds\\pacman_beginning.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
	while ((timer += (dt = FPS + wait(FPS))) && !EXIT_PROGRAM) {
		drawClearEntity(playerInput);
		if (f_count == 1 || f_count == 2)
			drawEntity(PacMan_F1);
		if (f_count == 3 || f_count == 4)
			drawEntity(PacMan_F2);
		if (f_count == 5 || f_count == 0)
			drawEntity(PacMan_F3);
		drawMap();
		input();
		logic();
	}
	return 0;
}

void input() {
	if (playerInput != NONE && playerInput != START)
		lastPlayerInput = playerInput;
	playerInput = NONE;

	char enterKey = 13;

	if (keyIsDown(enterKey, true, false))
		playerInput = START;

	if (keyIsDown('A') && !keyIsDown('D'))
		playerInput = LEFT;
	else if (keyIsDown('D') && !keyIsDown('A'))
		playerInput = RIGHT;
	else if (keyIsDown('W') && !keyIsDown('S'))
		playerInput = UP;
	else if (keyIsDown('S') && !keyIsDown('W'))
		playerInput = DOWN;

	if (playerInput != NONE && playerInput != START)
		lastPlayerInput = playerInput;
}

void logic() {
	if (f_count == 5)
		f_count = 0;
	else
		f_count++;
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
	case START:
		EXIT_PROGRAM = true;
		break;
	}
}

void rotate(int sprite[16][16], int deg) {
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 16; x++) {
			switch (deg) {
			case 0:
				if (sprite[y][x] == 1)
					SetPixel(hdc, playerX + x, playerY + y, yellow);
				break;
			case 90:
				if (sprite[y][x] == 1)
					SetPixel(hdc, playerX + y, playerY + x, yellow);
				break;
			case 180:
				if (sprite[y][x] == 1) {
					if (x < 8)
						SetPixel(hdc, playerX + x + ((8 - x) * 2), playerY + y, yellow);
					if (x >= 8)
						SetPixel(hdc, playerX + x - ((x - 8) * 2), playerY + y, yellow);
				}
				break;
			case 270:
				if (sprite[y][x] == 1) {
					if (x < 8)
						SetPixel(hdc, playerX + y + ((8 - y) * 2), playerY + x + ((8 - x) * 2), yellow);
					if (x >= 8)
						SetPixel(hdc, playerX + y - ((y - 8) * 2), playerY + x - ((x - 8) * 2), yellow);
				}
				break;
			}
		}
	}
}

void rotateTile(int sprite[8][8], int deg, int spriteX, int spriteY) {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			switch (deg) {
			case 0:
				if (sprite[y][x] == 2)
					SetPixel(hdc, spriteX + x, spriteY + y, blue);
				break;
			case 90:
				if (sprite[y][x] == 2)
					SetPixel(hdc, spriteX + y, spriteY + x, blue);
				break;
			case 180:
				if (sprite[y][x] == 2) {
					if (x < 4)
						SetPixel(hdc, spriteX + x + ((4 - x) * 2), spriteY + y, blue);
					if (x >= 4)
						SetPixel(hdc, spriteX + x - ((x - 4) * 2), spriteY + y, blue);
				}
				break;
			case 270:
				if (sprite[y][x] == 2) {
					if (x < 4)
						SetPixel(hdc, spriteX + y + ((4 - y) * 2), spriteY + x + ((4 - x) * 2), blue);
					if (x >= 4)
						SetPixel(hdc, spriteX + y - ((y - 4) * 2), spriteY + x - ((x - 4) * 2), blue);
				}
				break;
			}
		}
	}
}

void drawEntity(int sprite[16][16]) {
	switch (lastPlayerInput) {
	case RIGHT:
	case NONE:
		rotate(sprite, 0);
		break;
	case DOWN:
		rotate(sprite, 90);
		break;
	case LEFT:
		rotate(sprite, 180);
		break;
	case UP:
		rotate(sprite, 270);
		break;
	}
}

void drawMap() {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			// CORNERS
			if (map[y][x] == 'a') // Top Left Corner
				rotateTile(wall_corner, 270, (x * 8) + Xshift, (y * 8) + Yshift);
			if (map[y][x] == 'b') // Top Right Corner
				rotateTile(wall_corner, 90, (x * 8) + Xshift, (y * 8) + Yshift + 1);
			if (map[y][x] == 'c') // Bottom Left Corner
				rotateTile(wall_corner, 0, (x * 8) + Xshift + 1, (y * 8) + Yshift);
			if (map[y][x] == 'd') // Bottom Right Corner
				rotateTile(wall_corner, 180, (x * 8) - 1 + Xshift, (y * 8) + Yshift);
			// WALLS
			if (map[y][x] == 'f') // Bottom Wall
				rotateTile(wall, 90, (x * 8) + Xshift, (y * 8) + Yshift);
			if (map[y][x] == 't') // Top wall
				rotateTile(wall, 270, (x * 8) + Xshift, (y * 8) + Yshift);
			if (map[y][x] == 'l') // Left Wall
				rotateTile(wall, 180, (x * 8) + Xshift, (y * 8) + Yshift);
			if (map[y][x] == 'r') // Right wall
				rotateTile(wall, 0, (x * 8) + Xshift, (y * 8) + Yshift);
		}
	}
}

void drawClearEntity(keyboardInput playerInput) {
	int xOffset = 0, yOffset = 0;
	if (playerInput == LEFT)
		xOffset = 5;
	else if (playerInput == RIGHT)
		xOffset = -5;
	else if (playerInput == UP)
		yOffset = 5;
	else if (playerInput == DOWN)
		yOffset = -5;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			SetPixel(hdc, playerX + xOffset + j, playerY + yOffset + i, black);
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

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // Set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
