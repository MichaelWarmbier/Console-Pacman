#pragma once
#include "Header.h"
class Pac {
private:
	int Phase;
	double PhaseTS;
public:
	int BoardCollision[BH * 16][BW * 16];
	double MovementTS;
	int X;
	int Y;
	dir Face;
	Pac() : Face(NONE),Phase(1),PhaseTS(GetTime()),X(216),Y(416),MovementTS(GetTime()) {};
	void UpdatePhase();
	int CollisionCheck(dir DIR) const;
	bool UpdateMovement();
	void DrawPlayer() const;
};

