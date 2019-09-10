#include <conio.h>
#include <chrono>
#include <string>
#include <cstring>

using namespace std;
using namespace chrono;

const int MapWidth = 28;
const int MapHeight = 36;
int SpriteSize = 16;
HDC console = GetDC(GetConsoleWindow());
HDC hdc = CreateCompatibleDC(NULL);
HBITMAP bmap = (HBITMAP)LoadImage(NULL, _T("ConsolePacmanSpriteSheet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

bool EXIT_PROGRAM = false;
bool EXIT_GAME = false;
const int GSS = 16; // Global Sprite Size
bool EXIT_PROGRAM = false, EXIT_MENU = false, EXIT_GAME = false;
enum screen { mainmenu, howtoplay, options, debug };
enum input { UP, DOWN, LEFT, RIGHT, START, ESCAPE, NONE };
enum state { BEFORE, DURING, AFTER, LIMBO };
typedef int opt;

// Debug Options
bool ShowFPS = false;
bool ShowCounter = false;

double FPS = 1.0 / 60.0;
double timer = 0, dt = 0;

// Menu Variables
input MMInput = NONE;
screen CurrentScreen = mainmenu;
opt mainmenu_option = 1;
opt options_option = 1;
double PacmanFrameTS = 0;
int PacmanFrameMenu = 1;
double GhostFrameTS = 0;
double GhostIterationTS = 0;
int GhostFrameTick = 1;
int GhostIterationTick = 1;

// Game Variables
const int MapWidth = 28;
const int MapHeight = 36;
double PacmanX = 0;
double PacmanY = 0;
int PacmanPhase = 1;
@@ -23,84 +50,45 @@ double PhaseTimeStamp = 0.0;
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
void DrawSprite(int ArrPos, int x, int y, int size);
void SpriteTest(int x, int y);
void DrawLogo(int x, int y);
void DrawString(const char text[30], int x, int y);
int GetCharSpriteID(char character);
void ShowConsoleCursor(bool flag);
double Wait(double WaitTime);
double GetTimeSince(double StartTime);
double GetTime();
bool KeyIsDown(char key, bool pressed, bool held);
void DrawNumber(int value, int x, int y);
int NumberToSpriteIndex(int number);
void MenuIteratePacman();
void IncrementPacmanPhase();
int NumberToSpriteIndex(int);
bool CheckCollision(input);
void MovePacman();
void CollectedDotCounter();
int GetSpriteThroughPhase();
void DrawDesc(char ghost, int x, int y);
void DrawMap();

void MenuSetup();
void MenuDraw();
void MenuInput();
void MenuLogic();
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
const int SpritePositions[99][2] = {
	02,22, // Thick Wall Vertical Left 1
	22,22, // Thick Wall Horizontal Top 2
	42,22, // Thick Wall Vertical Right 3
@@ -173,21 +161,349 @@ const int SpritePositions[56][2] = {
	162, 02, // #7 54
	182, 02, // #8 55
	202, 02, // #9 56

	02, 222, // A 57
	22, 222, // B 58
	42, 222, // C 59
	62, 222, // D 60
	82, 222, // E 61
	102, 222, // F 62
	122, 222, // G 63
	142, 222, // H 64
	162, 222, // I 65

	02, 242, // J 66
	22, 242, // K 67
	42, 242, // L 68
	62, 242, // M 69
	82, 242, // N 70
	102, 242, // O 71
	122, 242, // P 72
	142, 242, // Q 73
	162, 242, // R 74

	02, 262, // S 75
	22, 262, // T 76
	42, 262, // U 77
	62, 262, // V 78
	82, 262, // W 79
	102, 262, // X 80
	122, 262, // Y 81
	142, 262, // Z 82

	02, 282, // - 83
	22, 282, // : 84
	42, 282, // ! 85
	62, 282, // ? 86
	82, 282, // " 87
	102, 282, // . 88

	02, 02, // ' ' 89

	42, 182, // Blinky R P1 90
	62, 182, // Blinky R P2 91
	82, 182, // Inky R P1 92
	102, 182, // Inky R P2 93
	122, 182, // Pinky R P1 94
	142, 182, // Pinky R P2 95
	162, 182, // Clyde R P1 96
	182, 182, // Clyde R P2 97

	62, 162, // False X 98
	82, 162, // True Check 99
};

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


int main() {
	do {
		MenuSetup();
		do {
			while (GetConsoleWindow() != GetForegroundWindow()) {}
			MenuDraw();
			MenuInput();
			MenuLogic();
		} while (!EXIT_MENU && (timer += (dt = FPS + Wait(FPS))));
		GameSetup();
		while (!EXIT_GAME && (timer += (dt = FPS + Wait(FPS)))) { // Add FPS
			while (GetConsoleWindow() != GetForegroundWindow()) {}
			GameDraw();
			GameInput();
			GameLogic();
		}
	} while (!EXIT_PROGRAM);
}

// -- Start of Menu Frame
void MenuSetup() {

	HWND console = GetConsoleWindow();
	HMONITOR monitor = MonitorFromWindow(console, MONITOR_DEFAULTTOPRIMARY);

	// Logical Width/Height
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(monitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// Physical Width/Height
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;

	// Scaling Factor
	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double vertScale = ((double)cyPhysical / (double)cyLogical);

	//system("MODE 30, 30"); // Easy hack to remove scrollbar

	SetWindowPos(console, HWND_TOP, 0, 0, double(36 * 16) / horzScale + 10, double(25 * 16) / vertScale, SWP_NOMOVE); // Resize without moving where the console window was placed

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size = {
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
	ShowConsoleCursor(false);
	GameSetup();
	while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
		while (GetConsoleWindow() != GetForegroundWindow()) {}
		GameDraw();
		GameInput();
		GameLogic();
	PacmanFrameTS = GetTime();
	GhostFrameTS = GetTime();
	GhostIterationTS = GetTime();
}
void MenuDraw() {
	DrawLogo(5, 2);
	switch (CurrentScreen) {
	case mainmenu:
		if (mainmenu_option == 1)
			DrawString("* ", 10, 10);
		else
			DrawString("  ", 10, 10);
		DrawString("Play Game", 12, 10);
		if (mainmenu_option == 2)
			DrawString("* ", 10, 11);
		else
			DrawString("  ", 10, 11);
		DrawString("Options", 12, 11);
		if (mainmenu_option == 3)
			DrawString("* ", 10, 12);
		else
			DrawString("  ", 10, 12);
		DrawString("How To Play", 12, 12);
		if (mainmenu_option == 4)
			DrawString("* ", 10, 13);
		else
			DrawString("  ", 10, 13);
		DrawString("Exit Game", 12, 13);
		break;
	case howtoplay:
		DrawString("Simply score as many points", 4, 10);
		DrawString("as you can by eating the small", 3, 11);
		DrawString("dots all around the maze. Avoid", 2, 12);
		DrawString("the ghosts at all costs! Eat all", 2, 13);
		DrawString("the dots to move onto", 7, 14);
		DrawString("the next level!", 9, 15);
		break;
	case options:
		if (options_option == 1)
			DrawString("* ", 8, 10);
		else
			DrawString("  ", 8, 10);
		DrawString("FPS Display:", 10, 10);
		if (ShowFPS)
			DrawString(">", 27, 10);
		else
			DrawString("<", 27, 10);
		if (options_option == 2)
			DrawString("* ", 8, 11);
		else
			DrawString("  ", 8, 11);
		DrawString("Pellet Counter:", 10, 11);
		if (ShowCounter)
			DrawString(">", 27, 11);
		else
			DrawString("<", 27, 11);
		if (options_option == 3)
			DrawString("* ", 8, 12);
		else
			DrawString("  ", 8, 12);
		DrawString("Delete Save Data", 10, 12);
		if (options_option == 4)
			DrawString("* ", 8, 13);
		else
			DrawString("  ", 8, 13);
		DrawString("Go Back", 10, 13);
		break;
	}
	DrawString("&", 8, 17);
	switch (GhostIterationTick) {
	case 1:
		DrawDesc('b', 10, 17);
		break;
	case 2:
		DrawDesc('p', 10, 17);
		break;
	case 3:
		DrawDesc('i', 10, 17);
		break;
	case 4:
		DrawDesc('c', 10, 17);
		break;
	}
}
void MenuInput() {
	MMInput = NONE;
	if (KeyIsDown('W', true, false))
		MMInput = UP;
	else if (KeyIsDown('S', true, false))
		MMInput = DOWN;
	else if (KeyIsDown(13, true, false))
		MMInput = START;
	else if (KeyIsDown(27, true, false) || KeyIsDown(8, true, false))
		MMInput = ESCAPE;
}
void MenuLogic() {
	switch (CurrentScreen) {
	case mainmenu:
		switch (MMInput) {
		case UP:
			mainmenu_option--;
			break;
		case DOWN:
			mainmenu_option++;
			break;
		case START:
			switch (mainmenu_option) {
			case 1:
				EXIT_MENU = true;
				EXIT_GAME = false;
				break;
			case 2:
				system("CLS");
				CurrentScreen = options;
				options_option = 1;
				break;
			case 3:
				system("CLS");
				CurrentScreen = howtoplay;
				break;
			case 4:
				EXIT_PROGRAM = true;
				EXIT_MENU = true;
				EXIT_GAME = true;
				break;
			}
		}
		break;
	case howtoplay:
		if (MMInput == ESCAPE) {
			system("CLS");
			CurrentScreen = mainmenu;
		}
	case options:
		switch (MMInput) {
		case UP:
			options_option--;
			break;
		case DOWN:
			options_option++;
			break;
		case START:
			switch (options_option) {
			case 1:
				ShowFPS = !ShowFPS;
				break;
			case 2:
				ShowCounter = !ShowCounter;
				break;
			case 3:
				break;
			case 4:
				system("CLS");
				CurrentScreen = mainmenu;
				mainmenu_option = 1;
				break;
			}
			break;
		case ESCAPE:
			system("CLS");
			CurrentScreen = mainmenu;
			mainmenu_option = 1;
		}
		break;
	}
	if (mainmenu_option < 1)
		mainmenu_option = 1;
	else if (mainmenu_option > 4)
		mainmenu_option = 4;
	if (options_option < 1)
		options_option = 1;
	else if (options_option > 4)
		options_option = 4;
	if (GetTimeSince(PacmanFrameTS) > .1) {
		MenuIteratePacman();
		PacmanFrameTS = GetTime();
	}
	if (GetTimeSince(GhostFrameTS) > .1) {
		GhostFrameTick++;
		if (GhostFrameTick > 2)
			GhostFrameTick = 1;
		GhostFrameTS = GetTime();
	}
	if (GetTimeSince(GhostIterationTS) > 4) {
		GhostIterationTick++;
		if (GhostIterationTick > 4)
			GhostIterationTick = 1;
		GhostIterationTS = GetTime();
	}
	return 0;
}

// Game Frame //
// -- End of Menu Frame
// -- Start of Game Frame

void GameSetup() {
	system("MODE 56, 37");
	PacmanX = 13;
@@ -255,7 +571,166 @@ void GameLogic() {
		PacmanX = MapWidth - 2;
}

////////////////
// -- End of Game Frame

void DrawSprite(int ArrPos, int x, int y, int size) {
	SelectObject(hdc, bmap);
	BitBlt(console, x, y, size, size, hdc, SpritePositions[ArrPos - 1][0], SpritePositions[ArrPos - 1][1], SRCCOPY);
	DeleteObject(bmap);
}

void SpriteTest(int x, int y) {
	for (int i = 0, j = 0, k = 1; k <= 89; i++, k++) {
		DrawSprite(k, (16 * i) + (x * 16), (j * 16) + (y * 16), GSS);
		if (i % 15 == 0) {
			j++;
			i = 0;
		}
	}
}

void DrawLogo(int x, int y) {
	SelectObject(hdc, bmap);
	BitBlt(console, x * 16, y * 16, 404, 91, hdc, 0, 301, SRCCOPY);
	DeleteObject(bmap);
}
void DrawDesc(char ghost, int x, int y) {
	switch (ghost) {
	case 'b':
		SelectObject(hdc, bmap);
		BitBlt(console, x * 16, y * 16, 272, 16, hdc, 2, 395, SRCCOPY);
		DeleteObject(bmap);
		break;
	case 'i':
		SelectObject(hdc, bmap);
		BitBlt(console, x * 16, y * 16, 272, 16, hdc, 2, 413, SRCCOPY);
		DeleteObject(bmap);
		break;
	case 'p':
		SelectObject(hdc, bmap);
		BitBlt(console, x * 16, y * 16, 272, 16, hdc, 2, 431, SRCCOPY);
		DeleteObject(bmap);
		break;
	case 'c':
		SelectObject(hdc, bmap);
		BitBlt(console, x * 16, y * 16, 272, 16, hdc, 2, 449, SRCCOPY);
		DeleteObject(bmap);
		break;
	}
}
// 272
void DrawString(const char text[30], int x, int y) {
	for (int i = 0;; i++) {
		if (text[i] == '\0')
			break;
		DrawSprite(GetCharSpriteID(tolower(text[i])), (i * GSS) + (x * GSS), y * GSS, GSS);
	}
}

int GetCharSpriteID(char character) {
	if (character == ' ')
		return 89;
	else if (character == '-')
		return 83;
	else if (character == ':')
		return 84;
	else if (character == '!')
		return 85;
	else if (character == '?')
		return 86;
	else if (character == '"')
		return 87;
	else if (character == '.')
		return 88;
	else if (character == '<')
		return 98;
	else if (character == '>')
		return 99;
	else if (character == '*') {
		switch (PacmanFrameMenu) {
		case 1:
			return 37;
			break;
		case 2:
			return 38;
			break;
		case 3:
			return 39;
			break;
		}
	}
	else if (character == '&') {
		switch (GhostIterationTick) {
		case 1: // Blinky
			if (GhostFrameTick == 1)
				return 90;
			else if (GhostFrameTick == 2)
				return 91;
			break;
		case 2: // Inky
			if (GhostFrameTick == 1)
				return 92;
			else if (GhostFrameTick == 2)
				return 93;
			break;
		case 3: // Pinky
			if (GhostFrameTick == 1)
				return 94;
			else if (GhostFrameTick == 2)
				return 95;
			break;
		case 4: // Clyde
			if (GhostFrameTick == 1)
				return 96;
			else if (GhostFrameTick == 2)
				return 97;
			break;
		}
	}
	else if (character >= 48 && character <= 57)
		return character - 1;
	character = tolower(character);
	return static_cast<int>(character) - 40;
}

void MenuIteratePacman() {
	PacmanFrameMenu++;
	if (PacmanFrameMenu > 3)
		PacmanFrameMenu = 1;
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

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void MovePacman() {
	double OldX = PacmanX;
	double OldY = PacmanY;
@@ -408,49 +883,11 @@ void DrawMap() {
	for (int y = 0; y < MapHeight; y++) {
		for (int x = 0; x < MapWidth; x++) {
			if ((Map[y][x] == 37 || Map[y][x] == 38 || Map[y][x] == 39) && !PlayerHasMadeFirstInput)
				DrawSprite(Map[y][x], (x * SpriteSize) + 8, (y * SpriteSize), SpriteSize);
				DrawSprite(Map[y][x], (x * GSS) + 8, (y * GSS), GSS);
			else if (Map[y][x] == 40 && !PlayerHasMadeFirstInput)
				continue;
			else if (Map[y][x] > 0)
				DrawSprite(Map[y][x], (x * SpriteSize), (y * SpriteSize), SpriteSize);
				DrawSprite(Map[y][x], (x * GSS), (y * GSS), GSS);
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