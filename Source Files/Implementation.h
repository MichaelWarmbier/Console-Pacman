#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include "DataStructures.h"

//////////////////////////////////////// - TO BE CREDITED
#define TIMER_INIT \
    LARGE_INTEGER frequency; \
    LARGE_INTEGER t1,t2; \
    double elapsedTime; \
    QueryPerformanceFrequency(&frequency);

/** Use to start the performance timer */
#define TIMER_START QueryPerformanceCounter(&t1);

/** Use to stop the performance timer and output the result to the standard stream.*/
#define TIMER_STOP \
    QueryPerformanceCounter(&t2); \
    elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart; \
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {3, 1}); \
    wcout << 1.0 / elapsedTime << setprecision(1) << fixed << L" fps"; \
	cout << " Collected Dots: " << CollectedDots; \
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 1});
//////////////////////////////////////// - TO BE CREDITED

// Menu Setup & Routines
void MenuSetup();
void MenuDraw();
void MenuInput();
void MenuLogic();

// Game Setup & Routines
void GameSetup();
void GameDraw();
void GameInput();
void GameLogic();

// Drawing & Clearing Functions
void DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position);
void DrawPacman(const int sprite[8][8], int rotation_value, int X_position, int Y_position);
void DrawChar(const int sprite[CharacterSize][CharacterSize], int x, int y, COLORREF color);
void DrawMap(int X_pos, int Y_pos);
void DrawLogo();
void DrawReady(bool);
void ClearArrows();

// Check Functions
bool CollisionCheck(input dir);
bool CheckForDot(input dir);
bool KeyIsDown(char key, bool pressed = true, bool held = true);

// Chrono Functions
double GetTime();
double GetTimeSince(double);
double wait(double);

// Adjustment Functions
void ShowConsoleCursor(bool showFlag);
void MovePacman(input dir);
void ChangePhase();

// Get Functions
int GetRotationValue(input dir);
COLORREF GetColor(int color_data);

////////////////////////////////////////////
// Main Menu Routines & Setup Definitions //
////////////////////////////////////////////
void MenuSetup() {
	system("CLS");
	system("MODE 44, 20");
	ShowConsoleCursor(false);
	EXIT_MENU = false;
}
void MenuDraw() {
	DrawChar(P, 10, 15, RGB(255, 255, 255));
	DrawChar(L, 11, 15, RGB(255, 255, 255));
	DrawChar(A, 12, 15, RGB(255, 255, 255));
	DrawChar(Y, 13, 15, RGB(255, 255, 255));
	DrawChar(EXCLAMATION, 14, 15, RGB(255, 255, 255));
	if (OptionSelected == 1) {
		DrawChar(LEFT_ARROW, 16, 15, RGB(255, 255, 255));
		DrawChar(DASH, 17, 15, RGB(255, 255, 255));
	}
	DrawChar(O, 10, 17, RGB(255, 255, 255));
	DrawChar(P, 11, 17, RGB(255, 255, 255));
	DrawChar(T, 12, 17, RGB(255, 255, 255));
	DrawChar(I, 13, 17, RGB(255, 255, 255));
	DrawChar(O, 14, 17, RGB(255, 255, 255));
	DrawChar(N, 15, 17, RGB(255, 255, 255));
	DrawChar(S, 16, 17, RGB(255, 255, 255));
	if (OptionSelected == 2) {
		DrawChar(LEFT_ARROW, 18, 17, RGB(255, 255, 255));
		DrawChar(DASH, 19, 17, RGB(255, 255, 255));
	}
	DrawChar(E, 10, 19, RGB(255, 255, 255));
	DrawChar(X, 11, 19, RGB(255, 255, 255));
	DrawChar(I, 12, 19, RGB(255, 255, 255));
	DrawChar(T, 13, 19, RGB(255, 255, 255));
	if (OptionSelected == 3) {
		DrawChar(LEFT_ARROW, 15, 19, RGB(255, 255, 255));
		DrawChar(DASH, 16, 19, RGB(255, 255, 255));
	}
}
void MenuInput() {
	MainMenuInput = NONE;
	if (KeyIsDown('W', true, false))
		MainMenuInput = UP;
	if (KeyIsDown('S', true, false))
		MainMenuInput = DOWN;
	if (KeyIsDown(13, true, false))
		MainMenuInput = START;
}
void MenuLogic() {
	if (MainMenuInput == NONE)
		DrawLogo();
	if (MainMenuInput != NONE && MainMenuInput != START) {
		ClearArrows();
		switch (MainMenuInput) {
		case UP:
			OptionSelected--;
			break;
		case DOWN:
			OptionSelected++;
			break;
		}
		if (OptionSelected < 1)
			OptionSelected = 1;
		else if (OptionSelected > 3)
			OptionSelected = 3;
	}
	if (MainMenuInput == START) {
		switch (OptionSelected) {
		case 1:
			EXIT_MENU = true;
			EXIT_GAME = false;
			break;
		case 2:
			break;
		case 3:
			EXIT_MENU = true;
			EXIT_GAME = true;
			EXIT_PROGRAM = true;
			break;
		}
	}

}

