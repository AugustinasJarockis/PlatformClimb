#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <gdiplus.h>
#include <algorithm>

#include "Timer.h"
#include "Definitions.h"

using namespace Gdiplus;

class Character {
public:
    Character();
    ~Character();

    void Init(HWND hwnd);
    void Reset();
    void Redraw(Graphics &graphics, HWND hwnd);
    void NotifyAboutCollision();

    bool ready = false;

    int x = WINDOW_WIDTH / 2 - 25, y = 100;
    int charSize = 50;
    double speedY = 0;
private:
    void UpdatePosition(HWND hwnd);
    void UpdateSpeed(HWND hwnd);

    Timer timer;

    const double gravity = 20;
    const double jumpStrength = 18;
    double speedX = 0;

    Image* sprite = nullptr;
};
