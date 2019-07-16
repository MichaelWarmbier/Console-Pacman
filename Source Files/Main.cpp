#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <conio.h>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

const int MapWidth = 28;
const int MapHeight = 36;
int SpriteSize = 16;

bool EXIT_PROGRAM = false;
bool EXIT_GAME = false;

double PacmanX = 0;
double PacmanY = 0;
int PacmanPhase = 1;
int CollectedDots = 0;
double PhaseTimeStamp = 0.0;
double MovementTimeStamp = 0.0;
double MoveSpeed = .5;
int PortalHeight = 17;

enum input { UP, DOWN, LEFT, RIGHT, ENTER, NONE };
enum state { BEFORE, DURING, AFTER, LIMBO };
state GameState = DURING;
input PlayerInput = NONE;
input StoredInput = NONE;
bool PlayerHasMadeFirstInput = false;
bool PausePacmanAnimation = false;

double FPS = 1.0 / 60.0;
double timer = 0, dt = 0;

HDC console = GetDC(GetConsoleWindow());
HDC hdc = CreateCompatibleDC(NULL);
HBITMAP bmap = (HBITMAP)LoadImage(NULL, _T("ConsolePacmanSpriteSheet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

int Map[MapHeight][MapWidth]{
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	05,02,02,02,02,02,02,02,02,02,02,02,02,20,34,02,02,02,02,02,02,02,02,02,02,02,02,07,
	01,32,32,32,32,32,32,32,32,32,32,32,32, 9,11,32,32,32,32,32,32,32,32,32,32,32,32,03,
	01,32,13,12,12,15,32,13,12,12,12,15,32, 9,11,32,13,12,12,12,15,32,13,12,12,15,32,03,
	01,33, 9,00,00,11,32, 9,00,00,00,11,32, 9,11,32, 9,00,00,00,11,32, 9,00,00,11,33,03,
	01,32,14,10,10,16,32,14,10,10,10,16,32,14,16,32,14,10,10,10,16,32,14,10,10,16,32,03,
	01,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,03,
	01,32,13,12,12,15,32,13,15,32,13,12,12,12,12,12,12,15,32,13,15,32,13,12,12,15,32,03,
	01,32,14,10,10,16,32, 9,11,32,14,10,10,15,13,10,10,16,32, 9,11,32,14,10,10,16,32,03,
	01,32,32,32,32,32,32, 9,11,32,32,32,32, 9,11,32,32,32,32, 9,11,32,32,32,32,32,32,03,
	06,04,04,04,04,15,32, 9,14,12,12,15,40, 9,11,40,13,12,12,16,11,32,13,04,04,04,04, 8,
	00,00,00,00,00,01,32, 9,13,10,10,16,40,14,16,40,14,10,10,15,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,40,40,40,40,40,40,40,40,40,40, 9,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,40,25,04,29,31,31,30,04,28,40, 9,11,32,03,00,00,00,00,00,
	02,02,02,02,02,16,32,14,16,40,03,00,00,00,00,00,00,01,40,14,16,32,14,02,02,02,02,02,
	40,40,40,40,40,40,32,40,40,40,03,00,00,00,00,00,00,01,40,40,40,32,40,40,40,40,40,40,
	04,04,04,04,04,15,32,13,15,40,03,00,00,00,00,00,00,01,40,13,15,32,13,04,04,04,04,04,
	00,00,00,00,00,01,32, 9,11,40,26,02,02,02,02,02,02,27,40, 9,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,40,40,40,40,40,40,40,40,40,40, 9,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,40,13,12,12,12,12,12,12,15,40, 9,11,32,03,00,00,00,00,00,
	05,02,02,02,02,16,32,14,16,40,14,10,10,15,13,10,10,16,40,14,16,32,14,02,02,02,02,07,
	01,32,32,32,32,32,32,32,32,32,32,32,32, 9,11,32,32,32,32,32,32,32,32,32,32,32,32,03,
	01,32,13,12,12,15,32,13,12,12,12,15,32, 9,11,32,13,12,12,12,15,32,13,12,12,15,32,03,
	01,32,14,10,15,11,32,14,10,10,10,16,32,14,16,32,14,10,10,10,16,32, 9,13,10,16,32,03,
	01,33,32,32, 9,11,32,32,32,32,32,32,32,40,40,32,32,32,32,32,32,32, 9,11,32,32,33,03,
	35,12,15,32, 9,11,32,13,15,32,13,12,12,12,12,12,12,15,32,13,15,32, 9,11,32,13,12,19,
	17,10,16,32,14,16,32, 9,11,32,14,10,10,15,13,10,10,16,32, 9,11,32,14,16,32,14,10,36,
	01,32,32,32,32,32,32, 9,11,32,32,32,32, 9,11,32,32,32,32, 9,11,32,32,32,32,32,32,03,
	01,32,13,12,12,12,12,16,14,12,12,15,32, 9,11,32,13,12,12,16,14,12,12,12,12,15,32,03,
	01,32,14,10,10,10,10,10,10,10,10,16,32,14,16,32,14,10,10,10,10,10,10,10,10,16,32,03,
	01,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,03,
	06,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 8,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
};

void GameSetup();
void GameDraw();
void GameInput();
void GameLogic();

int GetSpriteThroughPhase();
void DrawMap();
void DrawSprite(int, int, int, int);
void IncrementPacmanPhase();
int NumberToSpriteIndex(int);
void ClearPacman(int, int);
void MovePacman();
bool CheckCollision(input);
void CollectedDotCounter();
void DrawNumber(int, int, int);

bool KeyIsDown(char, bool, bool);
double GetTime();
double GetTimeSince(double);
double wait(double);
void ShowConsoleCursor(bool);

const int SpritePositions[56][2] = {
	02,22, // Thick Wall Vertical Left 1
	22,22, // Thick Wall Horizontal Top 2
	42,22, // Thick Wall Vertical Right 3
	62,22, // Thick Wall Horizontal Bottom 4

	02,42, // Thick Corner Top Left 5
	22,42, // Thick Corner Bottom Left 6 
	42,42, // Thick Corner Top Right 7
	62,42, // Thick Corner Bottom Right 8

	02,62, // Thin Wall Vertical Left 9
	22,62, // Thin Wall Horizontal Top 10
	42,62, // Thin Wall Vertical Right 11
	62,62, // Thin Wall Horizontal Bottom 12

	02,82, // Thin Corner Top Left 13
	22,82, // Thin Corner Bottom Left 14
	42,82, // Thin Corner Top Right 15
	62,82, // Thin Corner Bottom Right 16

	02,102, // Wall Cross Top Left 17
	22,102, // Wall Cross Bottom Left 18
	42,102, // Wall Cross Top Right 19
	62,102, // Wall Cross Bottom Right 20

	02,122, // Open Wall Cross Top Left 21
	22,122, // Open Wall Cross Bottom Left 22
	42,122, // Open Wall Cross Top Right 23 
	62,122, // Open Wall Cross Bottom Right 24

	02, 142, // Square Corner Top Left 25
	22, 142, // Square Corner Bottom Left 26
	42, 142, // Square Corner Top Right 27
	62, 142, // Square Corner Bottom Right 28

	02, 162, // Square Wall End Inwwards-Right 29
	22, 162, // Square Wall End Inwards-Left 30
	42, 162, // Gate 31

	02, 182, // Pac-dot 32
	22, 182, // Power-Pellet 33

	82, 102, // Special Wall Cross R1 34
	102, 102, // Special Wall Cross R2 35
	122, 102, // Special Wall Cross R3 36

	02, 202, // Pacman P1 37

	22, 202, // Pacman P2 RIGHT 38
	42, 202, // Pacman P3 RIGHT 39

	02,02, // Clear 40

	62, 202, // Pacman P2 DOWN 41
	82, 202, // Pacman P3 DOWN 42

	102, 202, // Pacman P2 LEFT 43
	122, 202, // Pacman P3 LEFT 44

	142, 202, // Pacman P2 UP 45
	162, 202, // Pacman P3 UP 46

	22, 02, // #0 47
	42, 02, // #1 48
	62, 02, // #2 49
	82, 02, // #3 50
	102, 02, // #4 51
	122, 02, // #5 52
	142, 02, // #6 53
	162, 02, // #7 54
	182, 02, // #8 55
	202, 02, // #9 56
};

int main() {
	ShowConsoleCursor(false);
	GameSetup();
	while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
		while (GetConsoleWindow() != GetForegroundWindow()) {}
		GameDraw();
		GameInput();
		GameLogic();
	}
	return 0;
}

// Game Frame //
void GameSetup() {
	system("MODE 56, 37");
	PacmanX = 13;
	PacmanY = 26;
	PlayerInput = NONE;
	StoredInput = NONE;
	PhaseTimeStamp = GetTime();
	MovementTimeStamp = GetTime();
	PlayerHasMadeFirstInput = false;
	PausePacmanAnimation = false;
	MoveSpeed = .5;
}

void GameDraw() {
	Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX)] = GetSpriteThroughPhase();
	DrawMap();
	DrawNumber(CollectedDots, 5, 1);
}

