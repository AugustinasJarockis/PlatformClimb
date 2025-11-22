#include "Character.h"

Character::Character(){}

Character::~Character()
{
    std::cout << "Character was destroyed." << std::endl;
    delete sprite;
}

void Character::Init(HWND hwnd)
{
    sprite = Image::FromFile(L"tex\\Ball.png");
    int spriteStatus = sprite ? sprite->GetLastStatus() : 0;
    if (!sprite || spriteStatus != 0) {
        MessageBox(hwnd, "Failed to load sprite.", "Error", MB_OK);
        std::cout << "Failed to load sprite: " << spriteStatus << std::endl;
        delete sprite;
        sprite = nullptr;
        return;
    }

    ready = true;
    timer.Start();

    std::cout << "Character created." << std::endl;
}

void Character::Reset()
{
    x = WINDOW_WIDTH / 2 - 25;
    y = 100;
    speedX = 0;
    speedY = 0;

    timer.Start();
}

void Character::Redraw(Graphics &graphics, HWND hwnd)
{
    if (ready && sprite) {
        UpdatePosition(hwnd);
        graphics.DrawImage(sprite, x, y, charSize, charSize);
    }
}

void Character::NotifyAboutCollision()
{
    speedY = -jumpStrength;
}

void Character::UpdatePosition(HWND hwnd)
{
    UpdateSpeed(hwnd);
    x += speedX;
    y += speedY;
}

void Character::UpdateSpeed(HWND hwnd)
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hwnd, &point);

    double distanceDiff = std::max(std::min((int)point.x - (x + charSize / 2), 100), -100) / 75.0;
    speedX = speedX * 0.9 + distanceDiff * abs(distanceDiff);
    speedY += gravity * (timer.GetDeltaTime() / 1000.0);
}
