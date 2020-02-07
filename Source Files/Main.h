#pragma once
#include "Header.h"
class Main {
private:
	bool EndProgram;
public:
	Main() :EndProgram(false) { };
	bool ProgramStatus() const { return EndProgram; };
	void ExitProgram() { EndProgram = true; };
	void SpriteTest() {
		for (int i = 0; i < 500; i++) {
			DrawSprite(5 * res, 5 * res, i);
			Wait(.08);
		}
	}

};

