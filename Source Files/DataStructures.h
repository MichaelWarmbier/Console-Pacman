#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#pragma comment(lib, "winmm.lib")

using namespace std;
using namespace chrono;

// Global
bool EXIT_GAME = false;
bool EXIT_MENU = false;
bool EXIT_PROGRAM = false;
const int CharacterSize = 12;

// FPS related
double FPS = 1.0 / 60.0;
double timer = 0, dt = 0;

// Board dimensions
const int mapWidth = 28;
const int mapHeight = 36;

// Input enumerator
enum input { LEFT, RIGHT, UP, DOWN, NONE, START };
input game_input = NONE;

// Menu variables
input MainMenuInput = NONE;
int OptionSelected = 1;

// Game variables
enum state {BEFORE, DURING, AFTER, LIMBO};
state GameState = BEFORE;
enum mode {CHASE, SCATTER, FEAR};
mode GhostMode = CHASE;
int TimeStamp = 0;
int CollectedDots = 0;

// Game Map
int Map[mapHeight][mapWidth]{
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	06,03,03,03,03,03,03,03,03,03,03,03,03,12,13,03,03,03,03,03,03,03,03,03,03,03,03,04,
	02,01,01,01,01,01,01,01,01,01,01,01,01,24,25,01,01,01,01,01,01,01,01,01,01,01,01,02,
	02,01,31,26,26,32,01,31,26,26,26,32,01,24,25,01,31,26,26,26,32,01,31,26,26,32,01,02,
	02,90,24,00,00,25,01,24,00,00,00,25,01,24,25,01,24,00,00,00,25,01,24,00,00,25,90,02,
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
	00,00,00,00,00,02,01,24,25,00,00,00,00,00,00,00,00,00,00,24,25,01,02,00,00,00,00,00,
	00,00,00,00,00,02,01,24,25,00,31,26,26,26,26,26,26,32,00,24,25,01,02,00,00,00,00,00,
	06,03,03,03,03,11,01,29,30,00,29,28,28,20,21,28,28,30,00,29,30,01,05,03,03,03,03,04,
	02,01,01,01,01,01,01,01,01,01,01,01,01,24,25,01,01,01,01,01,01,01,01,01,01,01,01,02,
	02,01,31,26,26,32,01,31,26,26,26,32,01,24,25,01,31,26,26,26,32,01,31,26,26,32,01,02,
	02,01,29,28,20,25,01,29,28,28,28,30,01,29,30,01,29,28,28,28,30,01,24,21,28,30,01,02,
	02,90,01,01,24,25,01,01,01,01,01,01,01,99,00,01,01,01,01,01,01,01,24,25,01,01,90,02,
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
const int Logo[30][89]{
	1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,
	1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,
	1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,
	1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,
	1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,2,2,2,2,2,2,2,2,2,3,0,0,0,0,0,0,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,3,0,2,2,2,2,2,3,0,0,0,0,0,0,2,2,2,3,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,3,
	2,2,2,2,2,2,2,2,2,2,3,0,0,0,0,0,2,2,2,2,2,3,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,3,0,0,0,0,0,2,2,2,2,2,3,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,3,
	2,2,3,3,3,3,3,3,2,2,3,0,0,0,0,2,2,2,3,2,2,2,3,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,2,2,3,3,3,2,2,3,3,3,2,2,3,0,0,0,0,2,2,2,3,2,2,2,3,0,0,0,0,2,2,3,3,3,3,3,3,2,2,3,
	2,2,3,0,0,0,0,0,2,2,3,0,0,0,2,2,2,3,0,0,2,2,2,3,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,2,2,3,0,0,0,2,2,2,3,0,0,2,2,2,3,0,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,2,2,3,0,0,2,2,2,3,0,0,0,0,2,2,2,3,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,2,2,3,0,0,2,2,2,3,0,0,0,0,2,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,3,3,3,3,3,3,3,3,0,0,2,2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,3,3,3,3,3,3,2,2,3,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,1,1,0,0,1,1,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,3,3,3,3,3,3,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
	2,2,3,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,0,2,2,3,0,0,2,2,3,0,0,0,0,0,2,2,3,
};
class TileSprites { // Class for functions and variables related to map tiles
public:
	void DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position);

	const HDC hdc = GetDC(GetConsoleWindow());

	const int spriteSize = 8;

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
	const int Special_R[8][8] = {
		0,0,0,0,0,0,0,0,
		4,4,4,4,4,4,0,0,
		4,4,0,0,0,4,4,0,
		4,4,0,0,0,4,4,0,
		4,4,0,0,4,4,4,0,
		4,4,4,4,4,0,0,0,
		4,4,0,4,4,4,0,0,
		4,4,0,0,4,4,4,0,
	};
	const int Special_E[8][8] = {
		0,0,0,0,0,0,0,0,
		4,4,4,4,4,4,0,0,
		4,4,0,0,0,0,0,0,
		4,4,0,0,0,0,0,0,
		4,4,4,4,4,0,0,0,
		4,4,0,0,0,0,0,0,
		4,4,0,0,0,0,0,0,
		4,4,4,4,4,4,0,0,
	};
	const int Special_A[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,4,4,4,0,0,0,
		0,4,4,0,4,4,0,0,
		4,4,0,0,0,4,4,0,
		4,4,0,0,0,4,4,0,
		4,4,4,4,4,4,4,0,
		4,4,0,0,0,4,4,0,
		4,4,0,0,0,4,4,0,
	};
	const int Special_D[8][8] = {
		0,0,0,0,0,0,0,0,
		4,4,4,4,4,0,0,0,
		4,4,0,0,4,4,0,0,
		4,4,0,0,0,4,4,0,
		4,0,0,0,0,4,4,0,
		4,4,0,0,0,4,4,0,
		4,4,0,0,4,4,0,0,
		4,4,4,4,4,0,0,0,
	};
	const int Special_Y[8][8] = {
		0,0,0,0,0,0,0,0,
		0,4,4,0,0,4,4,0,
		0,4,4,0,0,4,4,0,
		0,4,4,0,0,4,4,0,
		0,0,4,4,4,4,0,0,
		0,0,0,4,4,0,0,0,
		0,0,0,4,4,0,0,0,
		0,0,0,4,4,0,0,0,
	};
	const int Special_EXCLAMATIONPOINT[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,4,4,4,0,0,
		0,0,0,4,4,4,0,0,
		0,0,4,4,4,0,0,0,
		0,0,4,4,0,0,0,0,
		0,0,4,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		4,0,0,0,0,0,0,0,
	};
};

