#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <conio.h>
#include <chrono>

#include "SpriteSheet.h"

#pragma comment(lib, "winmm.lib") // Support for playing audio

using namespace std;

SpriteSheet sprites;

void input();
void logic();

int f_count = 9;
void animatePacman(int f_count);

bool keyIsDown(char key, bool pressed = true, bool held = true);
double getTime(); // Returns timer with nanosecond precision
double getTimeSince(double); // Returns timer with nanosecond precision since a point in time
double wait(double); // Pauses for the given amount of seconds, returns how much extra time was waited
void ShowConsoleCursor(bool);

void rotateTile(int sprite[8][8], int, int, int);
void drawMap();


int Xshift = 40, Yshift = 40;
bool EXIT_PROGRAM = false;

int pacX = sprites.draw.playerX / 8, pacY = sprites.draw.playerY / 8; // Position of Pacman on the map array.
const int mapWidth = 29, mapHeight = 31;
// Collision point values

double FPS = 1.0 / 90.0;
double timer = 0, dt = 0;

// MAP DRAWING GUIDE
// a = TOP LEFT CORNER, b = TOP RIGHT CORNER, c = BOTTOM LEFT CORNER, d = BOTTOM RIGHT CORNER
// A = LOWER TOP LEFT CORNER, B = LOWER TOP RIGHT CORNER, C = LOWER BOTTOM LEFT CORNER, D = LOWER BOTTOM RIGHT CORNER
// t = TOP WALL, f = BOTTOM WALL, l = LEFT WALL, r = RIGHT WALL
char map[mapHeight][mapWidth] = {
	"attttttttttttbattttttttttttb",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"cffffB                Affffd",
	"     r                l     ",
	"     r                l     ",
	"     r                l     ",
	"tttttD                Cttttt",
	"                            ",
	"fffffB                Afffff",
	"     r                l     ",
	"     r                l     ",
	"     r                l     ",
	"attttD                Cttttb",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"c                          d",
	"a                          b",
	"l                          r",
	"l                          r",
	"l                          r",
	"l                          r",
	"cffffffffffffffffffffffffffd"

};

int main() {
	int gameWidth = 69, gameHeight = 21;
	system(("MODE " + to_string(gameWidth) + ", " + to_string(gameHeight)).c_str());
	ShowConsoleCursor(false);

	SelectObject(sprites.draw.hdc, sprites.draw.outlinePen);
	SelectObject(sprites.draw.hdc, sprites.draw.blackBrush);

	// PlaySound("Sounds\\pacman_beginning.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);

	/*
	HDC Animation2 = CreateCompatibleDC(NULL);
	HBITMAP bmpF2 = (HBITMAP)LoadImage(NULL, _T("PacMan_F2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(Animation2, bmpF2);
	while (true)
		BitBlt(hdc, 300, 400, 16, 16, Animation2, 0, 0, SRCCOPY);
	*/

	drawMap();
	while ((timer += (dt = FPS + wait(FPS))) && !EXIT_PROGRAM) {
		sprites.draw.drawClearEntity(sprites.draw.lastPlayerInput);
		animatePacman(f_count);
		drawMap();
		input();
		logic();
	}
	return 0;
}

void input() {
	if (sprites.draw.playerInput != DrawAndRotate::NONE && sprites.draw.playerInput != DrawAndRotate::START)
		sprites.draw.lastPlayerInput = sprites.draw.playerInput;
	sprites.draw.playerInput = DrawAndRotate::NONE;

	char enterKey = 13;

	if (keyIsDown(enterKey, true, false))
		sprites.draw.playerInput = DrawAndRotate::START;

	if (keyIsDown('A') && !keyIsDown('D')) {
		sprites.draw.playerInput = DrawAndRotate::LEFT;
	}
	else if (keyIsDown('D') && !keyIsDown('A')) {
		sprites.draw.playerInput = DrawAndRotate::RIGHT;
	}
	else if (keyIsDown('W') && !keyIsDown('S')) {
		sprites.draw.playerInput = DrawAndRotate::UP;
	}
	else if (keyIsDown('S') && !keyIsDown('W')) {
		sprites.draw.playerInput = DrawAndRotate::DOWN;
	}

	if (sprites.draw.playerInput != DrawAndRotate::NONE)
		sprites.draw.lastPlayerInput = sprites.draw.playerInput;
}

void logic() {
	f_count++;
	if (f_count == 10)
		f_count = 0;

	switch (sprites.draw.lastPlayerInput) {
	case DrawAndRotate::LEFT:
		if (GetPixel(sprites.draw.hdc, sprites.draw.playerX - 2, sprites.draw.playerY + 8) == sprites.draw.black && GetPixel(sprites.draw.hdc, sprites.draw.playerX - 1, sprites.draw.playerY + 8) == sprites.draw.black)
			sprites.draw.playerX -= 2;
		break;
	case DrawAndRotate::RIGHT:
		if (GetPixel(sprites.draw.hdc, sprites.draw.playerX + 16, sprites.draw.playerY + 8) == sprites.draw.black && GetPixel(sprites.draw.hdc, sprites.draw.playerX + 17, sprites.draw.playerY + 8) == sprites.draw.black)
			sprites.draw.playerX += 2;
		break;
	case DrawAndRotate::UP:
		if (GetPixel(sprites.draw.hdc, sprites.draw.playerX + 8, sprites.draw.playerY - 2) == sprites.draw.black && GetPixel(sprites.draw.hdc, sprites.draw.playerX + 8, sprites.draw.playerY - 1) == sprites.draw.black)
			sprites.draw.playerY -= 2;
		break;
	case DrawAndRotate::DOWN:
		if (GetPixel(sprites.draw.hdc, sprites.draw.playerX + 8, sprites.draw.playerY + 16) == sprites.draw.black && GetPixel(sprites.draw.hdc, sprites.draw.playerX + 8, sprites.draw.playerY + 17) == sprites.draw.black)
			sprites.draw.playerY += 2;
		break;
	case DrawAndRotate::START:
		EXIT_PROGRAM = true;
		break;
	}
}

