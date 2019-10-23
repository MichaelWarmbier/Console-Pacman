#include "Header.h"

int main() {
	SetWindowDimensions(29, 39);
	int carried_level = 1, carried_lives = 3;
	int carried_board_data[MH][MW];
	do {
		COORD coord; coord.X = 0; coord.Y = 0;
		Game* Pacman = new Game(carried_level, carried_lives);
		Pacman->InitializeBoard(carried_board_data);
		newGame = false;
		system("CLS");
		while ((timer += (dt = FPS + Wait(FPS))) && !EXIT_GAME_F) {
			if (Pacman->StateOfGame != END)
				Pacman->Draw();
			Pacman->Input();
			Pacman->Logic();
			while (GetConsoleWindow() != GetForegroundWindow()) {}
		}
		if (Pacman->StateOfGame == END) {
			if (Pacman->PlayerLives <= 1)
				EXIT_PROGRAM_F = true;
			carried_lives--;
			CopyBoardData(Pacman->Board, carried_board_data);
		}
		if (!EXIT_PROGRAM_F) {
			if (Pacman->StateOfGame != END)
				carried_level++;
		}
		EXIT_GAME_F = false;
		delete Pacman;
	} while (!EXIT_PROGRAM_F);
	return EXIT_SUCCESS;
	return 0;
}

/////////////////////////////////////////////////////
// Game Class Implementation
/////////////////////////////////////////////////////

void Game::Draw() {
	DrawBoard();
	DrawLives();
	DrawLevelDisplay();
	DisplayInteger(1, 1, score);
	DisplayInteger(11, 1, 0);
	if (ShowRemainingPellets)
		DisplayInteger(10, MH - 2, collected_dots);
	P1.Draw();
}

void Game::GameOver() {
	Wait(2);
	double _TS = GetTime();
	DrawSprite(1, Blinky->X, Blinky->Y);
	DrawSprite(1, Pinky->X, Pinky->Y);
	for (int i = 0; i < 12; i++) {
		DrawSprite(1, pX, pY);
		DrawSprite(174 + i, pX, pY);
		Wait(.15);
	}
	Wait(2);
	EXIT_GAME_F = true;
	StateOfGame = END;
}

void Game::InitializeColBoard() {
	for (int i = 0; i < MH; i++) {
		for (int j = 0; j < MW; j++) {
			if (Board[i][j] != 01 && Board[i][j] != 78 && Board[i][j] != 79 && Board[i][j] != 97)
				for (int k = i * 16; k < i * 16 + 16; k++) {
					for (int h = j * 16; h < j * 16 + 16; h++) {
						CollisionBoard[k][h] = 1;
					}
				}
			else if (Board[i][j] == 78 || Board[i][j] == 79 || Board[i][j] == 92)
				for (int k = i * 16; k < i * 16 + 16; k++) {
					for (int h = j * 16; h < j * 16 + 16; h++) {
						CollisionBoard[k][h] = 2;
					}
				}
			else
				for (int k = i * 16; k < i * 16 + 16; k++) {
					for (int h = j * 16; h < j * 16 + 16; h++) {
						CollisionBoard[k][h] = 0;
					}
				}
		}
	}
}

