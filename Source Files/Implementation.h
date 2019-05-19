#pragma once
#include "DataStructures.h"

void GameDraw(); // Draws game
void GameInput(); // Sorts input
void GameLogic(); // Applies logic

bool KeyIsDown(char key, bool pressed = true, bool held = true);
double getTime();
double getTimeSince(double);
double wait(double);

void DrawMap(int X_pos, int Y_pos); // Outputs map using Map array and X and Y offset argument
COLORREF GetColor(int color_data); // Returns RGB value based off integer

TileSprites Game;
Pacman Pac;

/////////////////////////////////////////////////// -- Frame loop
void GameDraw() { // -- Draw
	DrawMap(20,20);
}
void GameInput() { // -- Get Input
	// Switches game input based off last button pressed, does not reset input each frame
	if (KeyIsDown('W', true, true) && !Pac.CollisionCheck(UP))
		game_input = UP;
	else if (KeyIsDown('S', true, true) && !Pac.CollisionCheck(DOWN))
		game_input = DOWN;
	else if (KeyIsDown('A', true, true) && !Pac.CollisionCheck(LEFT))
		game_input = LEFT;
	else if (KeyIsDown('D', true, true) && !Pac.CollisionCheck(RIGHT))
		game_input = RIGHT;
	if (game_input != NONE && Pac.adjpx != 0) { 
		// Adjusts pacman if no input has been made, centering him at origin
		Pac.adjpx = 0;
		Pac.clearorigin = true;
	}
}
void GameLogic() { // -- Apply Logic
	Pac.ChangePhase();
	Pac.MovePacman(game_input);
}
///////////////////////////////////////////////////
void Pacman::ChangePhase() {
	currentPhase++;
	if (currentPhase > 3)
		currentPhase = 1;
}
void Pacman::DrawPacman(const int sprite[8][8], int rotation_value, int X_position, int Y_position) {
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
int Pacman::GetRotationValue(input dir) {
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
void DrawMap(int s_x, int s_y) {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (Map[y][x] == 98) {
				if (!Pac.clearorigin)
					Pac.DrawPacman(Pac.clear, Pac.GetRotationValue(game_input), s_x + (x * 8) + Pac.adjpx, s_y + (y * 8) + 1);
				else {
					Pac.DrawPacman(Pac.clear, Pac.GetRotationValue(game_input), s_x + (x * 8) + 4, s_y + (y * 8) + 1);
					Pac.clearorigin = false;
				}
			}
			else if (Map[y][x] == 99) {
				switch (Pac.currentPhase) {
				case 1:
					Pac.DrawPacman(Pac.P1, Pac.GetRotationValue(game_input), s_x + (x * 8) + Pac.adjpx, s_y + (y * 8) + 1);
					break;
				case 2:
					Pac.DrawPacman(Pac.P2, Pac.GetRotationValue(game_input), s_x + (x * 8) + Pac.adjpx, s_y + (y * 8) + 1);
					break;
				case 3:
					Pac.DrawPacman(Pac.P3, Pac.GetRotationValue(game_input), s_x + (x * 8) + Pac.adjpx, s_y + (y * 8) + 1);
					break;
				}
				Pac.X_pos = x;
				Pac.Y_pos = y;
			}
			else if (Map[y][x] == 01)
				Game.DrawSprite(Game.dot, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 02)
				Game.DrawSprite(Game.wall, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 03)
				Game.DrawSprite(Game.wall, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 04)
				Game.DrawSprite(Game.wall_corner, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 05)
				Game.DrawSprite(Game.wall_corner, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 06)
				Game.DrawSprite(Game.wall_corner, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 07)
				Game.DrawSprite(Game.wall_corner_square, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 33)
				Game.DrawSprite(Game.wall_corner_square, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 34)
				Game.DrawSprite(Game.wall_corner_square, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 10)
				Game.DrawSprite(Game.wall_corner_square, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 11)
				Game.DrawSprite(Game.wall_corner, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 12)
				Game.DrawSprite(Game.wall_cross, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 13)
				Game.DrawSprite(Game.wall_cross, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 14)
				Game.DrawSprite(Game.wall_cross_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 15)
				Game.DrawSprite(Game.wall_cross_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 16)
				Game.DrawSprite(Game.wall_cross_two, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 17)
				Game.DrawSprite(Game.wall_cross_two, 5, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 18)
				Game.DrawSprite(Game.wall_cross_two, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 19)
				Game.DrawSprite(Game.wall_cross_two, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 20)
				Game.DrawSprite(Game.wall_cross_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 21)
				Game.DrawSprite(Game.wall_cross_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 22)
				Game.DrawSprite(Game.wall_cross_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 23)
				Game.DrawSprite(Game.wall_cross_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 24)
				Game.DrawSprite(Game.wall_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 25)
				Game.DrawSprite(Game.wall_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 26)
				Game.DrawSprite(Game.wall_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 27)
				Game.DrawSprite(Game.gate, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 28)
				Game.DrawSprite(Game.wall_thick, 2, s_x + (x * 8), s_y + (y * 8) - 1);
			else if (Map[y][x] == 29)
				Game.DrawSprite(Game.wall_corner_thick, 1, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 30)
				Game.DrawSprite(Game.wall_corner_thick, 4, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 31)
				Game.DrawSprite(Game.wall_corner_thick, 3, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 32)
				Game.DrawSprite(Game.wall_corner_thick, 2, s_x + (x * 8), s_y + (y * 8));
			else if (Map[y][x] == 00)
				continue;
			else
				Game.DrawSprite(Game.error, 1, s_x + (x * 8), s_y + (y * 8));
		}
	}
}
void Pacman::MovePacman(input dir) {
	if (!CollisionCheck(dir)) {
		Map[Y_old][X_old] = 00;
		X_old = X_pos;
		Y_old = Y_pos;
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
bool Pacman::CollisionCheck(input dir) {
	switch (dir) {
	case UP:
		if (Map[Y_pos - 1][X_pos] != 00 && Map[Y_pos - 1][X_pos] != 98 && Map[Y_pos - 1][X_pos] != 01)
			return true;
		break;
	case DOWN:
		if (Map[Y_pos + 1][X_pos] != 00 && Map[Y_pos + 1][X_pos] != 98 && Map[Y_pos + 1][X_pos] != 01)
			return true;
		break;
	case LEFT:
		if (Map[Y_pos][X_pos - 1] != 00 && Map[Y_pos][X_pos - 1] != 98 && Map[Y_pos][X_pos - 1] != 01)
			return true;
		break;
	case RIGHT:
		if (Map[Y_pos][X_pos + 1] != 00 && Map[Y_pos][X_pos + 1] != 98 && Map[Y_pos][X_pos + 1] != 01)
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
	else if (color_data == 9)
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
