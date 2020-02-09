#pragma once
#include "Header.h"
class Pac {
private:

	int Phase; // Current display phase
	double PhaseTS; // Timestamp for display phase

public:

	int BoardCollision[BH * 16][BW * 16]; // Board for collision
	double MovementTS; // Timestamp for movement

	int X; // X position
	int Y; // Y position
	dir Face; // Facing direction

	Pac() : Face(LEFT),Phase(1),PhaseTS(GetTime()),X(216),Y(416),MovementTS(GetTime()) {}; // Constructor

	void UpdatePhase(); // Updates display face accordingly
	bool CollisionCheck(dir DIR) const; // Checks for adjacent collision based on face
	int ActiveTileCheck(dir DIR) const; // Checks for adjacent unique tiles based on face
	bool UpdateMovement(); // Updates movement accordingly
	void DrawPlayer() const; // Draws player based around current phase and position
};