void Game::Logic() {

	ToggleModes();

	Blinky->Logic();
	Pinky->Logic();

	if ((GetDistanceOf(Blinky->X, Blinky->Y, pX, pY) < 6 || GetDistanceOf(Pinky->X, Pinky->Y, pX, pY) < 6) && CanGameOver)
		GameOver();

	if (collected_dots == 0)
		StateOfGame = AFTER;
	if (StateOfGame == BEFORE) {
		ChangeTile(11, 20, 99);
		ChangeTile(12, 20, 100);
		ChangeTile(13, 20, 101);
		ChangeTile(14, 20, 102);
		ChangeTile(15, 20, 103);
		ChangeTile(16, 20, 104);
		for (int i = 0; i < 3; i++) {
			Draw();
			P1.Draw();
		}
		Wait(3);
		ToggleReady();
		StateOfGame = DURING;
	}
	if (StateOfGame == AFTER) {
		Wait(1.5);
		for (int i = 0; i < 8; i++) {
			Wait(0.3);
			ToggleMapColor();
			Draw();
		}
		EXIT_GAME_F = true;
		newGame = true;
	}
	if (StateOfGame == DURING) {
		Input();
		for (double i = 0; i < P1.speed; i += .2) {
			if (OldInput != PlayerInput && PlayerInput != NONE)
				OldInput = PlayerInput;
			if (!GetCol(pX - 1, pY, 1) && !GetCol(pX - 1, pY + 15, 1) && (PlayerInput == LEFT || OldInput == LEFT)) {
				pX--;
				PlayerInput = LEFT;
			}
			else if (!GetCol(pX + 16, pY, 1) && !GetCol(pX + 16, pY + 15, 1) && (PlayerInput == RIGHT || OldInput == RIGHT)) {
				pX++;
				PlayerInput = RIGHT;
			}
			else if (!GetCol(pX, pY - 1, 1) && !GetCol(pX + 15, pY - 1, 1) && (PlayerInput == UP || OldInput == UP)) {
				pY--;
				PlayerInput = UP;
			}
			else if (!GetCol(pX, pY + 16, 1) && !GetCol(pX + 15, pY + 16, 1) && (PlayerInput == DOWN || OldInput == DOWN)) {
				pY++;
				PlayerInput = DOWN;
			}
			Teleport();
			P1.Draw();
		}
		if (GetTimeSince(_1UP_TS) > .3) {
			Toggle1UP();
			UpdateTS(_1UP_TS);
		}
		if (GetTimeSince(_PP_TS) > .2) {
			TogglePP();
			UpdateTS(_PP_TS);
		}
		if (GetTimeSince(P1._PhaseTS) > .08) {
			P1.MayUpdate = true;
			switch (PlayerInput) {
			case UP:
				if (GetCol(pX, pY - 16, 1))
					P1.MayUpdate = false;
				break;
			case RIGHT:
				if (GetCol(pX + 16, pY, 1))
					P1.MayUpdate = false;
				break;
			case LEFT:
				if (GetCol(pX - 16, pY, 1))
					P1.MayUpdate = false;
				break;
			case DOWN:
				if (GetCol(pX, pY + 16, 1))
					P1.MayUpdate = false;
				break;
			}
			P1.TogglePhase();
			UpdateTS(P1._PhaseTS);
		}
		DotGrab();
		CheckDotData();
	}
}

void Game::InitializeBoard(int data[MH][MW]) {
	if (!newGame) {
		CopyBoardData(data, Board);
		newGame = true;
	}
}

Game::Game() {
	InitializeColBoard();
	UpdateTS(_1UP_TS);
	UpdateTS(_PP_TS);
	UpdateTS(_SWAP_TS);
	PlayerLives = 3;
	PlayerLevel = 1;
	StateOfGame = BEFORE;
	collected_dots = 244;
	score = 0;
}
Game::Game(int level, int lives) {
	InitializeColBoard();
	UpdateTS(_1UP_TS);
	UpdateTS(_PP_TS);
	UpdateTS(_SWAP_TS);
	PlayerLives = lives;
	PlayerLevel = level;
	StateOfGame = BEFORE;
	collected_dots = 244;
	score = 0;
}

void Game::TurnAround() {
	if (Blinky->face == RIGHT)
		Blinky->face = LEFT;
	else if (Blinky->face == LEFT)
		Blinky->face = RIGHT;
	else if (Blinky->face == UP)
		Blinky->face = DOWN;
	else if (Blinky->face == DOWN)
		Blinky->face = UP;
	if (Pinky->face == RIGHT)
		Pinky->face = LEFT;
	else if (Pinky->face == LEFT)
		Pinky->face = RIGHT;
	else if (Pinky->face == UP)
		Pinky->face = DOWN;
	else if (Pinky->face == DOWN)
		Pinky->face = UP;
}

