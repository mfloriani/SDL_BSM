#pragma once
#pragma comment(lib, "winmm.lib")
#include <windows.h>

#define WinClock WindowsClock::GetInstance()

class WindowsClock
{
private:
	double m_startTime;

	WindowsClock(){ m_startTime = timeGetTime() * 0.001; }
	WindowsClock(const WindowsClock&);
	WindowsClock& operator=(const WindowsClock&);

public:
	static WindowsClock* GetInstance();
	double GetCurTime(){ return timeGetTime() * 0.001 - m_startTime; }
};

