#include "Timer.h"

void Timer::Start()
{
    intervalStartTime = GetTickCount();
}

DWORD Timer::GetDeltaTime()
{
    DWORD currentTime = GetTickCount();
    DWORD delta = currentTime - intervalStartTime;
    intervalStartTime = currentTime;
    return delta;
}
