#pragma once
#include "DataStructures.h"
void GameDraw();
void GameInput();
void GameLogic();
void DrawMap(int X_pos, int Y_pos);
COLORREF GetColor(int color_data);
TileSprites Game;
void TileSprites::DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position) {
	for (int y = 0; y < spriteSize; y++) {
		for (int x = 0; x < spriteSize; x++) {
			if (rotation_value != 1 && rotation_value != 2 && rotation_value != 3 && rotation_value != 4)
				SetPixel(hdc, X_position + x, Y_position + y, RGB(255, 20, 147));
			else if (sprite[y][x] == 0)
				continue;
			else if (rotation_value == 1)
				SetPixel(hdc, X_position + x, Y_position + y, GetColor(sprite[y][x]));
			else if (rotation_value == 2)
					SetPixel(hdc, X_position + y, Y_position + x, GetColor(sprite[y][x]));
		}
	}
}
void GameDraw() {
	DrawMap(20, 20);
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
	else
		return RGB(12, 12, 12);
}