void GameInput() {
	if (_kbhit()) {
		if (KeyIsDown('W', true, true) && !KeyIsDown('S', true, true) && !CheckCollision(UP)) {
			PlayerHasMadeFirstInput = true;
			StoredInput = UP;
		}
		if (KeyIsDown('S', true, true) && !KeyIsDown('W', true, true) && !CheckCollision(DOWN)) {
			PlayerHasMadeFirstInput = true;
			StoredInput = DOWN;
		}
		if (KeyIsDown('D', true, true) && !KeyIsDown('A', true, true) && !CheckCollision(RIGHT)) {
			PlayerHasMadeFirstInput = true;
			StoredInput = RIGHT;
		}
		if (KeyIsDown('A', true, true) && !KeyIsDown('D', true, true) && !CheckCollision(LEFT)) {
			PlayerHasMadeFirstInput = true;
			StoredInput = LEFT;
		}
	}
	if (!CheckCollision(StoredInput)) {
		PlayerInput = StoredInput;
		StoredInput = NONE;
	}
	if (KeyIsDown('W', true, true) && !KeyIsDown('S', true, true) && !CheckCollision(UP))
		PlayerInput = UP;
	if (KeyIsDown('S', true, true) && !KeyIsDown('W', true, true) && !CheckCollision(DOWN))
		PlayerInput = DOWN;
	if (KeyIsDown('D', true, true) && !KeyIsDown('A', true, true) && !CheckCollision(RIGHT))
		PlayerInput = RIGHT;
	if (KeyIsDown('A', true, true) && !KeyIsDown('D', true, true) && !CheckCollision(LEFT))
		PlayerInput = LEFT;
}

