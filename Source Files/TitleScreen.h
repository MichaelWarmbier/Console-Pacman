#pragma once
#include "Main.h"
class TitleScreen {
private:
	bool EndMenu;
	int CursorSel; // Stores the currently selected option
	double GhostTS, PacmanTS;
	int PacmanPhase;
	bool GhostFrame;

public:
	bool EXIT;

	bool MenuStatus() const { return EndMenu; }; // Rerturn program status
	void ExitMenu() { EndMenu = true; }; // Exit program
	TitleScreen() : CursorSel(1), EXIT(false), EndMenu(false), GhostTS(GetTime()), GhostFrame(0), PacmanPhase(0), PacmanTS(GetTime()) { }

	void Draw();
	void Input();
	void Logic();

	void StringToSprite(const char text[50], int xpos, int ypos, COLOR color) const;
	int CharToSprite(char c, COLOR color) const;
	int CursorLogic();
};

