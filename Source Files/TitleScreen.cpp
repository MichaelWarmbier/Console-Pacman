#include "TitleScreen.h"

void TitleScreen::Draw() {
	DrawSprite(res * 2.5, res * 2, 442);

	if (CursorSel == 1)
		DrawSprite(res * 7, res * 11, CursorLogic());
	else
		DrawSprite(res * 7, res * 11, 402);
	StringToSprite("1-Player", 9, 11, YELLOW);

	if (CursorSel == 2)
		DrawSprite(res * 9, res * 14, CursorLogic());
	else
		DrawSprite(res * 9, res * 14, 402);
	StringToSprite("Exit", 11, 14, YELLOW);

	StringToSprite("Character / Nickname", 4, 18, GRAY);
	
	DrawSprite(res * 2, res * 20, 369 + GhostFrame);
	StringToSprite("-Shadow     \"Blinky\"", 4, 20, RED);

	DrawSprite(res * 2, res * 22, 377 + GhostFrame);
	StringToSprite("-Speedy     \"Pinky\"", 4, 22, PINK);

	DrawSprite(res * 2, res * 24, 385 + GhostFrame);
	StringToSprite("-Bashful    \"Inky\"", 4, 24, BLUE);

	DrawSprite(res * 2, res * 26, 393 + GhostFrame);
	StringToSprite("-Pokey      \"Clyde\"", 4, 26, ORANGE);


}

void TitleScreen::Input() {
	if (KeyIsDown('W', true, false))
		CursorSel--;
	if (KeyIsDown('S', true, false))
		CursorSel++;
	if (KeyIsDown(13, true, false))
		switch (CursorSel) {
		case 1:
			EndMenu = true;
			break;
		case 2:
			EXIT = true;
			EndMenu = true;
			break;
		}
}

void TitleScreen::Logic() {
	if (CursorSel < 1)
		CursorSel = 2;
	if (CursorSel > 2)
		CursorSel = 1;
	if (GetTimeSince(GhostTS) > .05) {
		GhostTS = GetTime();
		GhostFrame = !GhostFrame;
	}
}

void TitleScreen::StringToSprite(const char text[50], int xpos, int ypos, COLOR color) const {
	for (int i = 0; text[i] != '\0'; i++)
		DrawSprite((xpos + i) * 16, ypos * 16, CharToSprite(tolower(text[i]), color));
}

int TitleScreen::CharToSprite(char c, COLOR color) const {
	if (c >= 97 && c <= 122)
		return (c - 96) + (41 * color);
	else if (c >= 48 && c <= 57)
		return (c - 19) + (41 * color);
	else if (c == '!')
		return 27 + (41 * color);
	else if (c == '-')
		return 40 + (41 * color);
	else if (c == '/')
		return 39 + (41 * color);
	else if (c == '"')
		return 41 + (41 * color);
	else if (c == ' ')
		return 402;
	return 0;
}

int TitleScreen::CursorLogic() {
	if (GetTimeSince(PacmanTS) > .07) {
		PacmanPhase++;
		PacmanTS = GetTime();
	}
	if (PacmanPhase > 2)
		PacmanPhase = 0;
	return 334 + PacmanPhase;
}
