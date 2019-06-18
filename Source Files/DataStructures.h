#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <conio.h>

using namespace std;
using namespace chrono;

double FPS = 1.0 / 60.0; // Frames
double timer = 0, dt = 0;

// Board dimensions
const int mapWidth = 28;
const int mapHeight = 36;

// Input enumerator
enum input { LEFT, RIGHT, UP, DOWN, NONE, START };
input game_input = NONE;

// Game variables

int CollectedDots = 0;

int Map[mapHeight][mapWidth]{
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	06,03,03,03,03,03,03,03,03,03,03,03,03,12,13,03,03,03,03,03,03,03,03,03,03,03,03,04,
	02,01,01,01,01,01,01,01,01,01,01,01,01,24,25,01,01,01,01,01,01,01,01,01,01,01,01,02,
	02,01,31,26,26,32,01,31,26,26,26,32,01,24,25,01,31,26,26,26,32,01,31,26,26,32,01,02,
	02,01,24,00,00,25,01,24,00,00,00,25,01,24,25,01,24,00,00,00,25,01,24,00,00,25,01,02,
	02,01,29,28,28,30,01,29,28,28,28,30,01,29,30,01,29,28,28,28,30,01,29,28,28,30,01,02,
	02,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,02,
	02,01,31,26,26,32,01,31,32,01,31,26,26,26,26,26,26,32,01,31,32,01,31,26,26,32,01,02,
	02,01,29,28,28,30,01,24,25,01,29,28,28,20,21,28,28,30,01,24,25,01,29,28,28,30,01,02,
	02,01,01,01,01,01,01,24,25,01,01,01,01,24,25,01,01,01,01,24,25,01,01,01,01,01,01,02,
	05,03,03,03,03,04,01,24,23,26,26,32,00,24,25,00,31,26,26,22,25,01,06,03,03,03,03,11,
	00,00,00,00,00,02,01,24,21,28,28,30,00,29,30,00,29,28,28,20,25,01,02,00,00,00,00,00,
	00,00,00,00,00,02,01,24,25,00,00,00,00,00,00,00,00,00,00,24,25,01,02,00,00,00,00,00,
	00,00,00,00,00,02,01,24,25,00,07,03,03,27,27,03,03,33,00,24,25,01,02,00,00,00,00,00,
	03,03,03,03,03,11,01,29,30,00,02,00,00,00,00,00,00,02,00,29,30,01,05,03,03,03,03,03,
	00,00,00,00,00,00,01,00,00,00,02,00,00,00,00,00,00,02,00,00,00,01,00,00,00,00,00,00, // Portal
	03,03,03,03,03,04,01,31,32,00,02,00,00,00,00,00,00,02,00,31,32,01,06,03,03,03,03,03,
	00,00,00,00,00,02,01,24,25,00,34,03,03,03,03,03,03,10,00,24,25,01,02,00,00,00,00,00,
	00,00,00,00,00,02,01,24,25,00,00,00,00,99,00,00,00,00,00,24,25,01,02,00,00,00,00,00,
	00,00,00,00,00,02,01,24,25,00,31,26,26,26,26,26,26,32,00,24,25,01,02,00,00,00,00,00,
	06,03,03,03,03,11,01,29,30,00,29,28,28,20,21,28,28,30,00,29,30,01,05,03,03,03,03,04,
	02,01,01,01,01,01,01,01,01,01,01,01,01,24,25,01,01,01,01,01,01,01,01,01,01,01,01,02,
	02,01,31,26,26,32,01,31,26,26,26,32,01,24,25,01,31,26,26,26,32,01,31,26,26,32,01,02,
	02,01,29,28,20,25,01,29,28,28,28,30,01,29,30,01,29,28,28,28,30,01,24,21,28,30,01,02,
	02,01,01,01,24,25,01,01,01,01,01,01,01,00,00,01,01,01,01,01,01,01,24,25,01,01,01,02,
	18,26,32,01,24,25,01,31,32,01,31,26,26,26,26,26,26,32,01,31,32,01,24,25,01,31,26,17,
	16,28,30,01,29,30,01,24,25,01,29,28,28,20,21,28,28,30,01,24,25,01,29,30,01,29,28,19,
	02,01,01,01,01,01,01,24,25,01,01,01,01,24,25,01,01,01,01,24,25,01,01,01,01,01,01,02,
	02,01,31,26,26,26,26,15,14,26,26,32,01,24,25,01,31,26,26,15,14,26,26,26,26,32,01,02,
	02,01,29,28,28,28,28,28,28,28,28,30,01,29,30,01,29,28,28,28,28,28,28,28,28,30,01,02,
	02,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,02,
	05,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,11,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
};

