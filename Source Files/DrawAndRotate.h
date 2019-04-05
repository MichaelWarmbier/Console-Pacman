#ifndef  DRAWANDROTATE_H
#define DRAWANDROTATE_H

#include <Windows.h>

class DrawAndRotate {
public:

	HDC hdc;

	COLORREF yellow;
	COLORREF blue;
	COLORREF black;

	HPEN outlinePen;
	HPEN blueOutlinePen;
	HBRUSH blackBrush;
	HBRUSH yellowBrush;
	
	enum keyboardInput { UP, DOWN, LEFT, RIGHT, START, NONE };
	keyboardInput playerInput = NONE, lastPlayerInput = NONE;

	int playerX = 60, playerY = 60;

	// Constructor
	DrawAndRotate() {
		hdc = GetDC(GetConsoleWindow());

		yellow = RGB(255, 255, 0);
		blue = RGB(0, 0, 255);
		black = RGB(12, 12, 12);

		outlinePen = CreatePen(PS_NULL, 0, black);
		blueOutlinePen = CreatePen(PS_SOLID, 1, blue);
		blackBrush = CreateSolidBrush(black);
		yellowBrush = CreateSolidBrush(yellow);
	}

	// Destructor (NO MORE WORRIES ABOUT MEMORY LEAKS... RIGHT?)
	~DrawAndRotate() {
		ReleaseDC(GetConsoleWindow(), hdc);
		DeleteObject(outlinePen);
		DeleteObject(blueOutlinePen);
		DeleteObject(blackBrush);
		DeleteObject(yellowBrush);
	}

	template<typename T>
	void rotate(T sprite[16][16], int deg) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				switch (deg) {
				case 0:
					if (sprite[y][x] == 1)
						SetPixelV(hdc, playerX + x, playerY + y, yellow);
					break;
				case 90:
					if (sprite[y][x] == 1)
						SetPixelV(hdc, playerX + y, playerY + x, yellow);
					break;
				case 180:
					if (sprite[y][x] == 1) {
						if (x < 8)
							SetPixelV(hdc, playerX + x + ((8 - x) * 2), playerY + y, yellow);
						if (x >= 8)
							SetPixelV(hdc, playerX + x - ((x - 8) * 2), playerY + y, yellow);
					}
					break;
				case 270:
					if (sprite[y][x] == 1) {
						if (x < 8)
							SetPixelV(hdc, playerX + y + ((8 - y) * 2), playerY + x + ((8 - x) * 2), yellow);
						if (x >= 8)
							SetPixelV(hdc, playerX + y - ((y - 8) * 2), playerY + x - ((x - 8) * 2), yellow);
					}
					break;
				}
			}
		}
	}

	template<typename T>
	void drawEntity(T sprite[16][16]) {
		switch (lastPlayerInput) {
		case RIGHT:
		case NONE:
			rotate(sprite, 0);
			break;
		case DOWN:
			rotate(sprite, 90);
			break;
		case LEFT:
			rotate(sprite, 180);
			break;
		case UP:
			rotate(sprite, 270);
			break;
		}
	}

	void drawClearEntity(keyboardInput lastPlayerInput) {
		int xOffset = 0, yOffset = 0;
		if (lastPlayerInput == LEFT)
			xOffset = 2;
		else if (lastPlayerInput == RIGHT)
			xOffset = -2;
		else if (lastPlayerInput == UP)
			yOffset = 2;
		else if (lastPlayerInput == DOWN)
			yOffset = -2;

		Rectangle(hdc, playerX + xOffset, playerY + yOffset, playerX + xOffset + 17, playerY + yOffset + 17);
	}
};

#endif