void Game::ToggleModes() {
	if (PlayerLevel < 3 && PlayerLevel >= 1) {
		if (GetTimeSince(_SWAP_TS) > 3 && S[0]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[0] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 11 && S[1]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[1] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 31 && S[2]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[2] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 38 && S[3]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[3] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 58 && S[4]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[4] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 63 && S[5]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[5] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 83 && S[6]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[6] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 88 && S[7]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[7] = 0;
		}
	}
	else if (PlayerLevel < 5 && PlayerLevel >= 2) {
		if (GetTimeSince(_SWAP_TS) > 3 && S[0]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[0] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 10 && S[1]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[1] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 30 && S[2]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[2] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 37 && S[3]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[3] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 57 && S[4]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[4] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 62 && S[5]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[5] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 1115 && S[6]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[6] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 1115.01666666666 && S[7]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[7] = 0;
		}
	}
	else if (PlayerLevel >= 5) {
		if (GetTimeSince(_SWAP_TS) > 3 && S[0]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[0] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 8 && S[1]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[1] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 28 && S[2]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[2] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 33 && S[3]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[3] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 54 && S[4]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[4] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 58 && S[5]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[5] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 1095 && S[6]) {
			Blinky->state = SCATTER;
			Pinky->state = SCATTER;
			TurnAround();
			S[6] = 0;
		}
		if (GetTimeSince(_SWAP_TS) > 1095.01666666666 && S[7]) {
			Blinky->state = CHASE;
			Pinky->state = CHASE;
			TurnAround();
			S[7] = 0;
		}
	}
}

void Game::DrawLives() {
	for (int i = 0; i < 5; i++) {
		if (i < PlayerLives - 1)
			ChangeTile(i + 3, MH - 2, 93);
		else {
			ChangeTile(i + 3, MH - 2, 1);
			DrawSprite(1, (i + 3) * 16, (MH - 2) * 16);
		}
	}
}

void Game::Toggle1UP() {
	if (GetTileID(3, 0) == 98) {
		ChangeTile(3, 0, 03);
		ChangeTile(4, 0, 32);
		ChangeTile(5, 0, 27);
	}
	else if (GetTileID(3, 0) == 03) {
		ChangeTile(3, 0, 98);
		ChangeTile(4, 0, 98);
		ChangeTile(5, 0, 98);
	}
}

void Game::ToggleReady() {
	for (int i = 0; i < 6; i++) {
		ChangeTile(11 + i, 20, 98);
		Draw();
		ChangeTile(11 + i, 20, 1);
	}
}

void Game::TogglePP() {
	if (GetTileID(1, 6) == 97)
		ChangeTile(1, 6, 79);
	else if (GetTileID(1, 6) == 79)
		ChangeTile(1, 6, 97);
	if (GetTileID(26, 6) == 97)
		ChangeTile(26, 6, 79);
	else if (GetTileID(26, 6) == 79)
		ChangeTile(26, 6, 97);
	if (GetTileID(1, 26) == 97)
		ChangeTile(1, 26, 79);
	else if (GetTileID(1, 26) == 79)
		ChangeTile(1, 26, 97);
	if (GetTileID(26, 26) == 97)
		ChangeTile(26, 26, 79);
	else if (GetTileID(26, 26) == 79)
		ChangeTile(26, 26, 97);
}

