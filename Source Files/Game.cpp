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
	DrawBoard(); // Main Map Drawing Function
	DrawCounter(0, 1, Score); // Score Display
	DrawCounter(10, 1, HighScore); // Highscore Display
	DrawLives(); // Lives Display
	Player.DrawPlayer();
	if (DisplayFPS) // Debug
		DrawCounter(2, 2, GetFPS());
	if (!GameStarted)
		GetReady();
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
	if (KeyIsDown('1', true, false) && InstaEnd) // Debug
		DotsCollected = 244;
	if (KeyIsDown('2', true, false) && InstaLive) // Debug
		Lives++;
	if (KeyIsDown('3', true, false) && InstaDeath) // Debug
		KillPlayer();
}
void Game::Logic() {
	UpdateTS(EnergizerFlashActive, EnergizerFlashTS, .2);
	UpdateTS(_1UPFlashActive, _1UPFlashTS, .4);
	Player.UpdatePhase();
	MovePacman();
	TeleportPlayer();
	EatObjects();
	LevelEnd();
	if (HighScore < Score)
		HighScore = Score;
	if ((DotsCollected == 70 && !Fruit1) || (DotsCollected == 170 && !Fruit2)) {
		FruitAppear = true;
		FruitTS = GetTime();
		if (DotsCollected == 70)
			Fruit1 = true;
		else
			Fruit2 = true;
	}
	if (FruitAppear) {
		SpawnFruit();
	}
	if (Player.Y == 320 && Player.X >= 208 && Player.X <= 224 && FruitAppear) {
		F_PointAppear = true;
		F_PointTS = GetTime();
		FruitAppear = false;
		FruitScore();
	}
	PointsDraw();
	if (GetTimeSince(FruitTS) > 9 && FruitAppear) {
		FruitTS = 0;
		FruitAppear = false;
		DrawSprite(216, 320, 402);
	}
	if (LastLifeEarned <= Score - 10000) {
		LastLifeEarned = Score;
		Lives++;
	}
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

void Game::DrawLives() {
	if (Lives > 5)
		Lives = 5;
	for (int i = 0; i < Lives; i++) {
		if (i == 0)
			continue;
		SetTile(0 + (i * 2), 34, 403);
		SetTile(1 + (i * 2), 34, 404);
		SetTile(0 + (i * 2), 35, 405);
		SetTile(1 + (i * 2), 35, 406);
	}
}

void Game::DrawLevel() {
	int R[2] = { 0,0 };
	if (Level < 8) {
		R[0] = 1;
		R[1] = Level;
	}
	else if (Level > 7 && Level < 19) {
		R[0] = Level - 6;
		R[1] = Level;
	}
	else if (Level > 18) {
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
			if (Board[y][x] == 322)
			for (int i = y * 16; i < y * 16 + 16; i++)
				for (int j = x * 16; j < x * 16 + 16; j++)
					Player.BoardCollision[i][j] = 2;
			else if (Board[y][x] == 324)
				for (int i = y * 16; i < y * 16 + 16; i++)
					for (int j = x * 16; j < x * 16 + 16; j++)
						Player.BoardCollision[i][j] = 3;
			else if (Board[y][x] >= 247 && Board[y][x] <= 321)
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

void Game::TeleportPlayer() {
	if (Player.Y == 272) {
		if (Player.X == 430) {
			DrawSprite(Player.X, Player.Y, 402);
			Player.X = 4;
		}
		else if (Player.X == 2) {
			DrawSprite(Player.X, Player.Y, 402);
			Player.X = 428;
		}
	}
}

void Game::EatObjects() {
	switch (Player.Face) {
	case LEFT:
		if (Player.ActiveTileCheck(LEFT) == 2) {
			ClearCollisionTile(Player.X / 16, Player.Y / 16);
			SetTile(Player.X / 16, Player.Y / 16, 000);
			DrawSprite((Player.X / 16) * 16, Player.Y, 439);
			DotsCollected++;
			Score += 10;
		}
		else if (Player.ActiveTileCheck(LEFT) == 3) {
			ClearCollisionTile(Player.X / 16, Player.Y / 16);
			SetTile(Player.X / 16, Player.Y / 16, 000);
			DrawSprite((Player.X / 16) * 16, Player.Y, 402);
			DotsCollected++;
			Score += 50;
		}
		break;
	case RIGHT:
		if (Player.ActiveTileCheck(RIGHT) == 2) {
			ClearCollisionTile(Player.X / 16 + 1, Player.Y / 16);
			SetTile(Player.X / 16 + 1, Player.Y / 16, 000);
			DrawSprite(((Player.X / 16) + 1) * 16, Player.Y, 439);
			DotsCollected++;
			Score += 10;
		}
		if (Player.ActiveTileCheck(RIGHT) == 3) {
			ClearCollisionTile(Player.X / 16 + 1, Player.Y / 16);
			SetTile(Player.X / 16 + 1, Player.Y / 16, 000);
			DrawSprite(((Player.X / 16) + 1) * 16, Player.Y, 402);
			DotsCollected++;
			Score += 50;
		}
		break;
	case UP:
		if (Player.ActiveTileCheck(LEFT) == 2) {
			ClearCollisionTile(Player.X / 16, Player.Y / 16);
			SetTile(Player.X / 16, Player.Y / 16, 000);
			DrawSprite(Player.X, (Player.Y / 16) * 16, 439);
			DotsCollected++;
			Score += 10;
		}
		if (Player.ActiveTileCheck(LEFT) == 3) {
			ClearCollisionTile(Player.X / 16, Player.Y / 16);
			SetTile(Player.X / 16, Player.Y / 16, 000);
			DrawSprite(Player.X, (Player.Y / 16) * 16, 402);
			DotsCollected++;
			Score += 50;
		}
		break;
	case DOWN:
		if (Player.ActiveTileCheck(LEFT) == 2) {
			ClearCollisionTile(Player.X / 16, Player.Y / 16 + 1);
			SetTile(Player.X / 16, Player.Y / 16 + 1, 000);
			DrawSprite(Player.X, ((Player.Y / 16) + 1) * 16, 439);
			DotsCollected++;
			Score += 10;
		}
		if (Player.ActiveTileCheck(LEFT) == 3) {
			ClearCollisionTile(Player.X / 16, Player.Y / 16 + 1);
			SetTile(Player.X / 16, Player.Y / 16 + 1, 000);
			DrawSprite(Player.X, ((Player.Y / 16) + 1) * 16, 402);
			DotsCollected++;
			Score += 50;
		}
		break;
	}
}

void  Game::ClearCollisionTile(int xpos, int ypos) {
	for (int i = ypos * 16; i < ypos * 16 + 16; i++)
		for (int j = xpos * 16; j < xpos * 16 + 16; j++)
			Player.BoardCollision[i][j] = 0;
}

int Game::GetFPS() {
	FPS++;
	if (GetTimeSince(FPSTS) >= 1) {
		LastFPS = FPS;
		FPS = 0;
		FPSTS = GetTime();
	}
	return LastFPS;
}

void Game::LevelEnd() {
	if (DotsCollected == 244) {
		Wait(2);
		for (int i = 0; i < 8; i++) {
			Wait(.15);
			Flash();
		}
		EndGame = true;
	}
}

void Game::Flash() {
	if (Board[3][0] == 287) {
		for (int y = 0; y < BH; y++) {
			for (int x = 0; x < BW; x++) {
				if (Board[y][x] >= 286 && Board[y][x] <= 321)
					Board[y][x] -= 39;
			}
		}
	}
	else if (Board[3][0] == 248) {
		for (int y = 0; y < BH; y++) {
			for (int x = 0; x < BW; x++) {
				if (Board[y][x] >= 247 && Board[y][x] <= 282)
					Board[y][x] += 39;
			}
		}
	}
	DrawBoard();
}

void Game::KillPlayer() {
	Wait(2);
	DrawSprite(Player.X, Player.Y, 402);
	for (int i = 343; i <= 352; i++) {
		DrawSprite(Player.X, Player.Y, i);
		Wait(.13);
	}
	DrawSprite(Player.X, Player.Y, 440);
	Wait(.13);
	DrawSprite(Player.X, Player.Y, 402);
	Death = true;
	Lives--;
	EndGame = true;
}

void Game::GetReady() {
	SetTile(11, 20, 182);
	SetTile(12, 20, 169);
	SetTile(13, 20, 165);
	SetTile(14, 20, 168);
	SetTile(15, 20, 189);
	SetTile(16, 20, 191);
	DrawBoard();
	Wait(3.5);
	DrawBoard();
	for (int i = 0; i < 6; i++) {
		SetTile(11 + i, 20, 000);
		DrawSprite((11 + i) * 16, 320, 402);
	}
	GameStarted = true;
}

void Game::SpawnFruit() {
	switch (Level) {
	case 1:
		DrawSprite(216, 320, 353);
		break;
	case 2:
		DrawSprite(216, 320, 354);
		break;
	case 3:
	case 4:
		DrawSprite(216, 320, 355);
		break;
	case 5:
	case 6:
		DrawSprite(216, 320, 356);
		break;
	case 7:
	case 8:
		DrawSprite(216, 320, 357);
		break;
	case 9:
	case 10:
		DrawSprite(216, 320, 358);
		break;
	case 11:
	case 12:
		DrawSprite(216, 320, 359);
		break;
	default:
		DrawSprite(216, 320, 360);
		break;

	}
}

void Game::PointsDraw() {
	if (F_PointAppear) {
		switch (Level) {
		case 1:
			DrawSprite(216, 320, 329);
			break;
		case 2:
			DrawSprite(216, 320, 330);
			break;
		case 3:
		case 4:
			DrawSprite(216, 320, 331);
			break;
		case 5:
		case 6:
			DrawSprite(216, 320, 332);
			break;
		case 7:
		case 8:
			DrawSprite(216, 320, 329);
			DrawSprite(232, 320, 333);
			break;
		case 9:
		case 10:
			DrawSprite(216, 320, 441);
			DrawSprite(232, 320, 333);
			break;
		case 11:
		case 12:
			DrawSprite(216, 320, 331);
			DrawSprite(232, 320, 333);
			break;
		default:
			DrawSprite(216, 320, 332);
			DrawSprite(232, 320, 333);
			break;

		}
	}
	if (GetTimeSince(F_PointTS) > 1 && F_PointAppear) {
		F_PointTS = 0;
		F_PointAppear = false;
		DrawSprite(216, 320, 402);
		DrawSprite(232, 320, 402);
	}
}
void Game::FruitScore() {
	switch (Level) {
	case 1:
		Score += 100;
		break;
	case 2:
		Score += 300;
		break;
	case 3:
	case 4:
		Score += 500;
		break;
	case 5:
	case 6:
		Score += 700;
		break;
	case 7:
	case 8:
		Score += 1000;
		break;
	case 9:
	case 10:
		Score += 2000;
		break;
	case 11:
	case 12:
		Score += 3000;
		break;
	default:
		Score += 5000;
		break;

	}
}
