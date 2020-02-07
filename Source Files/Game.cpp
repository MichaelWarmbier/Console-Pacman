#include "Game.h"

void Game::DrawBoard() {
	for (int y = 0; y < BH; y++) {
		for (int x = 0; x < BW; x++) {
			if (Board[y][x] > 000)
				DrawSprite(x * res, y * res, Board[y][x]);
		}
	}
}

bool Game::GameStatus() const {
	return EndGame;
}
