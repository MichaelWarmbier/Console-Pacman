// Flags for exiting primary loops
bool EXIT_GAME_F = false;
bool EXIT_PROGRAM_F = false;

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

HDC console = GetDC(GetConsoleWindow());
HDC hdc = CreateCompatibleDC(NULL);
HBITMAP bmap = (HBITMAP)LoadImage(NULL, _T("ConsolePacmanSpriteSheet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

typedef unsigned char b_int; // Defined int with a range of 256
typedef bool bin_int; // Defined int with a range of 2 (0,1);

const int MH = 36, MW = 28; // Board Dimensions

enum Input { UP, DOWN, LEFT, RIGHT, NONE }; // Datatype labels received player input
enum GameState { LIMBO, BEFORE, DURING, AFTER }; // Datatype determines state of gameplay
enum class GhostType { INIT, BLINKY, INKY, PINKY, CLYDE}; // Datatype determines ghost type
enum GhostState { CHASE, SCATTER, PANIC, INTRO}; // Datatype determines state of ghost AIs

Input PlayerInput = NONE;
Input OldInput = NONE;

const b_int SpriteIDs[256][2]{
	//x,y//
	  0,0, // Empty // 1
	  16,0, // #0 // 2
	  32,0, // #1 // 3
	  48,0, // #2 // 4
	  64,0, // #3 // 5
	  80,0, // #4 // 6
	  96,0, // #5 // 7
	  112,0, // #6 // 8
	  128,0, // #7 // 9
	  144,0, // #8 // 10
	  160,0, // #9 // 11
	  176,0, // A // 12
	  192,0, // B // 13
	  208,0, // C // 14
	  224,0, // D // 15
	  240,0, // E // 16

	  0,16, // F // 17
	  16,16, // G // 18
	  32,16, // H // 19
	  48,16, // I // 20
	  64,16, // J // 21
	  80,16, // K // 22
	  96,16, // L // 23
	  112,16, // M // 24
	  128,16, // N // 25
	  144,16, // O // 26
	  160,16, // P // 27
	  176,16, // Q // 28
	  192,16, // R // 29
	  208,16, // S // 30
	  224,16, // T // 31
	  240,16, // U // 32

	  0,32, // V // 33
	  16,32, // W // 34
	  32,32, // X // 35
	  48,32, // Y // 36
	  64,32, // Z // 37
	  80,32, // - // 38
	  96,32, // : // 39
	  112,32, // ! // 40
	  128,32, // ? // 41
	  144,32, // " // 42
	  160,32, // . // 43
	  176,32, // Thick Wall Lean Left // 44
	  192,32, // Thick Wall Lean Up // 45
	  208,32, // Thick Wall Lean Down // 46
	  224,32, // Thick Wall Lean Right // 47
	  240,32, // Thin Wall Lean Right // 48

	  0,48, // Thin Wall Lean Up // 49
	  16,48, // Thin Wall Lean Left // 50
	  32,48, // Thin Wall Lean Down // 51
	  48,48, // Gate Hinge Wall From Right // 52
	  64,48, // Gate Hinge Wall From Left // 53
	  80,48, // Thick Corner R1 // 54
	  96,48, // Thick Corner R2 // 55
	  112,48, // Thick Corner R3 // 56
	  128,48, // Thick Corner R4 // 57
	  144,48, // Thin Corner R1 // 58
	  160,48, // Thin Corner R2 // 59
	  176,48, // Thin Corner R3 // 60
	  192,48, // Thin Corner R4 // 61
	  208,48, // Half Corner R1 // 62
	  224,48, // Half Corner R2 // 63
	  240,48, // Half Corner R3 // 64

	  0,64, // Half Corner R4 // 65
	  16,64, // Gate Corner R1 // 66
	  32,64, // Gate Corner R2 // 67
	  48,64, // Gate Corner R3 // 68
	  64,64, // Gate Corner R4 // 69
	  80,64, // Connector R1 // 70
	  96,64, // Connector R2 // 71
	  112,64, // Connector R3 // 72
	  128,64, // Connector R4 // 73
	  144,64, // Connector R5 // 74
	  160,64, // Connector R6 // 75
	  176,64, // Connector R7 // 76
	  192,64, // Gate // 77
	  208,64, // Pellet // 78
	  224,64, // Power Pellet // 79
	  240,64, // Cherry // 80

	  0,80, // Strawberry // 81
	  16,80, // Orange // 82
	  32,80, // Apple // 83
	  48,80, // Melon // 84
	  64,80, // Spaceship // 85
	  80,80, // Bell // 86
	  96,80, // Key // 87
	  112,80, // Pacman P1 // 88
	  128,80, // Pacman P2 R // 89
	  144,80, // Pacman P3 R // 90
	  160,80, // Pacman P2 D // 91
	  176,80, // Pacman P3 D // 92
	  192,80, // Pacman P2 L // 93
	  208,80, // Pacman P3 L // 94
	  224,80, // Pacman P2 U // 95
	  240,80, // Pacman P2 U // 96

	  0,0, // PP Clear // 97
	  0,0, // Other Clear // 98

	  0,128, // YR // 99
	  16,128, // YE // 100
	  32,128, // YA // 101
	  48,128, // YD // 102
	  64,128, // YY // 103
	  80,128, // Y! // 104

	  224,144, // Thick Wall Lean Left // 105
	  240,144, // Thick Wall Lean Up // 106
	  0,160, // Thick Wall Lean Down // 107
	  16,160, // Thick Wall Lean Right // 108
	  32,160, // Thin Wall Lean Right // 109
	  48,160, // Thin Wall Lean Up // 110
	  64,160, // Thin Wall Lean Left // 111
	  80,160, // Thin Wall Lean Down // 112
	  96,160, // Gate Hinge Wall From Right // 113
	  112,160, // Gate Hinge Wall From Left // 114
	  128,160, // Thick Corner R1 // 115
	  144,160, // Thick Corner R2 // 116
	  160,160, // Thick Corner R3 // 117
	  176,160, // Thick Corner R4 // 118
	  192,160, // Thin Corner R1 // 119
	  208,160, // Thin Corner R2 // 120
	  224,160, // Thin Corner R3 // 121
	  240,160, // Thin Corner R4 // 122
	  0,176, // Half Corner R1 // 123
	  16,176, // Half Corner R2 // 124
	  32,176, // Half Corner R3 // 125
	  48,176, // Half Corner R4 // 126
	  64,176, // Gate Corner R1 // 127
	  80,176, // Gate Corner R2 // 128
	  96,176, // Gate Corner R3 // 129
	  112,176, // Gate Corner R4 // 130
	  128,176, // Connector R1 // 131
	  144,176, // Connector R2 // 132
	  160,176, // Connector R3 // 133
	  176,176, // Connector R4 // 134
	  192,176, // Connector R5 // 135
	  208,176, // Connector R6 // 136
	  224,176, // Connector R7 // 137

	  0, 96, // Blinky R P1 // 138
	  16, 96, // Blinky R P2 // 139
	  32, 96, // Blinky D P1 // 140
	  48, 96, // Blinky D P2 // 141
	  64, 96, // Blinky U P1 // 142
	  80, 96, // Blinky U P2 // 143
	  96, 96, // Blinky L P1 // 144
	  112, 96, // Blinky L P2 // 145

	  128, 96, // Inky R P1 // 146
	  144, 96, // Inky R P2 // 147
	  160, 96, // Inky D P1 // 148
	  176, 96, // Inky D P2 // 149
	  192, 96, // Inky U P1 // 150
	  208, 96, // Inky U P2 // 151
	  224, 96, // Inky L P1 // 152
	  240, 96, // Inky L P2 // 153

	  0, 112, // Pinky R P1 // 154
	  16, 112, // Pinky R P2 // 155
	  32, 112, // Pinky D P1 // 156
	  48, 112, // Pinky D P2 // 157
	  64, 112, // Pinky U P1 // 158
	  80, 112, // Pinky U P2 // 159
	  96, 112, // Pinky L P1 // 160
	  112, 112, // Pinky L P2 // 161

	  128, 112, // Clyde R P1 // 162
	  144, 112, // Clyde R P2 // 163
	  160, 112, // Clyde D P1 // 164
	  176, 112, // Clyde D P2 // 165
	  192, 112, // Clyde U P1 // 166
	  208, 112, // Clyde U P2 // 167
	  224, 112, // Clyde L P1 // 168
	  240, 112, // Clyde L P2 // 169

};

void UpdateTS(double& timestamp);
void DrawSprite(int spriteID, double posx, double posy); // Draws 16x16 bitmap sprite
void ShowConsoleCursor(bool showFlag); // Hides/shows console cursor
double GetTimeSince(double startTime); // Returns time since argument
bool KeyIsDown(char key, bool pressed, bool held); // Returns true if key is down
double GetTime(); // Returns current time
double Wait(double waitTime); // Pauses runtime temporarily
void SetWindowDimensions(int x, int y);

// CLASS WILL BE WRITTEN WITHOUT INLINE FUNCTIONS LATER
class Ghost {
	friend class Pacman;
	friend class PacmanGame;
private:
	Input dir;
	bin_int phase;
	double _PhaseTS;
	GhostType type = GhostType::INIT;
	GhostState state;
	double tX, tY;
	double x, y;
public:
	void TogglePhase();
	Ghost() : type(GhostType::INIT), x(0), y(0), tX(0), tY(0), phase(0), dir(RIGHT), state(CHASE), _PhaseTS(GetTime()) { } 
	Ghost(const GhostType ghostName) : tX(0), tY(0), phase(0), dir(RIGHT), state(CHASE), _PhaseTS(GetTime()), type(ghostName) {
		switch (ghostName) {
		case GhostType::BLINKY:
			x = 216.0;
			y = 224.0;
			break;
		case GhostType::INKY:
			x = 200.0;
			y = 272.0;
			break;
		case GhostType::PINKY:
			x = 216.0;
			y = 272.0;
			break;
		case GhostType::CLYDE:
			x = 232.0;
			y = 272.0;
			break;
		default:
			x = 0;
			y = 0;
			break;
		}
	};
	void Logic() {
		DrawGhost();
		if (GetTimeSince(_PhaseTS) > .08) {
			_PhaseTS = GetTime();
			TogglePhase();
		}
	}
	void DrawGhost() const {
		switch (type) {
		case GhostType::BLINKY:
			switch (dir) {
			case RIGHT:
				DrawSprite(138 + phase, x, y);
				break;
			case DOWN:
				DrawSprite(140 + phase, x, y);
				break;
			case UP:
				DrawSprite(142 + phase, x, y);
				break;
			case LEFT:
				DrawSprite(144 + phase, x, y);
				break;
			}
			break;
		case GhostType::INKY:
			switch (dir) {
			case RIGHT:
				DrawSprite(146 + phase, x, y);
				break;
			case DOWN:
				DrawSprite(148 + phase, x, y);
				break;
			case UP:
				DrawSprite(150 + phase, x, y);
				break;
			case LEFT:
				DrawSprite(152 + phase, x, y);
				break;
			}
			break;
		case GhostType::PINKY:
			switch (dir) {
			case RIGHT:
				DrawSprite(154 + phase, x, y);
				break;
			case DOWN:
				DrawSprite(156 + phase, x, y);
				break;
			case UP:
				DrawSprite(158 + phase, x, y);
				break;
			case LEFT:
				DrawSprite(160 + phase, x, y);
				break;
			}
			break;
		case GhostType::CLYDE:
			switch (dir) {
			case RIGHT:
				DrawSprite(162 + phase, x, y);
				break;
			case DOWN:
				DrawSprite(164 + phase, x, y);
				break;
			case UP:
				DrawSprite(166 + phase, x, y);
				break;
			case LEFT:
				DrawSprite(168 + phase, x, y);
				break;
			}
			break;
		}
	}
};

class Pacman {
	friend class PacmanGame;
	friend class Ghost;
private:
	double _PhaseTS; // Timestamp for animation
	int SpritePhase; // Phase of animation
	double PosX, PosY; // Floating point position
	void DrawPlayer() const;
	void TogglePhase();
	double speed;
public:
	Pacman();
};

class PacmanGame {
	friend class Pacman;
	friend class Ghost;
private:
	int CollisionBoard[MH * 16][MW * 16];
	int Board[MH][MW] = {
		01,01,01,03,32,27,01,01,01,19,20,18,19,01,30,14,26,29,16,01,01,01,01,01,01,01,01,01,
		01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
		01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
		54,45,45,45,45,45,45,45,45,45,45,45,45,73,74,45,45,45,45,45,45,45,45,45,45,45,45,56,
		44,78,78,78,78,78,78,78,78,78,78,78,78,48,50,78,78,78,78,78,78,78,78,78,78,78,78,46,
		44,78,58,51,51,60,78,58,51,51,51,60,78,48,50,78,58,51,51,51,60,78,58,51,51,60,78,46,
		44,79,48,01,01,50,78,48,01,01,01,50,78,48,50,78,48,01,01,01,50,78,48,01,01,50,79,46,
		44,78,59,49,49,61,78,59,49,49,49,61,78,59,61,78,59,49,49,49,61,78,59,49,49,61,78,46,
		44,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,46,
		44,78,58,51,51,60,78,58,60,78,58,51,51,51,51,51,51,60,78,58,60,78,58,51,51,60,78,46,
		44,78,59,49,49,61,78,48,50,78,59,49,49,65,62,49,49,61,78,48,50,78,59,49,49,61,78,46,
		44,78,78,78,78,78,78,48,50,78,78,78,78,48,50,78,78,78,78,48,50,78,78,78,78,78,78,46,
		55,47,47,47,47,60,78,48,63,51,51,60,01,48,50,01,58,51,51,64,50,78,58,47,47,47,47,57,
		01,01,01,01,01,44,78,48,62,49,49,61,01,59,61,01,59,49,49,65,50,78,46,01,01,01,01,01,
		01,01,01,01,01,44,78,48,50,01,01,01,01,01,01,01,01,01,01,48,50,78,46,01,01,01,01,01,
		01,01,01,01,01,44,78,48,50,01,66,47,53,77,77,52,47,69,01,48,50,78,46,01,01,01,01,01,
		45,45,45,45,45,61,78,59,61,01,46,01,01,01,01,01,01,44,01,59,61,78,59,45,45,45,45,45,
		01,01,01,01,01,01,78,01,01,01,46,01,01,01,01,01,01,44,01,01,01,78,01,01,01,01,01,01,
		47,47,47,47,47,60,78,58,60,01,46,01,01,01,01,01,01,44,01,58,60,78,58,47,47,47,47,47,
		01,01,01,01,01,44,78,48,50,01,67,45,45,45,45,45,45,68,01,48,50,78,46,01,01,01,01,01,
		01,01,01,01,01,44,78,48,50,01,01,01,01,01,01,01,01,01,01,48,50,78,46,01,01,01,01,01,
		01,01,01,01,01,44,78,48,50,01,58,51,51,51,51,51,51,60,01,48,50,78,46,01,01,01,01,01,
		54,45,45,45,45,61,78,59,61,01,59,49,49,65,62,49,49,61,01,59,61,78,59,45,45,45,45,56,
		44,78,78,78,78,78,78,78,78,78,78,78,78,48,50,78,78,78,78,78,78,78,78,78,78,78,78,46,
		44,78,58,51,51,60,78,58,51,51,51,60,78,48,50,78,58,51,51,51,60,78,58,51,51,60,78,46,
		44,78,59,49,65,50,78,59,49,49,49,61,78,59,61,78,59,49,49,49,61,78,48,62,49,61,78,46,
		44,79,78,78,48,50,78,78,78,78,78,78,78,01,01,78,78,78,78,78,78,78,48,50,78,78,79,46,
		75,51,60,78,48,50,78,58,60,78,58,51,51,51,51,51,51,60,78,58,60,78,48,50,78,58,51,72,
		70,49,61,78,59,61,78,48,50,78,59,49,49,65,62,49,49,61,78,48,50,78,59,61,78,59,49,76,
		44,78,78,78,78,78,78,48,50,78,78,78,78,48,50,78,78,78,78,48,50,78,78,78,78,78,78,46,
		44,78,58,51,51,51,51,64,63,51,51,60,78,48,50,78,58,51,51,64,63,51,51,51,51,60,78,46,
		44,78,59,49,49,49,49,49,49,49,49,61,78,59,61,78,59,49,49,49,49,49,49,49,49,61,78,46,
		44,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,46,
		55,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,57,
		01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
		01,01,01,01,01,01,01,01,01,01,01,01,01,01,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	};
	GameState StateOfGame;
	void TeleportPacman();
	void ChangeTile(int grix, int gridy, int newTileID); // Sets oldTile to newTile
	void DrawBoard(); // Outputs board through sprites
	void Toggle1UP(); // Hides or shows 1UP visual
	void TogglePP(); // Hides or shows Power Pellets
	int GetTileID(int gridx, int gridy) const; // Returns ID of given coordinate
	void DrawLives(); // Draws life counter
	void DrawLevelDisplay(); // Draws level counter
	void DisplayInteger(int x, int y, int integer); // Draws integers as sprites
	bool CheckCol(double x, double y, int ID) const; // Returns true if tile is collidable
	void DotGrab();
	void ToggleMapColor();
	void ToggleReady();
	void CheckDotData();
	void InitializeColBoard();
	Ghost* Blinky = new Ghost(GhostType::BLINKY);
	Ghost* Inky = new Ghost(GhostType::INKY);
	Ghost* Pinky = new Ghost(GhostType::PINKY);
	Ghost* Clyde = new Ghost(GhostType::CLYDE);
	Pacman Player;

public:
	int score; // Score per game
	int collected_dots; // Collected dots per round
	double _1UP_TS, _PP_TS; // Timestamps
	int PlayerLives; // Remaining extra lives
	b_int PlayerLevel; // Level per game
	void Draw();
	void Logic();
	void UserInput();
	PacmanGame();
	PacmanGame(int level);
};

int main() {
	SetWindowDimensions(29, 39);
	int temp_level = 1;
	do {
		PacmanGame* Game = new PacmanGame(temp_level);
		while (!EXIT_GAME_F) {
			Game->Draw();
			Game->Logic();
		}
		if (!EXIT_PROGRAM_F) {
			temp_level++;
			EXIT_GAME_F = false;
		}
		delete Game;
	} while (!EXIT_PROGRAM_F);
	return EXIT_SUCCESS;
}

void PacmanGame::Draw() {
	DrawBoard();
	DrawLives();
	DrawLevelDisplay();
	DisplayInteger(1, 1, score);
	DisplayInteger(11, 1, 0);
	DisplayInteger(10, MH - 2, collected_dots);
	Player.DrawPlayer();
}

void PacmanGame::InitializeColBoard() {
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

void PacmanGame::Logic() {
	Blinky->Logic();
	Inky->Logic();
	Pinky->Logic();
	Clyde->Logic();
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
			Player.DrawPlayer();
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
		UserInput();
		for (double i = 0; i < Player.speed; i += .2) {
			if (OldInput != PlayerInput && PlayerInput != NONE)
				OldInput = PlayerInput;
			if (!CheckCol(Player.PosX - 1, Player.PosY, 1) && !CheckCol(Player.PosX - 1, Player.PosY + 15, 1) && (PlayerInput == LEFT || OldInput == LEFT)) {
				Player.PosX--;
				PlayerInput = LEFT;
			}
			else if (!CheckCol(Player.PosX + 16, Player.PosY, 1) && !CheckCol(Player.PosX + 16, Player.PosY + 15, 1) && (PlayerInput == RIGHT || OldInput == RIGHT)) {
				Player.PosX++;
				PlayerInput = RIGHT;
			}
			else if (!CheckCol(Player.PosX, Player.PosY - 1, 1) && !CheckCol(Player.PosX + 15, Player.PosY - 1, 1) && (PlayerInput == UP || OldInput == UP)) {
				Player.PosY--;
				PlayerInput = UP;
			}
			else if (!CheckCol(Player.PosX, Player.PosY + 16, 1) && !CheckCol(Player.PosX + 15, Player.PosY + 16, 1) && (PlayerInput == DOWN || OldInput == DOWN)) {
				Player.PosY++;
				PlayerInput = DOWN;
			}
			TeleportPacman();
			Player.DrawPlayer();
		}
		if (GetTimeSince(_1UP_TS) > .3) {
			Toggle1UP();
			UpdateTS(_1UP_TS);
		}
		if (GetTimeSince(_PP_TS) > .2) {
			TogglePP();
			UpdateTS(_PP_TS);
		}
		if (GetTimeSince(Player._PhaseTS) > .08) {
			Player.TogglePhase();
			UpdateTS(Player._PhaseTS);
		}
		DotGrab();
		CheckDotData();
	}
}

PacmanGame::PacmanGame() {
	InitializeColBoard();
	UpdateTS(_1UP_TS);
	UpdateTS(_PP_TS);
	PlayerLives = 3;
	PlayerLevel = 1;
	StateOfGame = BEFORE;
	collected_dots = 244;
	score = 0;
}
PacmanGame::PacmanGame(int level) {
	InitializeColBoard();
	UpdateTS(_1UP_TS);
	UpdateTS(_PP_TS);
	PlayerLives = 3;
	PlayerLevel = level;
	StateOfGame = BEFORE;
	collected_dots = 244;
	score = 0;
}
void PacmanGame::DrawLives() {
	for (int i = 0; i < 5; i++) {
		if (i < PlayerLives - 1)
			ChangeTile(i + 3, MH - 2, 93);
		else
			ChangeTile(i + 3, MH - 2, 1);
	}
}

void PacmanGame::Toggle1UP() {
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

void PacmanGame::ToggleReady() {
	for (int i = 0; i < 6; i++) {
		ChangeTile(11 + i, 20, 98);
		Draw();
		ChangeTile(11 + i, 20, 1);
	}
}

void PacmanGame::TogglePP() {
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

void PacmanGame::DrawLevelDisplay() {
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

void PacmanGame::DisplayInteger(int x, int y, int integer) {
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

void PacmanGame::ChangeTile(int gridx, int gridy, int newTileID) {
	Board[gridy][gridx] = newTileID;
}

void PacmanGame::CheckDotData() {
	collected_dots = 0;
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			if (GetTileID(x, y) == 79 || GetTileID(x, y) == 78 || GetTileID(x, y) == 97)
				collected_dots++;
		}
	}
}

void PacmanGame::DotGrab() { // WIP
	switch (PlayerInput) {
	case LEFT:
		if (CheckCol(Player.PosX + 6, Player.PosY + 4, 2)) {
			ChangeTile(static_cast<int>(Player.PosX + 6) / 16, (static_cast<int>(Player.PosY) + 4) / 16, 0);
		}
		break;
	case RIGHT:
		if (CheckCol(Player.PosX + 6, Player.PosY + 4, 2)) {
			ChangeTile(static_cast<int>(Player.PosX + 6) / 16, (static_cast<int>(Player.PosY) + 4) / 16, 0);
		}
		break;
	case UP:
		if (CheckCol(Player.PosX + 4, Player.PosY + 6, 2)) {
			ChangeTile(static_cast<int>(Player.PosX + 4) / 16, (static_cast<int>(Player.PosY) + 6) / 16, 0);
		}
		break;
	case DOWN:
		if (CheckCol(Player.PosX + 4, Player.PosY + 6, 2)) {
			ChangeTile(static_cast<int>(Player.PosX + 4) / 16, (static_cast<int>(Player.PosY) + 6) / 16, 0);
		}
		break;
	}
}

void PacmanGame::DrawBoard() {
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			if (Board[y][x] == 01)
				continue;
			DrawSprite(Board[y][x], x * 16.0, y * 16.0);
		}
	}
}

int PacmanGame::GetTileID(int gridx, int gridy) const {
	return Board[gridy][gridx];
}

Pacman::Pacman() {
	SpritePhase = 2;
	_PhaseTS = GetTime();
	PosX = 13 * 16.0 + 8;
	PosY = 26 * 16.0;
	PlayerInput = NONE;
	speed = .4;
}

void PacmanGame::ToggleMapColor() {
	for (int y = 0; y < MH; y++) {
		for (int x = 0; x < MW; x++) {
			if (GetTileID(x, y) < 77 && GetTileID(x, y) > 43)
				Board[y][x] += 61;
			else if (GetTileID(x, y) < 138 && GetTileID(x, y) > 104)
				Board[y][x] -= 61;
		}
	}
}

bool PacmanGame::CheckCol(double x, double y, int ID) const {
	if (CollisionBoard[int(y)][int(x)] == ID)
		return true;
	return false;
}

void Pacman::TogglePhase() {
	SpritePhase++;
	if (SpritePhase > 3)
		SpritePhase = 1;
}

void Ghost::TogglePhase() {
	phase = !phase;
}

void PacmanGame::TeleportPacman() {
	if (Player.PosY == 272) {
		if (Player.PosX == 432) {
			DrawSprite(1, Player.PosX, Player.PosY);
			Player.PosX = 0;
		}
		else if (Player.PosX == 0) {
			DrawSprite(1, Player.PosX, Player.PosY);
			Player.PosX = 432;
		}
	}
}

void Pacman::DrawPlayer() const {
	switch (SpritePhase) {
	case 1:
		DrawSprite(88, PosX, PosY);
		break;
	case 2:
		switch (PlayerInput) {
		case LEFT:
			DrawSprite(93, PosX, PosY);
			break;
		case RIGHT:
			DrawSprite(89, PosX, PosY);
			break;
		case UP:
			DrawSprite(95, PosX, PosY);
			break;
		case DOWN:
			DrawSprite(91, PosX, PosY);
			break;
		}
		break;
	case 3:
		switch (PlayerInput) {
		case LEFT:
			DrawSprite(94, PosX, PosY);
			break;
		case RIGHT:
			DrawSprite(90, PosX, PosY);
			break;
		case UP:
			DrawSprite(96, PosX, PosY);
			break;
		case DOWN:
			DrawSprite(92, PosX, PosY);
			break;
		}
		break;
	}
}

void PacmanGame::UserInput() {
	// Alt controls TBA
	if (KeyIsDown('W', true, true) && !CheckCol(Player.PosX, Player.PosY - 1, 1) && !CheckCol(Player.PosX + 15, Player.PosY - 1, 1))
		PlayerInput = UP;
	if (KeyIsDown('S', true, true) && !CheckCol(Player.PosX, Player.PosY + 16, 1) && !CheckCol(Player.PosX + 15, Player.PosY + 16, 1))
		PlayerInput = DOWN;
	if (KeyIsDown('A', true, true) && !CheckCol(Player.PosX - 1, Player.PosY, 1) && !CheckCol(Player.PosX - 1, Player.PosY + 15, 1))
		PlayerInput = LEFT;
	if (KeyIsDown('D', true, true) && !CheckCol(Player.PosX + 16, Player.PosY, 1) && !CheckCol(Player.PosX + 16, Player.PosY + 15, 1))
		PlayerInput = RIGHT;

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

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void DrawSprite(int spriteID, double posx, double posy) {
	if (spriteID > 0) {
		SelectObject(hdc, bmap);
		BitBlt(console, (int)posx, (int)posy, 16, 16, hdc, SpriteIDs[spriteID - 1][0], SpriteIDs[spriteID - 1][1], SRCCOPY);
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
