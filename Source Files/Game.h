#pragma once
#include "Header.h"
#include "Pac.h"
class Game {
private:
	Pac Player;

	bool EndGame;
	int Board[BH][BW] = {
	000,000,000,235,226,221,000,000,000,213,214,212,213,000,224,208,220,223,210,000,000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	287,296,296,296,296,296,296,296,296,296,296,296,296,321,320,296,296,296,296,296,296,296,296,296,296,296,296,286,
	289,322,322,322,322,322,322,322,322,322,322,322,322,307,306,322,322,322,322,322,322,322,322,322,322,322,322,288,
	289,322,305,298,298,304,322,305,298,298,298,304,322,307,306,322,305,298,298,298,304,322,305,298,298,304,322,288,
	289,324,307,000,000,306,322,307,000,000,000,306,322,307,306,322,307,000,000,000,306,322,307,000,000,306,324,288,
	289,322,309,303,303,308,322,309,303,303,303,308,322,309,308,322,309,303,303,303,308,322,309,303,303,308,322,288,
	289,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,288,
	289,322,305,298,298,304,322,305,304,322,305,298,298,298,298,298,298,304,322,305,304,322,305,298,298,304,322,288,
	289,322,309,303,303,308,322,307,306,322,309,303,303,317,316,303,303,308,322,307,306,322,309,303,303,308,322,288,
	289,322,322,322,322,322,322,307,306,322,322,322,322,307,306,322,322,322,322,307,306,322,322,322,322,322,322,288,
	291,297,297,297,297,304,322,307,318,298,298,304,402,307,306,000,305,298,298,319,306,322,305,297,297,297,297,290,
	000,000,000,000,000,289,322,307,316,303,303,308,000,309,308,000,309,303,303,317,306,322,288,000,000,000,000,000,
	000,000,000,000,000,289,322,307,306,000,000,000,000,000,000,000,000,000,000,307,306,322,288,000,000,000,000,000,
	000,000,000,000,000,289,322,307,306,000,311,297,315,401,401,314,297,310,000,307,306,322,288,000,000,000,000,000,
	296,296,296,296,296,308,322,309,308,000,288,000,000,000,000,000,000,289,000,309,308,322,309,296,296,296,296,296,
	000,000,000,000,000,000,322,000,000,000,288,000,000,000,000,000,000,289,000,000,000,322,000,000,000,000,000,000,
	297,297,297,297,297,304,322,305,304,000,288,000,000,000,000,000,000,289,000,305,304,322,305,297,297,297,297,297,
	000,000,000,000,000,289,322,307,306,000,313,296,296,296,296,296,296,312,000,307,306,322,288,000,000,000,000,000,
	000,000,000,000,000,289,322,307,306,000,000,000,000,000,000,000,000,000,000,307,306,322,288,000,000,000,000,000,
	000,000,000,000,000,289,322,307,306,000,305,298,298,298,298,298,298,304,000,307,306,322,288,000,000,000,000,000,
	287,296,296,296,296,308,322,309,308,000,309,303,303,317,316,303,303,308,000,309,308,322,309,296,296,296,296,286,
	289,322,322,322,322,322,322,322,322,322,322,322,322,307,306,322,322,322,322,322,322,322,322,322,322,322,322,288,
	289,322,305,298,298,304,322,305,298,298,298,304,322,307,306,322,305,298,298,298,304,322,305,298,298,304,322,288,
	289,322,309,303,317,306,322,309,303,303,303,308,322,309,308,322,309,303,303,303,308,322,307,316,303,308,322,288,
	289,324,322,322,307,306,322,322,322,322,322,322,322,000,000,322,322,322,322,322,322,322,307,306,322,322,324,288,
	293,298,304,322,307,306,322,305,304,322,305,298,298,298,298,298,298,304,322,305,304,322,307,306,322,305,298,292,
	295,303,308,322,309,308,322,307,306,322,309,303,303,317,316,303,303,308,322,307,306,322,309,308,322,309,303,294,
	289,322,322,322,322,322,322,307,306,322,322,322,322,307,306,322,322,322,322,307,306,322,322,322,322,322,322,288,
	289,322,305,298,298,298,298,319,318,298,298,304,322,307,306,322,305,298,298,319,318,298,298,298,298,304,322,288,
	289,322,309,303,303,303,303,303,303,303,303,308,322,309,308,322,309,303,303,303,303,303,303,303,303,308,322,288,
	289,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,322,288,
	291,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,297,290,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	}; 
	// Primary Board
	bool EnergizerFlashActive, _1UPFlashActive; // Stores activity of Energizer and 1UP flash respectively
	double EnergizerFlashTS, _1UPFlashTS; // Time stamps for Energizer and 1UP flash respectively
	bool GameStarted; // Stores if game has passed READY screen