///////////////////////////////////////
// Game Routines & Setup Definitions //
///////////////////////////////////////
void GameSetup() {
	system("CLS");
	system("MODE 34, 20");
	GameState = BEFORE;
	CollectedDots = 0;
	TimeStamp = GetTime();
}

void GameDraw() {
	DrawMap(20, 20);
}

void GameInput() {
	if (KeyIsDown(13, true, false)) {
		if (GameState == DURING)
			GameState = LIMBO;
		else if (GameState == LIMBO)
			GameState = DURING;
	}
	if (GameState != LIMBO && GameState != BEFORE) {
		if (KeyIsDown('W', true, true) && !KeyIsDown('S', true, true) && !CollisionCheck(UP))
			game_input = UP;
		else if (KeyIsDown('S', true, true) && !KeyIsDown('W', true, true) && !CollisionCheck(DOWN))
			game_input = DOWN;
		else if (KeyIsDown('A', true, true) && !KeyIsDown('D', true, true) && !CollisionCheck(LEFT))
			game_input = LEFT;
		else if (KeyIsDown('D', true, true) && !KeyIsDown('A', true, true) && !CollisionCheck(RIGHT))
			game_input = RIGHT;
		if (game_input != NONE && adjpx != 0) {
			// Adjusts pacman if no input has been made, centering him at origin
			adjpx = 0;
			clearorigin = true;
		}
	}
}

void GameLogic() {
	ChangePhase();
	MovePacman(game_input);
}

//////////////////////////
// Function Definitions //
//////////////////////////
void DrawLogo() {
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 89; x++) {
			if (Logo[y][x] == 0)
				continue;
			else if (Logo[y][x] == 1) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						SetPixel(hdc, x + 40 + i + (x * 2), y + 40 + j + (y * 2), RGB(255, 255, 255));
					}
				}
			}
			else if (Logo[y][x] == 2) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						SetPixel(hdc, x + 40 + i + (x * 2), y + 40 + j + (y * 2), RGB(255, 255, 0));
					}
				}
			}
			else if (Logo[y][x] == 3) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						SetPixel(hdc, x + 40 + i + (x * 2), y + 40 + j + (y * 2), RGB(0, 38, 255));
					}
				}
			}
			else if (Logo[y][x] == 4) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						SetPixel(hdc, x + 40 + i + (x * 2), y + 40 + j + (y * 2), RGB(255, 188, 149));
					}
				}
			}
		}
	}
}
void DrawReady(bool condition) {
	switch (condition) {
	case true:
		Map[20][11] = 75;
		Map[20][12] = 76;
		Map[20][13] = 77;
		Map[20][14] = 78;
		Map[20][15] = 79;
		Map[20][16] = 80;
		break;
	case false:
		Map[20][11] = 0;
		Map[20][12] = 0;
		Map[20][13] = 0;
		Map[20][14] = 0;
		Map[20][15] = 0;
		Map[20][16] = 0;
		Map[20][15] = 0;
		Map[20][16] = 0;
		break;
	}
}
void ClearArrows() {
	DrawChar(SPACE, 18, 17, RGB(12, 12, 12));
	DrawChar(SPACE, 19, 17, RGB(12, 12, 12));
	DrawChar(SPACE, 16, 15, RGB(12, 12, 12));
	DrawChar(SPACE, 17, 15, RGB(12, 12, 12));
	DrawChar(SPACE, 15, 19, RGB(12, 12, 12));
	DrawChar(SPACE, 16, 19, RGB(12, 12, 12));
}
void DrawChar(const int sprite[CharacterSize][CharacterSize], int x_pos, int y_pos, COLORREF color) {
	for (int y = 0; y < CharacterSize; y++) {
		for (int x = 0; x < CharacterSize; x++) {
			if (sprite[y][x] == 0)
				continue;
			else if (sprite[y][x] == 1)
				SetPixel(hdc, (x_pos * CharacterSize) + x, (y_pos * CharacterSize) + y, color);
		}
	}
}