void Game::DrawLevelDisplay() {
	int FruitBox[19] = { 80, 81, 82, 82, 83, 83, 84, 84, 85, 85, 86, 86, 87, 87, 87, 87, 87, 87, 87 };
	int Range[2] = { 0,0 };
	if (PlayerLevel < 8) {
		Range[0] = 1; Range[1] = PlayerLevel;
	}
	else if (PlayerLevel < 19 && PlayerLevel > 7) {
		Range[0] = PlayerLevel - 6; Range[1] = PlayerLevel;
	}
	else if (PlayerLevel > 18) {
		Range[0] = 13; Range[1] = 19;
	}
	int j = 0;
	for (int i = Range[0] - 1; i < Range[1]; i++, j++)
		ChangeTile(MW - 3 - j, MH - 2, FruitBox[i]);
}

void Game::DisplayInteger(int x, int y, int integer) {
	int n = integer;
	if (n > 9999999)
		n = 9999999;
	int Digit[7] = { 0,0,0,0,0,0,0 };
	for (int i = 6; i > -1; i--) {
		if (i != 0)
			Digit[i] = n / int(pow(10, i));
		n -= Digit[i] * int(pow(10, i));
		if (i == 0)
			Digit[i] = n;
		ChangeTile(x + abs(i - 6), y, Digit[i] + 2);
	}
}

void Game::ChangeTile(int gridx, int gridy, int newTileID) {
	Board[gridy][gridx] = newTileID;
}

void Game::CheckDotData() {
	collected_dots = 0;
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			if (GetTileID(x, y) == 79 || GetTileID(x, y) == 78 || GetTileID(x, y) == 97) {
				collected_dots++;
			}
		}
	}
}

void Game::DotGrab() {
	switch (PlayerInput) {
	case LEFT:
		if (GetCol(pX + 6, pY + 4, 2)) {
			ChangeTile(static_cast<int>(pX + 6) / 16, (static_cast<int>(pY) + 4) / 16, 0);
		}
		break;
	case RIGHT:
		if (GetCol(pX + 6, pY + 4, 2)) {
			ChangeTile(static_cast<int>(pX + 6) / 16, (static_cast<int>(pY) + 4) / 16, 0);
		}
		break;
	case UP:
		if (GetCol(pX + 4, pY + 6, 2)) {
			ChangeTile(static_cast<int>(pX + 4) / 16, (static_cast<int>(pY) + 6) / 16, 0);
		}
		break;
	case DOWN:
		if (GetCol(pX + 4, pY + 6, 2)) {
			ChangeTile(static_cast<int>(pX + 4) / 16, (static_cast<int>(pY) + 6) / 16, 0);
		}
		break;
	}
}

void Game::DrawBoard() {
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			if (Board[y][x] == 01)
				continue;
			DrawSprite(Board[y][x], x * 16.0, y * 16.0);
		}
	}
}

int Game::GetTileID(int gridx, int gridy) const {
	return Board[gridy][gridx];
}

void Game::ToggleMapColor() {
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			if (GetTileID(x, y) < 77 && GetTileID(x, y) > 43)
				Board[y][x] += 61;
			else if (GetTileID(x, y) < 138 && GetTileID(x, y) > 104)
				Board[y][x] -= 61;
		}
	}
}

void Game::Teleport() {
	if (pY == 272) {
		if (pX == 432) {
			DrawSprite(1, pX, pY);
			pX = 0;
		}
		else if (pX == 0) {
			DrawSprite(1, pX, pY);
			pX = 432;
		}
	}
}

bool Game::GetCol(int x, int y, int ID) const {
	if (CollisionBoard[y][x] == ID)
		return true;
	return false;
}

void Game::Input() {
	// Alt controls TBA
	if (KeyIsDown('W', true, true) && !GetCol(pX, pY - 1, 1) && !GetCol(pX + 15, pY - 1, 1))
		PlayerInput = UP;
	if (KeyIsDown('S', true, true) && !GetCol(pX, pY + 16, 1) && !GetCol(pX + 15, pY + 16, 1))
		PlayerInput = DOWN;
	if (KeyIsDown('A', true, true) && !GetCol(pX - 1, pY, 1) && !GetCol(pX - 1, pY + 15, 1))
		PlayerInput = LEFT;
	if (KeyIsDown('D', true, true) && !GetCol(pX + 16, pY, 1) && !GetCol(pX + 16, pY + 15, 1))
		PlayerInput = RIGHT;

}

