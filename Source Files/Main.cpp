#include "Implementation.h"

int main() {
	do {
		MenuSetup();
		do {
			MenuDraw();
			MenuLogic();
			MenuInput();
		} while (!EXIT_MENU);
		if (!EXIT_GAME)
			GameSetup();
		TIMER_INIT
			while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
				TIMER_START
					GameDraw();
				GameInput();
				if (GameState != LIMBO && GameState != BEFORE)
					GameLogic();
				if (GameState == BEFORE) {
					DrawReady(true);
					if (GetTimeSince(TimeStamp) >= 5) {
						GameState = DURING;
						DrawReady(false);
						system("CLS");
					}
				}
				TIMER_STOP
			}
	} while (!EXIT_PROGRAM);
	return EXIT_SUCCESS;
}