void GameLogic() {
	if (GetTimeSince(PhaseTimeStamp) > .01 && !PausePacmanAnimation) {
		IncrementPacmanPhase();
		PhaseTimeStamp = GetTime();
	}
	if (GetTimeSince(MovementTimeStamp) >= MoveSpeed * dt) {
		MovePacman();
		MovementTimeStamp = GetTime();
	}
	if (PacmanX == MapWidth - 2 && PacmanY == PortalHeight && PlayerInput == RIGHT)
		PacmanX = MapWidth - MapWidth;
	else if (PacmanX == MapWidth - MapWidth && PacmanY == PortalHeight && PlayerInput == LEFT)
		PacmanX = MapWidth - 2;
}

////////////////
void MovePacman() {
	double OldX = PacmanX;
	double OldY = PacmanY;
	if (!CheckCollision(PlayerInput)) {
		CollectedDotCounter();
		switch (PlayerInput) {
		case LEFT:
			PacmanX -= 25 * dt;
			break;
		case RIGHT:
			PacmanX += 25 * dt;
			break;
		case DOWN:
			PacmanY += 25 * dt;
			break;
		case UP:
			PacmanY -= 25 * dt;
			break;
		}
	}
	if (OldX != PacmanX || OldY != PacmanY)
		Map[static_cast<int>(OldY)][static_cast<int>(OldX)] = 40;
}

bool CheckCollision(input dir) {
	switch (dir) {
	case LEFT:
		if (Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) - 1] == 40 || Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) - 1] == 33 || Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) - 1] == 32)
			return false;
		break;
	case RIGHT:
		if (Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) + 1] == 40 || Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) + 1] == 33 || Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) + 1] == 32)
			return false;
		break;
	case DOWN:
		if (Map[static_cast<int>(PacmanY) + 1][static_cast<int>(PacmanX)] == 40 || Map[static_cast<int>(PacmanY) + 1][static_cast<int>(PacmanX)] == 33 || Map[static_cast<int>(PacmanY) + 1][static_cast<int>(PacmanX)] == 32)
			return false;
		break;
	case UP:
		if (Map[static_cast<int>(PacmanY) - 1][static_cast<int>(PacmanX)] == 40 || Map[static_cast<int>(PacmanY) - 1][static_cast<int>(PacmanX)] == 33 || Map[static_cast<int>(PacmanY) - 1][static_cast<int>(PacmanX)] == 32)
			return false;
		break;
	}
	return true;
}

