#pragma once
#include "Header.h"
class Main {
private:
	bool EndProgram;
public:
	Main() :EndProgram(false) { };
	bool ProgramStatus() const { return EndProgram; };
	void ExitProgram() { EndProgram = true; };
	void SpriteTest() {
		for (int i = 0; i < 500; i++) {
			DrawSprite(5 * res, 5 * res, i);
			Wait(.08);
		}
	}
	void SetWindowDimensions(int x, int y) {
		HWND console = GetConsoleWindow();
		HMONITOR monitor = MonitorFromWindow(console, MONITOR_DEFAULTTOPRIMARY);

		MONITORINFOEX miex;
		miex.cbSize = sizeof(miex);
		GetMonitorInfo(monitor, &miex);
		int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
		int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

		DEVMODE dm;
		dm.dmSize = sizeof(dm);
		dm.dmDriverExtra = 0;
		EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
		int cxPhysical = dm.dmPelsWidth;
		int cyPhysical = dm.dmPelsHeight;

		double horzScale = ((double)cxPhysical / (double)cxLogical);
		double vertScale = ((double)cyPhysical / (double)cyLogical);
		SetWindowPos(console, HWND_TOP, 0, 0, x * 16.0 / horzScale, y * 16.0 / vertScale, SWP_NOMOVE);
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(handle, &info);
		COORD new_size = {
			info.srWindow.Right - info.srWindow.Left + 1,
			info.srWindow.Bottom - info.srWindow.Top + 1
		};
		SetConsoleScreenBufferSize(handle, new_size);
		ShowConsoleCursor(false);
	}

};

