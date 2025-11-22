#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer {
public:
    void Start();
    DWORD GetDeltaTime();
private:
    DWORD intervalStartTime = 0;
};

#endif