void ChangePhase() {
	currentPhase++;
	if (currentPhase > 6)
		currentPhase = 1;
}

void DrawPacman(const int sprite[8][8], int rotation_value, int X_position, int Y_position) {
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
				if (x <= spriteSize / 2)
					SetPixel(hdc, X_position + y, Y_position + (x + ((spriteSize / 2) - x) * 2) - 1, GetColor(sprite[y][x]));
				else if (x > spriteSize / 2)
					SetPixel(hdc, X_position + y, Y_position + (x - (x - (spriteSize / 2)) * 2) - 1, GetColor(sprite[y][x]));
			}
			else if (rotation_value == 4) {
				if (x <= spriteSize / 2)
					SetPixel(hdc, X_position + (x + ((spriteSize / 2) - x) * 2) - 1, Y_position + y, GetColor(sprite[y][x]));
				else if (x > spriteSize / 2)
					SetPixel(hdc, X_position + (x - (x - (spriteSize / 2)) * 2) - 1, Y_position + y, GetColor(sprite[y][x]));
			}

		}
	}
}

void DrawSprite(const int sprite[8][8], int rotation_value, int X_position, int Y_position) {
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

int GetRotationValue(input dir) {
	switch (dir) {
	case NONE:
		return 1;
		break;
	case UP:
		return 3;
		break;
	case LEFT:
		return 4;
		break;
	case DOWN:
		return 2;
		break;
	case RIGHT:
		return 1;
		break;
	}
	return 1;
}

bool CheckForDot(input dir) {
	switch (dir) {
	case LEFT:
		if (Map[Y_pos][X_pos - 1] == 01)
			return true;
		else
			return false;
	case RIGHT:
		if (Map[Y_pos][X_pos + 1] == 01)
			return true;
		else
			return false;
	case DOWN:
		if (Map[Y_pos + 1][X_pos] == 01)
			return true;
		else
			return false;
	case UP:
		if (Map[Y_pos - 1][X_pos] == 01)
			return true;
		else
			return false;
	default:
		return false;
	}
}

void DrawMap(int s_x, int s_y) {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (Map[y][x] == 00 || Map[y][x] == 69)
				continue;
			if (Map[y][x] == 98) {
				if (!clearorigin) {
					SelectObject(hdc, blackBrush);
					Rectangle(hdc, s_x + (x * 8) + adjpx, s_y + (y * 8) + 1, s_x + (x * 8) + 10, s_y + (y * 8) + 11);
				}
				else {
					SelectObject(hdc, blackBrush);
					Rectangle(hdc, s_x + (x * 8) + adjpx, s_y + (y * 8), s_x + (x * 8) + 12, s_y + (y * 8) + 10);
					clearorigin = false;
				}
			}
			else if (Map[y][x] == 99) {
				switch (currentPhase) {
				case 1:
				case 2:
					DrawPacman(P1, GetRotationValue(game_input), s_x + (x * 8) + adjpx, s_y + (y * 8) + 1);
					break;
				case 3:
				case 4:
					DrawPacman(P2, GetRotationValue(game_input), s_x + (x * 8) + adjpx, s_y + (y * 8) + 1);
					break;
				case 5:
				case 6:
					DrawPacman(P3, GetRotationValue(game_input), s_x + (x * 8) + adjpx, s_y + (y * 8) + 1);
					break;
				}
				X_pos = x;
				Y_pos = y;
			}
			else if (Map[y][x] == 01) {
				SelectObject(hdc, dotsBrush);
				Rectangle(hdc, s_x + (x * 8) + 3, s_y + (y * 8) + 3, s_x + (x * 8) + 6, s_y + (y * 8) + 6);
			}
			else if (Map[y][x] == 02) {
				SelectObject(hdc, blueOutlinePen);
				MoveToEx(hdc, s_x + (x * 8) + 5, s_y + (y * 8) + 8, NULL);
				LineTo(hdc, s_x + (x * 8) + 5, s_y + (y * 8) - 1);
				MoveToEx(hdc, s_x + (x * 8) + 2, s_y + (y * 8) + 8, NULL);
				LineTo(hdc, s_x + (x * 8) + 2, s_y + (y * 8) - 1);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 03) {
				SelectObject(hdc, blueOutlinePen);
				MoveToEx(hdc, s_x + (x * 8), s_y + (y * 8) + 2, NULL);
				LineTo(hdc, s_x + (x * 8) + 8, s_y + (y * 8) + 2);
				MoveToEx(hdc, s_x + (x * 8), s_y + (y * 8) + 5, NULL);
				LineTo(hdc, s_x + (x * 8) + 8, s_y + (y * 8) + 5);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 04)
				DrawSprite(wall_corner, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 05)
				DrawSprite(wall_corner, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 06)
				DrawSprite(wall_corner, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 07)
				DrawSprite(wall_corner_square, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 33)
				DrawSprite(wall_corner_square, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 34)
				DrawSprite(wall_corner_square, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 10)
				DrawSprite(wall_corner_square, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 11)
				DrawSprite(wall_corner, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 12)
				DrawSprite(wall_cross, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 13)
				DrawSprite(wall_cross, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 14)
				DrawSprite(wall_cross_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 15)
				DrawSprite(wall_cross_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 16)
				DrawSprite(wall_cross_two, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 17)
				DrawSprite(wall_cross_two, 5, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 18)
				DrawSprite(wall_cross_two, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 19)
				DrawSprite(wall_cross_two, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 20)
				DrawSprite(wall_cross_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 21)
				DrawSprite(wall_cross_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 22)
				DrawSprite(wall_cross_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 23)
				DrawSprite(wall_cross_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 24) {
				SelectObject(hdc, blueOutlinePen);
				MoveToEx(hdc, s_x + (x * 8) + 4, s_y + (y * 8) + 8, NULL);
				LineTo(hdc, s_x + (x * 8) + 4, s_y + (y * 8) - 1);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 25) {
				SelectObject(hdc, blueOutlinePen);
				MoveToEx(hdc, s_x + (x * 8) + 3, s_y + (y * 8) + 8, NULL);
				LineTo(hdc, s_x + (x * 8) + 3, s_y + (y * 8) - 1);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 26) {
				SelectObject(hdc, blueOutlinePen);
				MoveToEx(hdc, s_x + (x * 8), s_y + (y * 8) + 4, NULL);
				LineTo(hdc, s_x + (x * 8) + 8, s_y + (y * 8) + 4);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 27) {
				SelectObject(hdc, whiteBrush);
				Rectangle(hdc, s_x + (x * 8), s_y + (y * 8) + 3, s_x + (x * 8) + 10, s_y + (y * 8) + 6);
			}
			else if (Map[y][x] == 28) {
				SelectObject(hdc, blueOutlinePen);
				MoveToEx(hdc, s_x + (x * 8), s_y + (y * 8) + 3, NULL);
				LineTo(hdc, s_x + (x * 8) + 8, s_y + (y * 8) + 3);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 29)
				DrawSprite(wall_corner_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 30)
				DrawSprite(wall_corner_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 31)
				DrawSprite(wall_corner_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 32)
				DrawSprite(wall_corner_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 50) {
				SelectObject(hdc, blueOutlinePen);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				RoundRect(hdc, s_x + (x * 8) + 4, s_y + (y * 8) + 4, s_x + (x * 8) + 36, s_y + (y * 8) + 20, 3, 3);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 51) {
				SelectObject(hdc, blueOutlinePen);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				RoundRect(hdc, s_x + (x * 8) + 4, s_y + (y * 8) + 4, s_x + (x * 8) + 28, s_y + (y * 8) + 20, 3, 3);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 52) {
				SelectObject(hdc, blueOutlinePen);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				RoundRect(hdc, s_x + (x * 8) + 4, s_y + (y * 8) + 4, s_x + (x * 8) + 28, s_y + (y * 8) + 12, 3, 3);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 53) {
				SelectObject(hdc, blueOutlinePen);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				RoundRect(hdc, s_x + (x * 8) + 4, s_y + (y * 8) + 4, s_x + (x * 8) + 12, s_y + (y * 8) + 36, 3, 3);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 54) {
				SelectObject(hdc, blueOutlinePen);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				RoundRect(hdc, s_x + (x * 8) + 4, s_y + (y * 8) + 4, s_x + (x * 8) + 36, s_y + (y * 8) + 12, 3, 3);
				SelectObject(hdc, outlinePen);
			}
			else if (Map[y][x] == 75)
				DrawSprite(Special_R, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 76)
				DrawSprite(Special_E, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 77)
				DrawSprite(Special_A, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 78)
				DrawSprite(Special_D, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 79)
				DrawSprite(Special_Y, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 80)
				DrawSprite(Special_EXCLAMATIONPOINT, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 90) {
				SelectObject(hdc, dotsBrush);
				Ellipse(hdc, s_x + (x * 8), s_y + (y * 8), s_x + (x * 8) + 9, s_y + (y * 8) + 9);
			}
		}
	}
}

