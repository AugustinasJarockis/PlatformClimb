#include "Platform.h"

Platform::Platform(int height, int width)
:   platformHeight(height), platformWidth(width)
{}

Platform::~Platform()
{
    std::cout << "Platform was destroyed." << std::endl;
    delete sprite;
}

void Platform::Init(HWND hwnd)
{
    sprite = Image::FromFile(L"tex\\Platform.png");
    int spriteStatus = sprite ? sprite->GetLastStatus() : 0;
    if (!sprite || spriteStatus != 0) {
        MessageBox(hwnd, "Failed to load platfrom sprite.", "Error", MB_OK);
        std::cout << "Failed to load platfrom sprite: " << spriteStatus << std::endl;
        delete sprite;
        sprite = nullptr;
        return;
    }

    ready = true;
    std::cout << "Platform created." << std::endl;
}

void Platform::Redraw(Graphics &graphics)
{
    if (ready && sprite) {
        graphics.DrawImage(sprite, x, y, platformWidth, platformHeight);
    }
}

void Platform::Place(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Platform::CheckCollision(int x, int y, int radius)
{
    if (x + 1.5 * radius >= this->x && x + 0.5 * radius <= this->x + platformWidth
    && y + radius <= this->y && y + 2 * radius >= this->y)
        return true;
    return false;
}