void rotateTile(int sprite[8][8], int deg, int spriteX, int spriteY) {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			switch (deg) {
			case 0:
				if (sprite[y][x] == 2)
					SetPixelV(sprites.draw.hdc, spriteX + x, spriteY + y, sprites.draw.blue);
				break;
			case 90:
				if (sprite[y][x] == 2)
					SetPixelV(sprites.draw.hdc, spriteX + y, spriteY + x, sprites.draw.blue);
				break;
			case 180:
				if (sprite[y][x] == 2) {
					if (x < 4)
						SetPixelV(sprites.draw.hdc, spriteX + x + ((4 - x) * 2), spriteY + y, sprites.draw.blue);
					if (x >= 4)
						SetPixelV(sprites.draw.hdc, spriteX + x - ((x - 4) * 2), spriteY + y, sprites.draw.blue);
				}
				break;
			case 270:
				if (sprite[y][x] == 2) {
					if (y < 4)
						SetPixelV(sprites.draw.hdc, spriteX + x, spriteY + y + ((4 - y) * 2), sprites.draw.blue);
					if (y >= 4)
						SetPixelV(sprites.draw.hdc, spriteX + x, spriteY + y - ((y - 4) * 2), sprites.draw.blue);
				}
				break;
			}
		}
	}
}

void drawMap() {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			switch (map[y][x]) {
			case 'f':
				SelectObject(sprites.draw.hdc, sprites.draw.blueOutlinePen);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift, (y * 8) + Yshift + 4, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 8, (y * 8) + Yshift + 4);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift, (y * 8) + Yshift + 7, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 8, (y * 8) + Yshift + 7);
				SelectObject(sprites.draw.hdc, sprites.draw.outlinePen);
				//rotateTile(wall, 90, (x * 8) + Xshift, (y * 8) + Yshift);
				break;
			case 't':
				SelectObject(sprites.draw.hdc, sprites.draw.blueOutlinePen);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift, (y * 8) + Yshift + 4, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 8, (y * 8) + Yshift + 4);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift, (y * 8) + Yshift + 1, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 8, (y * 8) + Yshift + 1);
				SelectObject(sprites.draw.hdc, sprites.draw.outlinePen);
				//rotateTile(wall, 4, (x * 8) + Xshift, (y * 8) + Yshift);
				break;
			case 'l':
				SelectObject(sprites.draw.hdc, sprites.draw.blueOutlinePen);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift + 4, (y * 8) + Yshift + 8, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 4, (y * 8) + Yshift);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift + 1, (y * 8) + Yshift + 8, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 1, (y * 8) + Yshift);
				SelectObject(sprites.draw.hdc, sprites.draw.outlinePen);
				//rotateTile(wall, 180, (x * 8) + Xshift, (y * 8) + Yshift);
				break;
			case 'r':
				SelectObject(sprites.draw.hdc, sprites.draw.blueOutlinePen);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift + 4, (y * 8) + Yshift + 8, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 4, (y * 8) + Yshift);
				MoveToEx(sprites.draw.hdc, (x * 8) + Xshift + 7, (y * 8) + Yshift + 8, NULL);
				LineTo(sprites.draw.hdc, (x * 8) + Xshift + 7, (y * 8) + Yshift);
				SelectObject(sprites.draw.hdc, sprites.draw.outlinePen);
				//rotateTile(wall, 0, (x * 8) + Xshift, (y * 8) + Yshift);
				break;
			case 'a':
				rotateTile(sprites.wall_corner, 270, (x * 8) + Xshift + 1, (y * 8) + Yshift);
				break;
			case 'b':
				rotateTile(sprites.wall_corner, 90, (x * 8) + Xshift, (y * 8) + Yshift + 1);
				break;
			case 'c':
				rotateTile(sprites.wall_corner, 0, (x * 8) + Xshift + 1, (y * 8) + Yshift);
				break;
			case 'd':
				rotateTile(sprites.wall_corner, 180, (x * 8) - 1 + Xshift, (y * 8) + Yshift);
				break;
			case 'A':
				rotateTile(sprites.wall_corner, 270, (x * 8) + Xshift + 1, (y * 8) + Yshift + 3);
				break;
			case 'B':
				rotateTile(sprites.wall_corner, 90, (x * 8) + Xshift, (y * 8) + Yshift + 4);
				break;
			case 'C':
				rotateTile(sprites.wall_corner, 0, (x * 8) + Xshift + 1, (y * 8) + Yshift - 3);
				break;
			case 'D':
				rotateTile(sprites.wall_corner, 180, (x * 8) - 1 + Xshift, (y * 8) + Yshift - 3);
				break;
			}
		}
	}
}

void animatePacman(int f_count) {
	switch (f_count) {
	case 1:
	case 2:
	case 3:
		sprites.draw.drawEntity(sprites.PacMan_F1);
		break;
	case 4:
	case 5:
	case 6:
		sprites.draw.drawEntity(sprites.PacMan_F2);
		break;
	case 7:
	case 8:
	case 9:
		sprites.draw.drawEntity(sprites.PacMan_F3);
		break;
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
