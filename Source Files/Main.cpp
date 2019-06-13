#include "Implementation.h"

int main() {
	system("MODE 29, 16");
	ShowConsoleCursor(false);
	bool EXIT_GAME = false;
	//SetGraphicsMode(Pac.hdc, GM_ADVANCED);
	TIMER_INIT
	while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
		TIMER_START
		GameDraw();
		GameInput();
		GameLogic();
		TIMER_STOP
	}
	return EXIT_SUCCESS;
}
