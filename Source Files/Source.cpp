void ShowConsoleCursor(bool flag);
double Wait(double WaitTime);
double GetTimeSince(double StartTime);
double GetTime();
bool KeyIsDown(char key, bool pressed, bool held);
void SetConsoleSize(int x, int y);

#include "Functions.h"

int main() {
	SetConsoleSize(CW, CH);
	Menu* Main = new Menu;
	while (1) {
		Main->Draw();
		Main->Input();
		Main->Logic();
	}
	delete Main;
	return EXIT_SUCCESS;
}