/////////////////////////////////////////////////////
// Player Class Implementation
/////////////////////////////////////////////////////

Game::Player::Player() {
	SpritePhase = 2;
	_PhaseTS = GetTime();
	pX = 13 * 16.0 + 8;
	pY = 26 * 16.0;
	PlayerInput = NONE;
	speed = .4;
}

void Game::Player::TogglePhase() {
	if (MayUpdate)
		SpritePhase++;
	if (SpritePhase > 3)
		SpritePhase = 1;
}

void Game::Player::Draw() const {
	switch (SpritePhase) {
	case 1:
		DrawSprite(88, pX, pY);
		break;
	case 2:
		switch (PlayerInput) {
		case LEFT:
			DrawSprite(93, pX, pY);
			break;
		case RIGHT:
			DrawSprite(89, pX, pY);
			break;
		case UP:
			DrawSprite(95, pX, pY);
			break;
		case DOWN:
			DrawSprite(91, pX, pY);
			break;
		}
		break;
	case 3:
		switch (PlayerInput) {
		case LEFT:
			DrawSprite(94, pX, pY);
			break;
		case RIGHT:
			DrawSprite(90, pX, pY);
			break;
		case UP:
			DrawSprite(96, pX, pY);
			break;
		case DOWN:
			DrawSprite(92, pX, pY);
			break;
		}
		break;
	}
}

/////////////////////////////////////////////////////
// Ghost Class Implementation
/////////////////////////////////////////////////////

// BLINKY

void Game::G1::Draw() {
	switch (face) {
	case RIGHT:
		DrawSprite(138 + phase, X, Y);
		break;
	case LEFT:
		DrawSprite(144 + phase, X, Y);
		break;
	case DOWN:
		DrawSprite(140 + phase, X, Y);
		break;
	case UP:
		DrawSprite(142 + phase, X, Y);
		break;
	}
}

void Game::G1::TogglePhase() {
	phase = !phase;
}

void Game::G1::Teleport() {
	if (Y == 272) {
		if (X == 432) {
			DrawSprite(1, X, Y);
			X = 0;
		}
		else if (X == 0) {
			DrawSprite(1, X, Y);
			X = 432;
		}
	}
}

void Game::G1::Advance() {
	double mSpeed = speed;
	if (Y == 272 && ((X <= 96 && X >= 0) || (X >= 336 && X <= 432)))
		mSpeed = speed / 2;
	for (double i = 0; i < mSpeed; i += .2) {
		if (X % 16 == 0 && Y % 16 == 0 && i != 0)
			break;
		switch (face) {
		case RIGHT:
			X++;
			break;
		case LEFT:
			X--;
			break;
		case DOWN:
			Y++;
			break;
		case UP:
			Y--;
			break;
		}
		Draw();
	}
}

void Game::G1::Logic() {
	AI();
	Draw();
	if (GetTimeSince(_PhaseTS))
		TogglePhase();
	if ((X % 16 == 0 && Y % 16 == 0))
		FindPath();
	Advance();
	Teleport();
}

void Game::G1::AI() {
	switch (state) {
	case CHASE:
	case PANIC:
		tX = pX;
		tY = pY;
		break;
	case SCATTER:
		tX = 384;
		tY = -16;
		break;
	}
}

