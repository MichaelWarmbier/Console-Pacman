#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <conio.h>
#include <chrono>
#include <string>
#include <stdio.h>
using namespace std;
using namespace chrono;

HBITMAP bmap = (HBITMAP)LoadImage(NULL, _T("ConsolePacmanSpriteSheet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
const int CW = 28;
const int CH = 36;

class Menu {
private:
	enum Input { ENTER, BACK, UP, DOWN, LEFT, RIGHT, NONE };
	const HDC console = GetDC(GetConsoleWindow());
	const HDC hdc = CreateCompatibleDC(NULL);
	Input PlayerInput;

	const int CSS = 16;

	bool GhostPhase;
	int PacmanPhase;
	double PhaseTS;

	int MainOpt;
	int OptionsOpt;
	bool CurrentMenu;

	bool ToggleDebug;
	bool ToggleAudio;

	const int SpritePositions[51][2] = {
	02,02, // Empty / 1

	2, 202, // Pacman P1 / 2
	22, 202, // Pacman P2 / 3
	42, 202, // Pacman P3 / 4

	02, 222, // A / 5
	22, 222, // B / 6
	42, 222, // C / 7
	62, 222, // D / 8
	82, 222, // E / 9
	102, 222, // F / 10
	122, 222, // G / 11
	142, 222, // H / 12
	162, 222, // I / 13

	02, 242, // J / 14
	22, 242, // K / 15
	42, 242, // L / 16
	62, 242, // M / 17
	82, 242, // N / 18
	102, 242, // O / 19
	122, 242, // P / 20
	142, 242, // Q / 21
	162, 242, // R / 22

	02, 262, // S / 23
	22, 262, // T / 24
	42, 262, // U / 25
	62, 262, // V / 26
	82, 262, // W / 27
	102, 262, // X / 28
	122, 262, // Y / 29
	142, 262, // Z / 30

	02, 282, // - / 31
	22, 282, // : / 32
	42, 282, // ! / 33
	62, 282, // ? / 34
	82, 282, // " / 35
	102, 282, // . / 36

	02, 02, // ' ' / 37

	42, 182, // Blinky P1 / 38
	62, 182, // Blinky P2 52 / 39
	82, 182, // Inky P1 53 / 40
	102, 182, // Inky P2 54 / 41
	122, 182, // Pinky P1 55 / 42
	142, 182, // Pinky P2 56 / 43
	162, 182, // Clyde P1 / 44
	182, 182, // Clyde P2 / 45

	62, 162, // False / 46
	82, 162, // True / 47

	2,395, // Blinky Desc / 48
	2,413, // Pinky Desc / 49
	2,431, // Inky Desc / 50
	2,449, // Clyde Desc / 51

	};
public:
	Menu() {
		GhostPhase = 1;
		PacmanPhase = 1;
		PhaseTS = GetTime();
		PlayerInput = NONE;
		MainOpt = 1;
		OptionsOpt = 1;
		CurrentMenu = true;
		ToggleDebug = false;
		ToggleAudio = true;
	}
	void Draw() {
		DrawLogo(1, 3);
		switch (CurrentMenu) {
		case true:
			if (MainOpt == 1)
				DrawSprite(GetSpriteByPhase(), CW / 2 - 7, 12, CSS);
			else
				DrawSprite(1, CW / 2 - 7, 12, CSS);
			DrawString("Play Game", CW / 2 - 5, 12);


			if (MainOpt == 2)
				DrawSprite(GetSpriteByPhase(), CW / 2 - 6, 14, CSS);
			else
				DrawSprite(1, CW / 2 - 6, 14, CSS);
			DrawString("Options", CW / 2 - 4, 14);


			if (MainOpt == 3)
				DrawSprite(GetSpriteByPhase(), CW / 2 - 5, 16, CSS);
			else
				DrawSprite(1, CW / 2 - 5, 16, CSS);
			DrawString("Exit", CW / 2 - 3, 16);

				for (int i = 0; i < 4; i++) {
					DrawSprite(48 + i, CW / 2 - 8, 20 + (2 * i), CSS * 17);
					if (GhostPhase)
						DrawSprite(38 + (i * 2), CW / 2 - 10, 20 + (2 * i), CSS);
					else
						DrawSprite(39 + (i * 2), CW / 2 - 10, 20 + (2 * i), CSS);
				}


			break;
		case false:
			if (OptionsOpt == 1)
				DrawSprite(GetSpriteByPhase(), CW / 2 - 10, 12, CSS);
			else
				DrawSprite(1, CW / 2 - 10, 12, CSS);
			DrawString("Toggle Debug", CW / 2 - 8, 12);
			if (ToggleDebug)
				DrawSprite(47, CW / 2 + 9, 12, CSS);
			else
				DrawSprite(46, CW / 2 + 9, 12, CSS);


			if (OptionsOpt == 2)
				DrawSprite(GetSpriteByPhase(), CW / 2 - 10, 14, CSS);
			else
				DrawSprite(1, CW / 2 - 10, 14, CSS);
			DrawString("Toggle Audio", CW / 2 - 8, 14);
			if (ToggleAudio)
				DrawSprite(47, CW / 2 + 9, 14, CSS);
			else
				DrawSprite(46, CW / 2 + 9, 14, CSS);


			if (OptionsOpt == 3)
				DrawSprite(GetSpriteByPhase(), CW / 2 - 10, 16, CSS);
			else
				DrawSprite(1, CW / 2 - 10, 16, CSS);
			DrawString("Delete Highscore", CW / 2 - 8, 16);
		}
	}
	void Input() {
		PlayerInput = NONE;
		if (KeyIsDown('W', true, false))
			PlayerInput = UP;
		if (KeyIsDown('S', true, false))
			PlayerInput = DOWN;
		if (KeyIsDown(13, true, false))
			PlayerInput = ENTER;
		if (KeyIsDown(27, true, false))
			PlayerInput = BACK;
	}
	void Logic() {
		while (GetConsoleWindow() != GetForegroundWindow()) {};
		if (GetTimeSince(PhaseTS) > .1) {
			GhostPhase = !GhostPhase;
			PacmanPhase++;
			PhaseTS = GetTime();
		}
		if (PacmanPhase > 3)
			PacmanPhase = 1;
		switch (CurrentMenu) {
		default:
		case true:
			switch (PlayerInput) {
			case UP:
				MainOpt--;
				break;
			case DOWN:
				MainOpt++;
				break;
			case ENTER:
				switch (MainOpt) {
				case 1:
					// START GAME
					break;
				case 2:
					system("CLS");
					CurrentMenu = false;
					break;
				case 3:
					// EXIT GAME
					break;
				}
				break;
			}
			break;
		case false:
			switch (PlayerInput) {
			case UP:
				OptionsOpt--;
				break;
			case DOWN:
				OptionsOpt++;
				break;
			case ENTER:
				switch (OptionsOpt) {
				case 1:
					ToggleDebug = !ToggleDebug;
					break;
				case 2:
					ToggleAudio = !ToggleAudio;
					break;
				case 3:
					// ERASE SAVE DATA
					break;
				}
				break;
			case BACK:
				system("CLS");
				CurrentMenu = true;
				break;
			}
			break;
		}
		if (MainOpt > 3)
			MainOpt = 3;
		else if (MainOpt < 1)
			MainOpt = 1;
		if (OptionsOpt > 3)
			OptionsOpt = 3;
		else if (OptionsOpt < 1)
			OptionsOpt = 1;
	}
	int GetSpriteByPhase() {
		switch (PacmanPhase) {
		default:
		case 1:
			return 2;
		case 2:
			return 3;
		case 3:
			return 4;
		}
	}
	void DrawSprite(int ArrPos, int x, int y, int width) {
		SelectObject(hdc, bmap);
		BitBlt(console, x * CSS, y * CSS, width, CSS, hdc, SpritePositions[ArrPos - 1][0], SpritePositions[ArrPos - 1][1], SRCCOPY);
		DeleteObject(bmap);
	}
	void DrawLogo(int x, int y) {
		SelectObject(hdc, bmap);
		BitBlt(console, x * CSS, y * CSS, 403, 92, hdc, 0, 300, SRCCOPY);
		DeleteObject(bmap);
	}
	void DrawString(const char arg[], int x, int y) {
		int i = 0;
		while (arg[i] != '\0') {
			if (arg[i] == '-')
				DrawSprite(31, x + i, y, CSS);
			else if (arg[i] == ':')
				DrawSprite(32, x + i, y, CSS);
			else if (arg[i] == '!')
				DrawSprite(33, x + i, y, CSS);
			else if (arg[i] == 63)
				DrawSprite(34, x + i, y, CSS);
			else if (arg[i] == '\"')
				DrawSprite(35, x + i, y, CSS);
			else if (arg[i] == '.')
				DrawSprite(36, x + i, y, CSS);
			else if (arg[i] == ' ')
				DrawSprite(37, x + i, y, CSS);
			else
				DrawSprite(toupper(arg[i]) - 60, x + i, y, CSS);
			i++;
		}
	}
};
