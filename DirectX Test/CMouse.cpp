#include "CMouse.h"

CMouse::CMouse()
{
    leftClick = false;
    rightClick = false;
    middleClick = false;
    mouseX = 0;
    mouseY = 0;
    scrollWheel = 0;
}

int CMouse::getX() const
{
    return mouseX;
}

int CMouse::getY() const
{
    return mouseY;
}

std::pair<int, int> CMouse::getPosition() const
{
    return std::pair<int, int>(mouseX, mouseY);
}

bool CMouse::leftClickPressed() const
{
    return leftClick;
}

bool CMouse::rightClickPressed() const
{
    return rightClick;
}

bool CMouse::middleClickPressed() const
{
    return middleClick;
}

int CMouse::getScroll() const
{
    return scrollWheel;
}

void CMouse::setLocation(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

void CMouse::setLeftClick(bool state)
{
    leftClick = state;
}

void CMouse::setMiddleClick(bool state)
{
    middleClick = state;
}

void CMouse::setRightClick(bool state)
{
    rightClick = state;
}

void CMouse::setScroll(int delta)
{
    scrollWheel = delta;
}
