#pragma once
#include "DataStructures.h"

void GameDraw();
void GameInput();
void GameLogic();

bool KeyIsDown(char key, bool pressed = true, bool held = true);
double getTime();
double getTimeSince(double);
double wait(double);

void DrawMap(int X_pos, int Y_pos);
COLORREF GetColor(int color_data);

TileSprites Game;
Pacman Pac;

///////////////////////////////////////////////////
void GameDraw() {
	DrawMap(20, 20);
}
void GameInput() {
	if (KeyIsDown('W', true, false))
		game_input = UP;
	else if (KeyIsDown('S', true, false))
		game_input = DOWN;
	else if (KeyIsDown('A', true, false))
		game_input = LEFT;
	else if (KeyIsDown('D', true, false))
		game_input = RIGHT;
}
///////////////////////////////////////////////////
void TileSprites::DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position) {
	for (int y = 0; y < spriteSize; y++) {
		for (int x = 0; x < spriteSize; x++) {
			if (rotation_value != 1 && rotation_value != 2 && rotation_value != 3 && rotation_value != 4 && rotation_value != 5)
				SetPixel(hdc, X_position + x, Y_position + y, RGB(255, 20, 147));
			else if (sprite[y][x] == 0)
				continue;
			else if (rotation_value == 1)
				SetPixel(hdc, X_position + x, Y_position + y, GetColor(sprite[y][x]));
			else if (rotation_value == 2)
				SetPixel(hdc, X_position + y, Y_position + x, GetColor(sprite[y][x]));
			else if (rotation_value == 3) {
				if (y <= spriteSize / 2)
					SetPixel(hdc, X_position + x, Y_position + (y + ((spriteSize / 2) - y) * 2) - 1, GetColor(sprite[y][x]));
				else if (y > spriteSize / 2)
					SetPixel(hdc, X_position + x, Y_position + (y - (y - (spriteSize / 2)) * 2) - 1, GetColor(sprite[y][x]));
			}
			else if (rotation_value == 4) {
				if (x <= spriteSize / 2)
					SetPixel(hdc, X_position + (x + ((spriteSize / 2) - x) * 2) - 1, Y_position + y, GetColor(sprite[y][x]));
				else if (x > spriteSize / 2)
					SetPixel(hdc, X_position + (x - (x - (spriteSize / 2)) * 2) - 1, Y_position + y, GetColor(sprite[y][x]));
			}
			else if (rotation_value == 5) {
				if (x <= spriteSize / 2)
					SetPixel(hdc, X_position + (x + ((spriteSize / 2) - x) * 2) - 1, Y_position + (y + ((spriteSize / 2) - y) * 2) - 1, GetColor(sprite[y][x]));
				else if (x > spriteSize / 2)
					SetPixel(hdc, X_position + (x - (x - (spriteSize / 2)) * 2) - 1, Y_position + (y - (y - (spriteSize / 2)) * 2) - 1, GetColor(sprite[y][x]));
			}
		}
	}
}
void DrawMap(int s_x, int s_y) {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth - 1; x++) {
			if (Map[y][x] == '#')
				Game.DrawSprite(Game.wall, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '=')
				Game.DrawSprite(Game.wall, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '2')
				Game.DrawSprite(Game.wall_corner, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '3')
				Game.DrawSprite(Game.wall_corner, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '1')
				Game.DrawSprite(Game.wall_corner, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '4')
				Game.DrawSprite(Game.wall_corner, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '5')
				Game.DrawSprite(Game.wall_cross, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '6')
				Game.DrawSprite(Game.wall_cross, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'e')
				Game.DrawSprite(Game.wall_cross_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'w')
				Game.DrawSprite(Game.wall_cross_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'g')
				Game.DrawSprite(Game.wall_cross_two, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'h')
				Game.DrawSprite(Game.wall_cross_two, 5, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'j')
				Game.DrawSprite(Game.wall_cross_two, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'b')
				Game.DrawSprite(Game.wall_cross_two, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'r')
				Game.DrawSprite(Game.wall_cross_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'c')
				Game.DrawSprite(Game.wall_cross_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'm')
				Game.DrawSprite(Game.wall_cross_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'n')
				Game.DrawSprite(Game.wall_cross_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '7')
				Game.DrawSprite(Game.wall_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '8')
				Game.DrawSprite(Game.wall_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'y')
				Game.DrawSprite(Game.wall_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == '-')
				Game.DrawSprite(Game.gate, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'u')
				Game.DrawSprite(Game.wall_thick, 2, s_x + (x * 8), s_y + (y * 8) - 1);
			else if (Map[y][x] == 'p')
				Game.DrawSprite(Game.wall_corner_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 't')
				Game.DrawSprite(Game.wall_corner_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'k')
				Game.DrawSprite(Game.wall_corner_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 'l')
				Game.DrawSprite(Game.wall_corner_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == ' ')
				continue;
			else
				Game.DrawSprite(Game.error, 1, s_x + (x * 8), s_y + (y * 8));
		}
	}
}
COLORREF GetColor(int color_data) {
	if (color_data == 1)
		return RGB(0, 0, 255);
	else if (color_data == 5)
		return RGB(255, 20, 147);
	else if (color_data == 3)
		return RGB(255, 255, 255);
	else if (color_data == 4)
		return RGB(255, 255, 0);
	else
		return RGB(12, 12, 12);
}
bool KeyIsDown(char key, bool pressed, bool held) {
	int keyState = GetAsyncKeyState(static_cast<int>(key));
	return (pressed && (keyState & 1)) || (held && (keyState & 0xA000));
}
double getTime() {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9;
}
double getTimeSince(double startTime) {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9 - startTime;
}
double wait(double waitTime) {
	double startTime = getTime();

	while (waitTime > getTimeSince(startTime)) {}

	return getTimeSince(startTime + waitTime);
}
