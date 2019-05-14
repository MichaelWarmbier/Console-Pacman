#include "Implementation.h"
int main() {
	bool EXIT_GAME = false;
	while (!EXIT_GAME && (timer += (dt = FPS + wait(FPS)))) {
		GameDraw();
		GameInput();
		GameLogic();
	}
	return EXIT_SUCCESS;
}
