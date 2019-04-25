#pragma once
#include <iostream>
#include <Windows.h>
const int mapWidth = 11;
const int mapHeight = mapWidth;
const char Map[mapHeight][mapWidth]{
	" ========2",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"3======== ",
};
class TileSprites {
	public:
	void DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position);
	const HDC hdc = GetDC(GetConsoleWindow());
	const int spriteSize = 8;
	const int wall[8][8] = {
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
	};
	const int wall_corner[8][8] = {
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,0,1,1,
		0,0,1,1,0,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,0,1,1,1,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};
	const int error[8][8] = {
		0,5,0,5,0,5,0,5,
		5,0,5,0,5,0,5,0,
		0,5,0,5,0,5,0,5,
		5,0,5,0,5,0,5,0,
		0,5,0,5,0,5,0,5,
		5,0,5,0,5,0,5,0,
		0,5,0,5,0,5,0,5,
		5,0,5,0,5,0,5,0,
	};
};