	int DotsCollected; // Collected dots value

	// DEBUG
	bool DisplayFPS = true; // Displays FPS meter
	bool InstaDeath = false; // Instant beath button bound to '3'
	bool InstaLive = false; // Instant life button bound to '2'
	bool InstaEnd = false; // Instant win button bound to '1'
	bool LoadSpriteTest = false; // Applies Sprite Test on load

	const int FruitTable[20] = {407,411,415,415,419,419,423,423,427,427,431,431,435,435,435,435,435,435,435,435}; // Table for determining which fruit to draw per level

public:
	int Score; // Score value
	int HighScore; // Highscore value
	int Lives; // Lives value
	int Level; // Level value
	bool Death; // Bool to check if player died or won a level
	double FPSTS; int FPS, LastFPS; // FPS related variables
	double FruitTS; bool FruitAppear;
	double F_PointTS; bool F_PointAppear;
	bool Fruit1, Fruit2;
	int LastLifeEarned;

	Game(int Lvl, int Lv, int Scre, int HScre) :EndGame(false), LastLifeEarned(0), Fruit1(false), Fruit2(false), FruitTS(0), F_PointTS(0), FruitAppear(false), F_PointAppear(false), Death(false), GameStarted(false), EnergizerFlashActive(false), EnergizerFlashTS(GetTime()), _1UPFlashActive(false), _1UPFlashTS(GetTime()), Score(Scre), HighScore(HScre), Lives(Lv), Level(Lvl), DotsCollected(0), FPSTS(GetTime()), FPS(0), LastFPS(0) {
		InitializeCollision(); // Provides map collision
		DrawLevel(); // Level
		if (LoadSpriteTest)
			SpriteTest();
	};

	bool GameStatus() const; // Returns the status of the game
	void SetTile(int xpos, int ypos, int ID); // Sets a specified tile to a specified value
	bool UpdateTS(bool & State, double & TS, double Time); // Updates a given time stamp every amount of given time and updates its state accordingly
	void DrawCounter(int xpos, int ypos, int value); // Draws an integer counter for a given value
	int NumberToSprite(int value) const; // Converts integer to its respective sprite value
	void DrawLives(); // Draws life display
	void DrawLevel(); // Draws level display
	void MovePacman(); // Increments Pacman in face direction
	void InitializeCollision(); // Creates collision board based off current board
	void TeleportPlayer(); // Teleports player when touching specific positions
	void EatObjects(); // Applies logic regarding eating power pellets and energizers
	void LevelEnd(); // Ends the current level if the total dot count is 244
	void ClearCollisionTile(int xpos, int ypos); // Clears a tiles data on the collision board
	int GetFPS(); // Returns the current FPS
	void Flash(); // Flashes the map to its alternative color scheme
	void KillPlayer(); // Kills the player
	void GetReady(); // Displays ready text
	void SpawnFruit(); // Allows fruit to appear
	void PointsDraw(); // Draws points when fruit is eaten
	void FruitScore(); // Rewards player points from eaten fruit

	void Draw(); // Draws all display elements
	void Input(); // Rerceives input used for future logic
	void Logic(); // Applies all game logic

	void DrawBoard(); // Draws specifically the board
	void SpriteTest() { // Sprite test
		for (int i = 0; i <= 442; i++) {
			DrawSprite(5 * res, 5 * res, i);
			Wait(.02);
		}
		Wait(4);
	}

};

