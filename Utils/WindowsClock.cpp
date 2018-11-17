#include "WindowsClock.h"

WindowsClock* WindowsClock::GetInstance()
{
	static WindowsClock instance;
	return &instance;
}
