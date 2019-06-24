#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <conio.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MapWidth = 28;
const int MapHeight = 36;
int SpriteSize = 16;

bool EXIT_PROGRAM = false;
bool EXIT_GAME = false;

int PacmanX = 0;
int PacmanY = 0;
int PacmanPhase = 1;
double FrameTimeStamp = 0.0;

enum input {UP, DOWN, LEFT, RIGHT, ENTER, NONE};
input PlayerInput = NONE;
bool PlayerHasMadeFirstInput = false;
bool AllowDefaultClear = false;

double FPS = 1.0 / 30.0;
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
	06,04,04,04,04,15,32, 9,14,12,12,15,00, 9,11,00,13,12,12,16,11,32,13,04,04,04,04, 8,
	00,00,00,00,00,01,32, 9,13,10,10,16,00,14,16,00,14,10,10,15,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,00,00,00,00,00,00,00,00,00,00, 9,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,00,25,04,29,31,31,30,04,28,00, 9,11,32,03,00,00,00,00,00,
	02,02,02,02,02,16,32,14,16,00,03,00,00,00,00,00,00,01,00,14,16,32,14,02,02,02,02,02,
	00,00,00,00,00,00,32,00,00,00,03,00,00,00,00,00,00,01,00,00,00,32,00,00,00,00,00,00,
	04,04,04,04,04,15,32,13,15,00,03,00,00,00,00,00,00,01,00,13,15,32,13,04,04,04,04,04,
	00,00,00,00,00,01,32, 9,11,00,26,02,02,02,02,02,02,27,00, 9,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,00,00,00,00,00,00,00,00,00,00, 9,11,32,03,00,00,00,00,00,
	00,00,00,00,00,01,32, 9,11,00,13,12,12,12,12,12,12,15,00, 9,11,32,03,00,00,00,00,00,
	05,02,02,02,02,16,32,14,16,00,14,10,10,15,13,10,10,16,00,14,16,32,14,02,02,02,02,07,
	01,32,32,32,32,32,32,32,32,32,32,32,32, 9,11,32,32,32,32,32,32,32,32,32,32,32,32,03,
	01,32,13,12,12,15,32,13,12,12,12,15,32, 9,11,32,13,12,12,12,15,32,13,12,12,15,32,03,
	01,32,14,10,15,11,32,14,10,10,10,16,32,14,16,32,14,10,10,10,16,32, 9,13,10,16,32,03,
	01,33,32,32, 9,11,32,32,32,32,32,32,32,00,00,32,32,32,32,32,32,32, 9,11,32,32,33,03,
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
void ClearPacman(int x, int y);
void MovePacman();
bool CheckCollision(input dir);

bool KeyIsDown(char, bool, bool);
double GetTime();
double GetTimeSince(double);
double wait(double);
void ShowConsoleCursor(bool);

const int SpritePositions[40][2] = {
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
	22, 202, // Pacman P2 38
	42, 202, // Pacman P3 39

	02,02 // Clear 40
};

int main() {
	GameSetup();
	while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
		GameDraw();
		GameInput();
		GameLogic();
	}
	system("PAUSE");
	return 0;
}

// Game Frame //
void GameSetup() {
	system("MODE 56, 37");
	PacmanX = 13;
	PacmanY = 26;
	PlayerInput = NONE;
	FrameTimeStamp = GetTime();
	PlayerHasMadeFirstInput = false;
	AllowDefaultClear = false;
}
void GameDraw() {
	Map[PacmanY][PacmanX] = GetSpriteThroughPhase();
	DrawMap();
}
void GameInput() {
	if (_kbhit())
		PlayerHasMadeFirstInput = true;
	if (KeyIsDown('W', true, false) &&  !CheckCollision(UP))
		PlayerInput = UP;
	if (KeyIsDown('S', true, false) && !CheckCollision(DOWN))
		PlayerInput = DOWN;
	if (KeyIsDown('D', true, false) && !CheckCollision(RIGHT))
		PlayerInput = RIGHT;
	if (KeyIsDown('A', true, false) && !CheckCollision(LEFT))
		PlayerInput = LEFT;
}
void GameLogic() {
	if (GetTimeSince(FrameTimeStamp) > .5)
		IncrementPacmanPhase();
	MovePacman();
}
////////////////
void MovePacman() {
	int OldX = PacmanX;
	int OldY = PacmanY;
	if (!CheckCollision(PlayerInput)) {
		switch (PlayerInput) {
		case LEFT:
			PacmanX--;
			break;
		case RIGHT:
			PacmanX++;
			break;
		case DOWN:
			PacmanY++;
			break;
		case UP:
			PacmanY--;
			break;
		}
	}
	if (OldX != PacmanX || OldY != PacmanY) {
		ClearPacman(OldX, OldY);
		Map[OldY][OldX] = 00;
	}
}
bool CheckCollision(input dir) {
	switch (dir) {
	case LEFT:
		if (Map[PacmanY][PacmanX - 1] == 32 || Map[PacmanY][PacmanX - 1] == 33 || Map[PacmanY][PacmanX - 1] == 00)
			return false;
		break;
	case RIGHT:
		if (Map[PacmanY][PacmanX + 1] == 32 || Map[PacmanY][PacmanX + 1] == 33 || Map[PacmanY][PacmanX + 1] == 00)
			return false;
		break;
	case DOWN:
		if (Map[PacmanY + 1][PacmanX] == 32 || Map[PacmanY + 1][PacmanX] == 33 || Map[PacmanY + 1][PacmanX] == 00)
			return false;
		break;
	case UP:
		if (Map[PacmanY - 1][PacmanX] == 32 || Map[PacmanY - 1][PacmanX] == 33 || Map[PacmanY - 1][PacmanX] == 00)
			return false;
		break;
	}
	return true;
}
void ClearPacman(int x, int y) {
	DrawSprite(40, (x * SpriteSize), (y * SpriteSize), SpriteSize);
}
void IncrementPacmanPhase() {
	PacmanPhase++;
	if (PacmanPhase > 3)
		PacmanPhase = 1;
}
int GetSpriteThroughPhase() {
	if (PacmanPhase == 1)
		return 37;
	else if (PacmanPhase == 2)
		return 38;
	else if (PacmanPhase == 3)
		return 39;
}
void DrawMap() {
	for (int y = 0; y < MapHeight; y++) {
		for (int x = 0; x < MapWidth; x++) {
			if ((Map[y][x] == 37 || Map[y][x] == 38 || Map[y][x] == 39) && !PlayerHasMadeFirstInput)
				DrawSprite(Map[y][x], (x * SpriteSize) + 8, (y * SpriteSize), SpriteSize);
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
