#include "Implementation.h"

int main() {
	ShowConsoleCursor(false);
	bool EXIT_GAME = false;
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
