#pragma once
#include "Header.h"
class Main {
private:

	bool EndProgram; // Bool to determine if program should end

public:
	Main() :EndProgram(false) { }; // Constructor

	bool ProgramStatus() const { return EndProgram; }; // Rerturn program status
	void ExitProgram() { EndProgram = true; }; // Exit program

	int ReadFile() {
		int hsdata = 0;
		ifstream hs;
		hs.open("score.hs");
		hs >> hsdata;
		return hsdata;
	}
	void WriteFile(int data) { // Wites highscore data to file
		ofstream hs;
		hs.open("score.hs", ofstream::out | ofstream::trunc);
		hs << data;
		hs.close();
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
	} // Sets window size once

};

