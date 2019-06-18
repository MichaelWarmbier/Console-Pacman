#include "Implementation.h"

int main() {
	system("MODE 34, 20");
	ShowConsoleCursor(false);
	bool EXIT_GAME = false;
	//SetGraphicsMode(Pac.hdc, GM_ADVANCED);
	TIMER_INIT
		while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
			TIMER_START
				GameDraw();
			GameInput();
			if (GameState != LIMBO)
				GameLogic();
			TIMER_STOP
		}
	return EXIT_SUCCESS;
}
