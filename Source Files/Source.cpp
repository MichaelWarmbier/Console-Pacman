void ShowConsoleCursor(bool flag);
double Wait(double WaitTime);
double GetTimeSince(double StartTime);
double GetTime();
bool KeyIsDown(char key, bool pressed, bool held);
void SetConsoleSize(int x, int y);

#include "Functions.h"

int main() {
	Menu* Main = new Menu;
	SetConsoleSize(CW, CH);
	do {
		do {
			Main->Draw();
			Main->Input();
			Main->Logic();
		} while (!EXIT_MENU && (timer += (dt = FPS + Wait(FPS))));
	} while (!EXIT_PROGRAM);
	delete Main;
	return EXIT_SUCCESS;
}
