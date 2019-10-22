#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

bool EXIT_GAME_F = false;
bool EXIT_PROGRAM_F = false;

bool newGame = true;

double BDfP = 0;

HDC console = GetDC(GetConsoleWindow());
HDC hdc = CreateCompatibleDC(NULL);
HBITMAP bmap = (HBITMAP)LoadImage(NULL, _T("ConsolePacmanSpriteSheet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

typedef unsigned char b_int;
typedef bool bin_int;

const int MH = 36, MW = 28;
int pX, pY; // Player Position

double FPS = 1.0 / 150.0;
double timer = 0, dt = 0;

// Debug
bool ShowRemainingPellets = false;
bool CanGameOver = true;
// Debug

enum DIR { UP, DOWN, LEFT, RIGHT, NONE };
enum GameState { END, BEFORE, DURING, AFTER};
enum GhostState { CHASE, SCATTER, PANIC, EATEN, INTRO };

DIR PlayerInput = NONE;
DIR OldInput = NONE;

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

	  112, 224, // Blinky X // 170
      128, 224, // Inky X // 171
	  144, 224, // Pinky X // 172
	  160, 224, // Clyde X // 173

	  224, 128, // Pacman Death P1 // 174
	  240, 128, // Pacman Death P2 // 175
	  0, 144, // Pacman Death P3 // 176
	  16, 144, // Pacman Death P4 // 177
   	  32, 144, // Pacman Death P5 // 178
      64, 144, // Pacman Death P6 // 179
	  80, 144, // Pacman Death P7 // 180
	  96, 144, // Pacman Death P8 // 181
	  112, 144, // Pacman Death P9 // 182
	  128, 144, // Pacman Death P10 // 183
	  144, 144, // Pacman Death P11 // 184

};

void UpdateTS(double& timestamp);
void DrawSprite(int spriteID, double posx, double posy);
void ShowConsoleCursor(bool showFlag);
double GetTimeSince(double startTime);
bool KeyIsDown(char key, bool pressed, bool held);
double GetTime();
double Wait(double waitTime);
void SetWindowDimensions(int x, int y);
double GetDistanceOf(int x1, int y1, int x2, int y2);
void CopyBoardData(int Arr1[MH][MW], int Arr2[MW][MW]);
void InitializeBoardData(int Arr[MH][MW]);

class Game { // Game Class
public:
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
private:
	class Player { // Player Class
		friend class G1;
		friend class G2;
	private:
		int SpritePhase; // Marker for current phase
	public:
		double _PhaseTS; // Timestamp for checking when to increment player phase
		double speed; // Movement speed

		Player(); // Constructor

		void Draw() const; // Function to draw player
		void TogglePhase(); // Toggles player visuals
	};

	class G1 {
	private:
		int GhostBoard[MH][MW];

		bin_int phase;
	public:
		DIR face;
		GhostState state;
		double _PhaseTS;
		int X, Y;
		int tX, tY;
		double speed;

		void Draw();
		void Logic();
		void AI();

		void Advance();
		void TogglePhase();
		bool CheckTile(int x, int y);
		void FindPath();
		void Teleport();

		G1() {
			InitializeBoardData(GhostBoard);
			X = 216.0; Y = 224.0;
			tX = pX; tY = pY;
			speed = .4;
			state = CHASE;
			if (newGame)
				face = LEFT;
			else
				face = RIGHT;
			_PhaseTS = GetTime();
			phase = 0;
		}
		G1(int Arr[MH][MW]) {
			CopyBoardData(Arr, GhostBoard);
			X = 216.0; Y = 224.0;
			tX = pX; tY = pY;
			state = CHASE;
			if (newGame)
				face = LEFT;
			else
				face = RIGHT;
			_PhaseTS = GetTime();
			phase = 0;
			speed = .4;
		};
	};
	class G2 {
	private:
		int GhostBoard[MH][MW];

		bin_int phase;
	public:
		DIR face;
		GhostState state;
		double _PhaseTS;
		int X, Y;
		int tX, tY;
		double speed;

		void Draw();
		void Logic();
		void AI();

		void Advance();
		void TogglePhase();
		bool CheckTile(int x, int y);
		void FindPath();
		void Teleport();

		G2() {
			InitializeBoardData(GhostBoard);
			X = 216.0; Y = 224.0;
			tX = pX; tY = pY;
			speed = .4;
			state = CHASE;
			if (newGame)
				face = LEFT;
			else
				face = RIGHT;
			_PhaseTS = GetTime();
			phase = 0;
		}
		G2(int Arr[MH][MW]) {
			CopyBoardData(Arr, GhostBoard);
			X = 216.0; Y = 224.0;
			tX = pX; tY = pY;
			state = CHASE;
			if (newGame)
				face = LEFT;
			else
				face = RIGHT;
			_PhaseTS = GetTime();
			phase = 0;
			speed = .4;
		};
	};


	void Teleport();
	void ChangeTile(int gridx, int gridy, int newTileID);
	void DrawBoard();
	void Toggle1UP();
	void TogglePP();
	void ToggleModes();
	int GetTileID(int gridx, int gridy) const;
	void DrawLives();
	void DrawLevelDisplay();
	void DisplayInteger(int x, int y, int integer);
	void DotGrab();
	void ToggleMapColor();
	void ToggleReady();
	void CheckDotData();
	void GameOver();
	void InitializeColBoard();
	void TurnAround();
	bool GetCol(int x, int y, int ID) const;

	Player P1;
	G1* Blinky = new G1(Board);
	G2* Pinky = new G2(Board);

	bool S[8] = { 1,1,1,1,1,1,1,1 };

public:
	void InitializeBoard(int data[MH][MW]);
	void Draw();
	void Logic();
	void Input();

	Game();
	Game(int level, int lives);

	GameState StateOfGame;
	int score;
	int collected_dots;
	double _1UP_TS, _PP_TS, _SWAP_TS; // Time stamps
	int PlayerLives;
	b_int PlayerLevel;
};
