#include "Game.h"

void Game::DrawBoard() {
	for (int y = 0; y < BH; y++) {
		for (int x = 0; x < BW; x++) {
			if (Board[y][x] == 324 && EnergizerFlashActive)
				DrawSprite(x * res, y * res, 402);
			else if (Board[y][x] == 324 && !EnergizerFlashActive)
				DrawSprite(x * res, y * res, Board[y][x]);
			else if ((Board[y][x] == 235 || Board[y][x] == 226 || Board[y][x] == 221) && y == 0 && _1UPFlashActive)
				DrawSprite(x * res, y * res, 402);
			else if ((Board[y][x] == 235 || Board[y][x] == 226 || Board[y][x] == 221) && y == 0 && !_1UPFlashActive)
				DrawSprite(x * res, y * res, Board[y][x]);
			else if (Board[y][x] > 000)
				DrawSprite(x * res, y * res, Board[y][x]);
		}
	}
}

bool Game::GameStatus() const {
	return EndGame;
}

void Game::Draw() {
	DrawBoard();
	DrawCounter(0, 1, Score); // Score
	DrawCounter(10, 1, HighScore); // Highscore
}
void Game::Input() {
	if (KeyIsDown('W', true, true) && !Player.CollisionCheck(UP))
		Player.Face = UP;
	if (KeyIsDown('A', true, true) && !Player.CollisionCheck(LEFT))
		Player.Face = LEFT;
	if (KeyIsDown('S', true, true) && !Player.CollisionCheck(DOWN))
		Player.Face = DOWN;
	if (KeyIsDown('D', true, true) && !Player.CollisionCheck(RIGHT))
		Player.Face = RIGHT;
}
void Game::Logic() {
	UpdateTS(EnergizerFlashActive, EnergizerFlashTS, .2);
	UpdateTS(_1UPFlashActive, _1UPFlashTS, .4);
	Player.UpdatePhase();
	Player.DrawPlayer();
	MovePacman();
}
bool Game::UpdateTS(bool & State, double & TS, double Time) {
	if (GetTimeSince(TS) > Time) {
		TS = GetTime();
		State = !State;
		return true;
	}
	return false;
}
void Game::DrawCounter(int xpos, int ypos, int value) {
	int Digits[7] = { 0,0,0,0,0,0,0 };
	for (int i = 0, j = 7; i < 7 && j > -1; i++) {
		Digits[i] = value / int(pow(10, j - 1));
		value -= int(pow(10, j - 1)) * Digits[i];
		j--;
	}
	bool NumberStart = false;
	for (int i = 0; i < 7; i++) {
		if (Digits[i] > 0 || i > 4)
			NumberStart = true;
		if (NumberStart)
			SetTile(xpos + i, ypos, NumberToSprite(Digits[i]));
	}
}

void Game::SetTile(int xpos, int ypos, int ID) {
	Board[ypos][xpos] = ID;
}

int Game::NumberToSprite(int value) const {
	return value + 234;
}

void Game::DrawLives(int L) {
	if (L > 5)
		L = 5;
	for (int i = 0; i < L; i++) {
		if (i == 0)
			continue;
		SetTile(0 + (i * 2), 34, 403);
		SetTile(1 + (i * 2), 34, 404);
		SetTile(0 + (i * 2), 35, 405);
		SetTile(1 + (i * 2), 35, 406);
	}
}

void Game::DrawLevel(int L) {
	int R[2] = { 0,0 };
	if (L < 8) {
		R[0] = 1;
		R[1] = L;
	}
	else if (L > 7 && L < 19) {
		R[0] = L - 6;
		R[1] = L;
	}
	else if (L > 18) {
		R[0] = 13;
		R[1] = 19;
	}
	for (int i = R[0] - 1, x = 24; i < R[1]; i++) {
		SetTile(x, 34, FruitTable[i]);
		SetTile(x + 1, 34, FruitTable[i] +  1);
		SetTile(x, 35, FruitTable[i] +  2);
		SetTile(x + 1, 35, FruitTable[i] +  3);
		x -= 2;
	}
}

void Game::MovePacman() {
	switch (Player.Face) {
	case UP:
		if (Player.UpdateMovement() && !Player.CollisionCheck(UP))
		for (int i = 0; i < 2; i++) {
			Player.Y--;
			Player.DrawPlayer();
		}
		break;
	case LEFT:
		if (Player.UpdateMovement() && !Player.CollisionCheck(LEFT))
			for (int i = 0; i < 2; i++) {
				Player.X--;
				Player.DrawPlayer();
			}
		break;
	case RIGHT:
		if (Player.UpdateMovement() && !Player.CollisionCheck(RIGHT))
			for (int i = 0; i < 2; i++) {
				Player.X++;
				Player.DrawPlayer();
			}
		break;
	case DOWN:
		if (Player.UpdateMovement() && !Player.CollisionCheck(DOWN))
			for (int i = 0; i < 2; i++) {
				Player.Y++;
				Player.DrawPlayer();
			}
		break;
	}
}

void Game::InitializeCollision() {
	for (int y = 0; y < BH; y++) {
		for (int x = 0; x < BW; x++) {
			if (Board[y][x] >= 247 && Board[y][x] <= 321)
				for (int i = y * 16; i < y *  16 + 16; i++)
					for (int j = x * 16; j < x * 16 +16; j++)
						Player.BoardCollision[i][j] = 1;
			else 
				for (int i = y * 16; i < y * 16 + 16; i++)
					for (int j = x * 16; j < x * 16 + 16; j++)
						Player.BoardCollision[i][j] = 0;
		}
	}
}
