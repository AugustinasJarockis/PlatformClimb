#include <windows.h>
#include <iostream>
#include <gdiplus.h>

using namespace Gdiplus;

class Platform {
public:
    Platform(int height, int width);
    ~Platform();

    void Init(HWND hwnd);
    void Redraw(Graphics &graphics);
    void Place(int x, int y);
    bool CheckCollision(int x, int y, int radius);

    bool ready = false;
    int x = 0, y = 100;
private:

    int platformHeight = 30;
    int platformWidth = 100;
    Image* sprite = nullptr;
};