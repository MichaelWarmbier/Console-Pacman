#include "Header.h"

int main() {
	SetWindowDimensions(29, 39);
	int temp_level = 1;
	do {
		Game* Pacman = new Game;
		//Game * Pacman = new Game(temp_level);
		while (!EXIT_GAME_F) {
			Pacman->Draw();
			Pacman->Input();
			Pacman->Logic();
		}
		if (!EXIT_PROGRAM_F) {
			temp_level++;
			EXIT_GAME_F = false;
		}
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
	DisplayInteger(10, MH - 2, collected_dots);
	P1.Draw();
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
	BlinkyLogic();
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
	}
	if (StateOfGame == DURING) {
		Input();
		for (double i = 0; i < P1.speed; i += .2) {
			if (OldInput != PlayerInput && PlayerInput != NONE)
				OldInput = PlayerInput;
			if (!CheckCol(P1.X - 1, P1.Y, 1) && !CheckCol(P1.X - 1, P1.Y + 15, 1) && (PlayerInput == LEFT || OldInput == LEFT)) {
				P1.X--;
				PlayerInput = LEFT;
			}
			else if (!CheckCol(P1.X + 16, P1.Y, 1) && !CheckCol(P1.X + 16, P1.Y + 15, 1) && (PlayerInput == RIGHT || OldInput == RIGHT)) {
				P1.X++;
				PlayerInput = RIGHT;
			}
			else if (!CheckCol(P1.X, P1.Y - 1, 1) && !CheckCol(P1.X + 15, P1.Y - 1, 1) && (PlayerInput == UP || OldInput == UP)) {
				P1.Y--;
				PlayerInput = UP;
			}
			else if (!CheckCol(P1.X, P1.Y + 16, 1) && !CheckCol(P1.X + 15, P1.Y + 16, 1) && (PlayerInput == DOWN || OldInput == DOWN)) {
				P1.Y++;
				PlayerInput = DOWN;
			}
			TeleportPacman();
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
			P1.TogglePhase();
			UpdateTS(P1._PhaseTS);
		}
		DotGrab();
		CheckDotData();
	}
}

Game::Game() {
	InitializeColBoard();
	UpdateTS(_1UP_TS);
	UpdateTS(_PP_TS);
	PlayerLives = 3;
	PlayerLevel = 1;
	StateOfGame = BEFORE;
	collected_dots = 244;
	score = 0;
}
Game::Game(int level) {
	InitializeColBoard();
	UpdateTS(_1UP_TS);
	UpdateTS(_PP_TS);
	PlayerLives = 3;
	PlayerLevel = level;
	StateOfGame = BEFORE;
	collected_dots = 244;
	score = 0;
}
void Game::DrawLives() {
	for (int i = 0; i < 5; i++) {
		if (i < PlayerLives - 1)
			ChangeTile(i + 3, MH - 2, 93);
		else
			ChangeTile(i + 3, MH - 2, 1);
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
			if (GetTileID(x, y) == 79 || GetTileID(x, y) == 78 || GetTileID(x, y) == 97)
				collected_dots++;
		}
	}
}