void Game::G1::FindPath() {
	bool D = true, U = true, L = true, R = true;
	double D_Down = -1, D_Up = -1, D_Left = -1, D_Right = -1;
	double top_value = 0;

	if (face == DOWN || !CheckTile(X / 16, Y / 16 - 1) || ((Y == 416 || Y == 224) && X >= 176 && X <= 272)) // Up Check
		U = false;
	else
		D_Up = GetDistanceOf(X, Y - 16, tX, tY);
	if (face == UP || !CheckTile(X / 16, Y / 16 + 1) || (Y == 224 && X >= 176 && X <= 272)) // Down Check
		D = false;
	else
		D_Down = GetDistanceOf(X, Y + 16, tX, tY);
	if (face == RIGHT || !CheckTile(X / 16 - 1, Y / 16)) // Left Check
		L = false;
	else
		D_Left = GetDistanceOf(X - 16, Y, tX, tY);
	if (face == LEFT || !CheckTile(X / 16 + 1, Y / 16)) // Right Check
		R = false;
	else
		D_Right = GetDistanceOf(X + 16, Y, tX, tY);

	if (U && D_Up != -1)
		top_value = D_Up;
	if ((D_Right <= top_value || top_value == 0) && R && D_Right != -1)
		top_value = D_Right;
	if ((D_Down <= top_value || top_value == 0) && D && D_Down != -1)
		top_value = D_Down;
	if ((D_Left <= top_value || top_value == 0) && L && D_Left != -1)
		top_value = D_Left;

	if (top_value == D_Up)
		face = UP;
	else if (top_value == D_Right)
		face = RIGHT;
	else if (top_value == D_Down)
		face = DOWN;
	else if (top_value == D_Left)
		face = LEFT;

}

bool Game::G1::CheckTile(int x, int y) {
	if (GhostBoard[y][x] == 78 || GhostBoard[y][x] == 79 || GhostBoard[y][x] == 00 || GhostBoard[y][x] == 01 || GhostBoard[y][x] == 97)
		return true;
	return false;
}

// Inky

void Game::G2::Draw() {
	switch (face) {
	case RIGHT:
		DrawSprite(154 + phase, X, Y);
		break;
	case LEFT:
		DrawSprite(160 + phase, X, Y);
		break;
	case DOWN:
		DrawSprite(156 + phase, X, Y);
		break;
	case UP:
		DrawSprite(158 + phase, X, Y);
		break;
	}
}

void Game::G2::TogglePhase() {
	phase = !phase;
}

void Game::G2::Teleport() {
	if (Y == 272) {
		if (X == 432) {
			DrawSprite(1, X, Y);
			X = 0;
		}
		else if (X == 0) {
			DrawSprite(1, X, Y);
			X = 432;
		}
	}
}

void Game::G2::Advance() {
	double mSpeed = speed;
	if (Y == 272 && ((X <= 96 && X >= 0) || (X >= 336 && X <= 432)))
		mSpeed = speed / 2;
	for (double i = 0; i < mSpeed; i += .2) {
		if (X % 16 == 0 && Y % 16 == 0 && i != 0)
			break;
		switch (face) {
		case RIGHT:
			X++;
			break;
		case LEFT:
			X--;
			break;
		case DOWN:
			Y++;
			break;
		case UP:
			Y--;
			break;
		}
		Draw();
	}
}

void Game::G2::Logic() {
	AI();
	Draw();
	if (GetTimeSince(_PhaseTS))
		TogglePhase();
	if ((X % 16 == 0 && Y % 16 == 0))
		FindPath();
	Advance();
	Teleport();
}

void Game::G2::AI() {
	switch (state) {
	case CHASE:
	case PANIC:
		switch (PlayerInput) {
		case UP:
			tY = pY - 64;
			tX = pX - 64;
			break;
		case DOWN:
			tY = pY + 64;
			tX = pX;
			break;
		case LEFT:
			tY = pY;
			tX = pX - 64;
			break;
		case RIGHT:
			tY = pY;
			tX = pX + 64;
			break;
		}
		break;
	case SCATTER:
		tX = 32;
		tY = -16;
		break;
	}
}

