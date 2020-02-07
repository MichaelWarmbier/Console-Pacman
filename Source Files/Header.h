#pragma once
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <chrono>
#include <tchar.h>
using namespace std;
using namespace chrono;

const HDC console = GetDC(GetConsoleWindow());
const HDC hdc = CreateCompatibleDC(NULL);
const HBITMAP bmap = (HBITMAP)LoadImage(NULL, _T("PM_Sprites.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 

bool DrawSprite(int xpos, int ypos, int ID);
bool KeyIsDown(char key, bool pressed, bool held);
double GetTime();
double GetTimeSince(double startTime);
double Wait(double waitTime);
void ShowConsoleCursor(bool showFlag);

const int res = 16;
const int BW = 28, BH = 36;
enum dir { UP, DOWN, LEFT, RIGHT, NONE };