void MovePacman(input dir) {
	if (!CollisionCheck(dir)) {
		Map[Y_old][X_old] = 00;
		X_old = X_pos;
		Y_old = Y_pos;
		if (CheckForDot(dir))
			CollectedDots++;
		switch (dir) {
		case UP:
			Y_pos--;
			break;
		case LEFT:
			X_pos--;
			break;
		case RIGHT:
			X_pos++;
			break;
		case DOWN:
			Y_pos++;
			break;
		default:
		case NONE:
			return;
			break;
		}
		if (X_pos >= mapWidth - 1)
			X_pos = mapWidth * 0 + 1;
		else if (X_pos <= mapWidth * 0)
			X_pos = mapWidth - 2;
		Map[Y_pos][X_pos] = 99;
		Map[Y_old][X_old] = 98;
	}
}

bool CollisionCheck(input dir) {
	switch (dir) {
	case UP:
		if (Map[Y_pos - 1][X_pos] != 00 && Map[Y_pos - 1][X_pos] != 98 && Map[Y_pos - 1][X_pos] != 01 && Map[Y_pos - 1][X_pos] != 90)
			return true;
		break;
	case DOWN:
		if (Map[Y_pos + 1][X_pos] != 00 && Map[Y_pos + 1][X_pos] != 98 && Map[Y_pos + 1][X_pos] != 01 && Map[Y_pos + 1][X_pos] != 90)
			return true;
		break;
	case LEFT:
		if (Map[Y_pos][X_pos - 1] != 00 && Map[Y_pos][X_pos - 1] != 98 && Map[Y_pos][X_pos - 1] != 01 && Map[Y_pos][X_pos - 1] != 90)
			return true;
		break;
	case RIGHT:
		if (Map[Y_pos][X_pos + 1] != 00 && Map[Y_pos][X_pos + 1] != 98 && Map[Y_pos][X_pos + 1] != 01 && Map[Y_pos][X_pos + 1] != 90)
			return true;
		break;
	default:
		return false;
	}
	return false;
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

double GetTime() {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9;
}

double GetTimeSince(double startTime) {
	return time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count() / 1e9 - startTime;
}

double wait(double waitTime) {
	double startTime = GetTime();

	while (waitTime > GetTimeSince(startTime)) {}

	return GetTimeSince(startTime + waitTime);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

#endif