class Pacman {
public:
	Pacman();
	~Pacman();
	void ChangePhase();
	void DrawPacman(const int sprite[8][8], int rotation_value, int X_position, int Y_position);
	bool CollisionCheck(input dir);
	void MovePacman(input dir);
	int GetRotationValue(input dir);

	const HDC hdc = GetDC(GetConsoleWindow());
	COLORREF black = RGB(12, 12, 12);
	COLORREF white = RGB(255, 255, 255);
	COLORREF blue = RGB(0, 0, 255);
	COLORREF dots = RGB(255, 188, 149);

	HBRUSH blackBrush = CreateSolidBrush(black);
	HBRUSH whiteBrush = CreateSolidBrush(white);
	HBRUSH dotsBrush = CreateSolidBrush(dots);
	HPEN outlinePen = CreatePen(PS_NULL, 0, black);
	HPEN blueOutlinePen = CreatePen(PS_SOLID, 1, blue);

	const int spriteSize = 8;

	int adjpx = 4; 
	bool clearorigin = false;
	int currentPhase = 1;

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

// Characters
const int SPACE[CharacterSize][CharacterSize]{
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
};
const int A[CharacterSize][CharacterSize]{
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,0,0,1,1,0,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
};
const int C[CharacterSize][CharacterSize]{
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
};
const int D[CharacterSize][CharacterSize]{
	0,1,1,1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,1,1,0,0,0,0,
	0,1,1,0,0,0,1,1,1,0,0,0,
	0,1,1,0,0,0,0,1,1,1,0,0,
	0,1,1,0,0,0,0,0,1,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,1,1,1,0,
	0,1,1,0,0,0,0,1,1,1,0,0,
	0,1,1,0,0,0,1,1,1,0,0,0,
	0,1,1,1,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,0,0,0,0,0,
};
const int E[CharacterSize][CharacterSize]{
	0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,0,
};
const int I[CharacterSize][CharacterSize]{
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
};
const int L[CharacterSize][CharacterSize]{
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,0,0,
};
const int N[CharacterSize][CharacterSize]{
	1,1,0,0,0,0,0,0,0,0,1,1,
	1,1,1,0,0,0,0,0,0,0,1,1,
	1,1,1,1,0,0,0,0,0,0,1,1,
	1,1,1,1,1,0,0,0,0,0,1,1,
	1,1,0,1,1,1,0,0,0,0,1,1,
	1,1,0,0,1,1,1,0,0,0,1,1,
	1,1,0,0,0,1,1,1,0,0,1,1,
	1,1,0,0,0,0,1,1,1,0,1,1,
	1,1,0,0,0,0,0,1,1,1,1,1,
	1,1,0,0,0,0,0,0,1,1,1,1,
	1,1,0,0,0,0,0,0,0,1,1,1,
	1,1,0,0,0,0,0,0,0,0,1,1,
};
const int O[CharacterSize][CharacterSize]{
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
};
const int P[CharacterSize][CharacterSize]{
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
};
const int R[CharacterSize][CharacterSize]{
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,0,0,0,0,0,0,
	0,0,1,1,1,1,1,0,0,0,0,0,
	0,0,1,1,0,1,1,1,0,0,0,0,
	0,0,1,1,0,0,1,1,1,0,0,0,
	0,0,1,1,0,0,0,1,1,1,0,0,
	0,0,1,1,0,0,0,0,1,1,0,0,
};
const int S[CharacterSize][CharacterSize]{
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,0,0,0,0,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
};
const int T[CharacterSize][CharacterSize]{
	0,0,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
};
const int V[CharacterSize][CharacterSize]{
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,1,0,0,0,0,1,1,1,0,
	0,0,1,1,1,0,0,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
};
const int X[CharacterSize][CharacterSize]{
	1,1,0,0,0,0,0,0,0,0,1,1,
	1,1,1,0,0,0,0,0,0,1,1,1,
	0,1,1,1,0,0,0,0,1,1,1,0,
	0,0,1,1,1,0,0,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,0,0,1,1,1,0,0,
	0,1,1,1,0,0,0,0,1,1,1,0,
	1,1,1,0,0,0,0,0,0,1,1,1,
	1,1,0,0,0,0,0,0,0,0,1,1,
};
const int Y[CharacterSize][CharacterSize]{
	0,1,1,0,0,0,0,0,0,1,1,0,
	0,1,1,1,0,0,0,0,1,1,1,0,
	0,0,1,1,1,0,0,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
};
const int EXCLAMATION[CharacterSize][CharacterSize]{
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
};
const int LEFT_ARROW[CharacterSize][CharacterSize]{
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,0,0,0,0,0,0,
	0,0,1,1,1,0,0,0,0,0,0,0,
	0,1,1,1,0,0,0,0,0,0,0,0,
	1,1,1,0,0,0,0,0,0,0,0,0,
	1,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,1,0,0,0,0,0,0,0,
	0,0,0,1,1,1,0,0,0,0,0,0,
	0,0,0,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
};
const int DASH[CharacterSize][CharacterSize]{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,0,
	1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
};
const int COLON[CharacterSize][CharacterSize]{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
};
const int QUESTIONMARK[CharacterSize][CharacterSize]{
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,0,0,1,1,0,0,0,
	0,0,1,1,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,
	0,0,0,0,0,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,0,0,0,0,0,
};

#endif