void CollectedDotCounter() {
	switch (PlayerInput) {
	case UP:
		if (Map[static_cast<int>(PacmanY) - 1][static_cast<int>(PacmanX)] == 32 || Map[static_cast<int>(PacmanY) - 1][static_cast<int>(PacmanX)] == 33)
			CollectedDots++;
		break;
	case DOWN:
		if (Map[static_cast<int>(PacmanY) + 1][static_cast<int>(PacmanX)] == 32 || Map[static_cast<int>(PacmanY) + 1][static_cast<int>(PacmanX)] == 33)
			CollectedDots++;
		break;
	case LEFT:
		if (Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) - 1] == 32 || Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) - 1] == 33)
			CollectedDots++;
		break;
	case RIGHT:
		if (Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) + 1] == 32 || Map[static_cast<int>(PacmanY)][static_cast<int>(PacmanX) + 1] == 33)
			CollectedDots++;
		break;
	}
}

void DrawNumber(int value, int x_pos, int y_pos) {
	int ZeroOffset = 0;
	bool FirstNumber = false;
	if (value > 9999999)
		value = 9999999;
	int Digits[7] = { 0,0,0,0,0,0,0 };
	for (int i = 6; i >= 0; i--) {
		Digits[i] = value % 10;
		value /= 10;
	}
	for (int i = 0; i < 7; i++) {
		if (Digits[i] > 0 && !FirstNumber)
			FirstNumber = true;
		if (Digits[i] == 0 && !FirstNumber && i != 6) {
			ZeroOffset++;
			continue;
		}
		Map[y_pos][x_pos + i - ZeroOffset] = NumberToSpriteIndex(Digits[i]);
	}
}

int NumberToSpriteIndex(int number) {
	if (number > 9 || number < 0)
		return 40;
	else {
		switch (number) {
		case 0:
			return 47;
		case 1:
			return 48;
		case 2:
			return 49;
		case 3:
			return 50;
		case 4:
			return 51;
		case 5:
			return 52;
		case 6:
			return 53;
		case 7:
			return 54;
		case 8:
			return 55;
		case 9:
			return 56;
		}
	}
}

void IncrementPacmanPhase() {
	PacmanPhase++;
	if (PacmanPhase > 3)
		PacmanPhase = 1;
}

int GetSpriteThroughPhase() {
	if (PacmanPhase == 1)
		return 37;
	else if (PacmanPhase == 2) {
		if (PlayerInput == RIGHT || PlayerInput == NONE)
			return 38;
		else if (PlayerInput == DOWN)
			return 41;
		else if (PlayerInput == LEFT)
			return 43;
		else if (PlayerInput == UP)
			return 45;
	}
	else if (PacmanPhase == 3) {
		if (PlayerInput == RIGHT || PlayerInput == NONE)
			return 39;
		if (PlayerInput == DOWN)
			return 42;
		else if (PlayerInput == LEFT)
			return 44;
		else if (PlayerInput == UP)
			return 46;
	}
}

void DrawMap() {
	for (int y = 0; y < MapHeight; y++) {
		for (int x = 0; x < MapWidth; x++) {
			if ((Map[y][x] == 37 || Map[y][x] == 38 || Map[y][x] == 39) && !PlayerHasMadeFirstInput)
				DrawSprite(Map[y][x], (x * SpriteSize) + 8, (y * SpriteSize), SpriteSize);
			else if (Map[y][x] == 40 && !PlayerHasMadeFirstInput)
				continue;
			else if (Map[y][x] > 0)
				DrawSprite(Map[y][x], (x * SpriteSize), (y * SpriteSize), SpriteSize);
		}
	}
}

void DrawSprite(int ArrPos, int x, int y, int size) {
	SelectObject(hdc, bmap);
	BitBlt(console, x, y, size, size, hdc, SpritePositions[ArrPos - 1][0], SpritePositions[ArrPos - 1][1], SRCCOPY);
	DeleteObject(bmap);
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
