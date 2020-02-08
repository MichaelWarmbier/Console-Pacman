#include "Pac.h"
void Pac::DrawPlayer() const {
	if (Phase == 1) {
		DrawSprite(X, Y, 334);
		return;
	}
	switch (Face) {
	case LEFT:
		DrawSprite(X, Y, 337 + 3 - Phase);
		break;
	case UP:
		DrawSprite(X, Y, 339 + 3 - Phase);
		break;
	case DOWN:
		DrawSprite(X, Y, 341 + 3 - Phase);
		break;
	case RIGHT:
		DrawSprite(X, Y, 335 + 3 - Phase);
		break;
	}
}

void Pac::UpdatePhase() {
	if (GetTimeSince(PhaseTS) > .05) {
		Phase++;
		PhaseTS = GetTime();
	}
	if (Phase > 3)
		Phase = 1;

}

bool Pac::UpdateMovement() {
	if (GetTimeSince(MovementTS) > .005) {
		MovementTS = GetTime();
		return true;
	}
	return false;
}

bool Pac::CollisionCheck(dir DIR) const {
	switch (DIR) {
	case LEFT:
		if (BoardCollision[Y][X - 1] == 1 || BoardCollision[Y + 15][X - 1] == 1)
			return true;
		break;
	case RIGHT:
		if (BoardCollision[Y][X + 16] == 1 || BoardCollision[Y + 15][X + 16] == 1)
			return true;
		break;
	case UP:
		if (BoardCollision[Y - 1][X] == 1 || BoardCollision[Y - 1][X + 15] == 1)
			return true;
		break;
	case DOWN:
		if (BoardCollision[Y + 16][X] == 1 || BoardCollision[Y + 16][X + 15] == 1)
			return true;
		break;
	}
	return false;
}

int Pac::ActiveTileCheck(dir DIR) const {
	switch (DIR) {
	case LEFT:
		return BoardCollision[Y + 7][X + 7];
		break;
	case RIGHT:
		return BoardCollision[Y + 7][X + 8];
		break;
	case UP:
		return BoardCollision[Y + 7][X + 7];
		break;
	case DOWN:
		return BoardCollision[Y + 8][X + 7];
		break;
	}
	return false;
}

