#pragma once
#include "Header.h"
class TitleScreen {
private:
	int OptSel;
	int Screen;
public:
	void Draw();
	void Input();
	void Logic();
	bool MoveCursor(dir VDirection);
};