void Game::DotGrab() {
	switch (PlayerInput) {
	case LEFT:
		if (CheckCol(P1.X + 6, P1.Y + 4, 2)) {
			ChangeTile(static_cast<int>(P1.X + 6) / 16, (static_cast<int>(P1.Y) + 4) / 16, 0);
		}
		break;
	case RIGHT:
		if (CheckCol(P1.X + 6, P1.Y + 4, 2)) {
			ChangeTile(static_cast<int>(P1.X + 6) / 16, (static_cast<int>(P1.Y) + 4) / 16, 0);
		}
		break;
	case UP:
		if (CheckCol(P1.X + 4, P1.Y + 6, 2)) {
			ChangeTile(static_cast<int>(P1.X + 4) / 16, (static_cast<int>(P1.Y) + 6) / 16, 0);
		}
		break;
	case DOWN:
		if (CheckCol(P1.X + 4, P1.Y + 6, 2)) {
			ChangeTile(static_cast<int>(P1.X + 4) / 16, (static_cast<int>(P1.Y) + 6) / 16, 0);
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

void Game::TeleportPacman() {
	if (P1.Y == 272) {
		if (P1.X == 432) {
			DrawSprite(1, P1.X, P1.Y);
			P1.X = 0;
		}
		else if (P1.X == 0) {
			DrawSprite(1, P1.X, P1.Y);
			P1.X = 432;
		}
	}
}

void Game::Input() {
	// Alt controls TBA
	if (KeyIsDown('W', true, true) && !CheckCol(P1.X, P1.Y - 1, 1) && !CheckCol(P1.X + 15, P1.Y - 1, 1))
		PlayerInput = UP;
	if (KeyIsDown('S', true, true) && !CheckCol(P1.X, P1.Y + 16, 1) && !CheckCol(P1.X + 15, P1.Y + 16, 1))
		PlayerInput = DOWN;
	if (KeyIsDown('A', true, true) && !CheckCol(P1.X - 1, P1.Y, 1) && !CheckCol(P1.X - 1, P1.Y + 15, 1))
		PlayerInput = LEFT;
	if (KeyIsDown('D', true, true) && !CheckCol(P1.X + 16, P1.Y, 1) && !CheckCol(P1.X + 16, P1.Y + 15, 1))
		PlayerInput = RIGHT;

}

/////////////////////////////////////////////////////
// Player Class Implementation
/////////////////////////////////////////////////////

Game::Player::Player() {
	SpritePhase = 2;
	_PhaseTS = GetTime();
	X = 13 * 16.0 + 8;
	Y = 26 * 16.0;
	PlayerInput = NONE;
	speed = .4;
}

void Game::Player::TogglePhase() {
	SpritePhase++;
	if (SpritePhase > 3)
		SpritePhase = 1;
}

void Game::Player::Draw() const {
	switch (SpritePhase) {
	case 1:
		DrawSprite(88, X, Y);
		break;
	case 2:
		switch (PlayerInput) {
		case LEFT:
			DrawSprite(93, X, Y);
			break;
		case RIGHT:
			DrawSprite(89, X, Y);
			break;
		case UP:
			DrawSprite(95, X, Y);
			break;
		case DOWN:
			DrawSprite(91, X, Y);
			break;
		}
		break;
	case 3:
		switch (PlayerInput) {
		case LEFT:
			DrawSprite(94, X, Y);
			break;
		case RIGHT:
			DrawSprite(90, X, Y);
			break;
		case UP:
			DrawSprite(96, X, Y);
			break;
		case DOWN:
			DrawSprite(92, X, Y);
			break;
		}
		break;
	}
}

void Game::BlinkyLogic() {
	BLINKY.Draw();
	if (GetTimeSince(BLINKY._PhaseTS))
		BLINKY.TogglePhase();
	BLINKY.Advance();
	if (BLINKY.X % 16 == 0 && BLINKY.Y % 16 == 0)
		BLINKY.GetNewTarget();
}

/////////////////////////////////////////////////////
// Ghost Class Implementation
/////////////////////////////////////////////////////

// BLINKY //

Game::Blinky::Blinky() {
	X = 216.0; Y = 224.0;
	tX = 0; tY = 0;
	state = CHASE; face = RIGHT;
	_PhaseTS = GetTime();
	phase = 0;
}

void Game::Blinky::Draw() {
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

void Game::Blinky::TogglePhase() {
	phase = !phase;
}

void Game::Blinky::Advance() {
	switch (face) {
	case RIGHT:
		X++;
		break;
	case LEFT:
		X--;
		break;
	case DOWN:
		break;
		Y++;
	case UP:
		Y--;
		break;
	}
}

void Game::Blinky::GetNewTarget() {
	//tba
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

bool CheckCol(int x, int y, int ID) {
	if (CollisionBoard[y][x] == ID)
		return true;
	return false;
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

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void DrawSprite(int spriteID, double X, double Y) {
	if (spriteID > 0) {
		SelectObject(hdc, bmap);
		BitBlt(console, (int)X, (int)Y, 16, 16, hdc, SpriteIDs[spriteID - 1][0], SpriteIDs[spriteID - 1][1], SRCCOPY);
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