class TileSprites { // Class for functions and variables related to map tiles
public:
	// Takes an argument of the sprite, it's rotation and it's position and draws
	void DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position);

	const HDC hdc = GetDC(GetConsoleWindow());

	const int spriteSize = 8; // Variable relating to sprite size

	// Wall Tiles Sprite Sheet //

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
	const int wall_corner_square[8][8] = {
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,1,1,
		0,0,1,0,0,0,0,0,
		0,0,1,0,0,0,0,0,
		0,0,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};
	const int wall_cross_two[8][8] = {
		0,0,1,0,0,0,0,0,
		0,0,1,0,0,0,0,0,
		0,0,1,0,0,0,0,0,
		0,0,1,0,0,0,0,1,
		0,0,1,0,0,0,1,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
		0,0,1,0,0,1,0,0,
	};
	const int wall_cross[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		1,1,1,0,0,0,0,0,
		0,0,0,1,0,0,0,0,
		0,0,0,0,1,0,0,0,
	};
	const int wall_cross_thick[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		1,1,1,0,0,0,0,0,
		0,0,0,1,0,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
	};
	const int wall_thick[8][8] = {
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
	};
	const int wall_corner_thick[8][8] = {
		0,0,0,0,1,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,1,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};
	const int gate[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		3,3,3,3,3,3,3,3,
		3,3,3,3,3,3,3,3,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};
	const int dot[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,3,3,0,0,0,
		0,0,0,3,3,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};
	// Error handling tile: invalid tile number
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

class Pacman {
public:
	Pacman();
	~Pacman();
	void ChangePhase(); // Increments Pacmans phase and defines it's bounds
	// Takes an argument of the sprite, it's rotation value, and it's position and draws
	void DrawPacman(const int sprite[8][8], int rotation_value, int X_position, int Y_position);
	bool CollisionCheck(input dir); // Check for collision in a given direction
	void MovePacman(input dir); // Main movement function
	int GetRotationValue(input dir); // Gets direction pacman should face given direction

	const HDC hdc = GetDC(GetConsoleWindow());
	COLORREF black = RGB(12, 12, 12);
	COLORREF white = RGB(255, 255, 255);
	COLORREF blue = RGB(0, 0, 255);
	HBRUSH blackBrush = CreateSolidBrush(black);
	HBRUSH whiteBrush = CreateSolidBrush(white);
	HPEN outlinePen = CreatePen(PS_NULL, 0, black);
	HPEN blueOutlinePen = CreatePen(PS_SOLID, 1, blue);

	const int spriteSize = 8; // Constant sprite size for Pacman sprites

	int adjpx = 4; // Adjustment pixels prior to first input
	bool clearorigin = false; // Bool for whether or not origin (starting coordinates) should be cleared
	int currentPhase = 1; // Current phase counter

	int X_pos = 0; // Current X position
	int Y_pos = 0; // Current Y position

	int X_old = 0; // Last X position
	int Y_old = 0; // Last Y position

	// Pacman phase sprites //

	const int P1[8][8] = {
		9,9,4,4,4,4,4,9,
		9,4,4,4,4,4,9,9,
		4,4,4,4,4,9,9,9,
		4,4,4,9,9,9,9,9,
		4,4,4,9,9,9,9,9,
		4,4,4,4,4,9,9,9,
		9,4,4,4,4,4,9,9,
		9,9,4,4,4,4,4,9,
	};
	const int P2[8][8] = {
		9,9,4,4,4,4,9,9,
		9,4,4,4,4,4,4,9,
		4,4,4,4,4,9,9,9,
		4,4,4,9,9,9,9,9,
		4,4,4,9,9,9,9,9,
		4,4,4,4,4,9,9,9,
		9,4,4,4,4,4,4,9,
		9,9,4,4,4,4,9,9,
	};
	const int P3[8][8] = {
		9,9,4,4,4,4,9,9,
		9,4,4,4,4,4,4,9,
		4,4,4,4,4,4,4,4,
		4,4,4,4,4,4,4,4,
		4,4,4,4,4,4,4,4,
		4,4,4,4,4,4,4,4,
		9,4,4,4,4,4,4,9,
		9,9,4,4,4,4,9,9,
	};
};

#endif