void Game::G2::FindPath() {
	bool D = true, U = true, L = true, R = true;
	double D_Down = -1, D_Up = -1, D_Left = -1, D_Right = -1;
	double top_value = 0;

	if (face == DOWN || !CheckTile(X / 16, Y / 16 - 1) || ((Y == 416 || Y == 224) && X >= 176 && X <= 272)) // Up Check
		U = false;
	else
		D_Up = GetDistanceOf(X, Y - 16, tX, tY);
	if (face == UP || !CheckTile(X / 16, Y / 16 + 1) || (Y == 224 && X >= 176 && X <= 272)) // Down Check
		D = false;
	else
		D_Down = GetDistanceOf(X, Y + 16, tX, tY);
	if (face == RIGHT || !CheckTile(X / 16 - 1, Y / 16)) // Left Check
		L = false;
	else
		D_Left = GetDistanceOf(X - 16, Y, tX, tY);
	if (face == LEFT || !CheckTile(X / 16 + 1, Y / 16)) // Right Check
		R = false;
	else
		D_Right = GetDistanceOf(X + 16, Y, tX, tY);

	if (U && D_Up != -1)
		top_value = D_Up;
	if ((D_Right <= top_value || top_value == 0) && R && D_Right != -1)
		top_value = D_Right;
	if ((D_Down <= top_value || top_value == 0) && D && D_Down != -1)
		top_value = D_Down;
	if ((D_Left <= top_value || top_value == 0) && L && D_Left != -1)
		top_value = D_Left;

	if (top_value == D_Up)
		face = UP;
	else if (top_value == D_Right)
		face = RIGHT;
	else if (top_value == D_Down)
		face = DOWN;
	else if (top_value == D_Left)
		face = LEFT;

}

bool Game::G2::CheckTile(int x, int y) {
	if (GhostBoard[y][x] == 78 || GhostBoard[y][x] == 79 || GhostBoard[y][x] == 00 || GhostBoard[y][x] == 01 || GhostBoard[y][x] == 97 || GhostBoard[y][x] == 77)
		return true;
	return false;
}

/////////////////////////////////////////////////////
// Prototype Implementation
/////////////////////////////////////////////////////

double GetDistanceOf(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void UpdateTS(double& timestamp) {
	timestamp = GetTime();
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

double Wait(double waitTime) {
	double startTime = GetTime();

	while (waitTime > GetTimeSince(startTime)) {}

	return GetTimeSince(startTime + waitTime);
}

void CopyBoardData(int Arr1[MH][MW], int Arr2[MW][MW]) {
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			Arr2[y][x] = Arr1[y][x];
		}
	}
}

void InitializeBoardData(int Arr[MH][MW]) {
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++)
			Arr[y][x] = 0;
	}
}

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void DrawSprite(int spriteID, double X, double Y) {
	SelectObject(hdc, bmap);
	if (spriteID > 0) {
		if (spriteID == 78)
			BitBlt(console, X + 6, Y + 6, 4, 4, hdc, SpriteIDs[spriteID - 1][0] + 6, SpriteIDs[spriteID - 1][1] + 6, SRCCOPY);
		else if (spriteID == 77)
			BitBlt(console, X, Y + 11, 16, 4, hdc, SpriteIDs[spriteID - 1][0], SpriteIDs[spriteID - 1][1] + 11, SRCCOPY);
		else
			BitBlt(console, X, Y, 16, 16, hdc, SpriteIDs[spriteID - 1][0], SpriteIDs[spriteID - 1][1], SRCCOPY);
		DeleteObject(bmap);
	}
}

void SetWindowDimensions(int x, int y) { // Thanks for this, Sam!
	HWND console = GetConsoleWindow();
	HMONITOR monitor = MonitorFromWindow(console, MONITOR_DEFAULTTOPRIMARY);

	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(monitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;

	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double vertScale = ((double)cyPhysical / (double)cyLogical);
	SetWindowPos(console, HWND_TOP, 0, 0, x * 16.0 / horzScale, y * 16.0 / vertScale, SWP_NOMOVE);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size = {
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
	ShowConsoleCursor(false);
}
