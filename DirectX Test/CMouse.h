#pragma once
#include <utility>

class CMouse
{
friend class MyWindow;
private:
    bool leftClick;
    bool rightClick;
    bool middleClick;
    int mouseX;
    int mouseY;
    int scrollWheel;

public:
    CMouse();

    int getX() const;
    int getY() const;
    std::pair<int, int> getPosition() const;
    bool leftClickPressed() const;
    bool rightClickPressed() const;
    bool middleClickPressed() const;
    int getScroll() const;

private:
    void setLocation(int x, int y);
    void setLeftClick(bool state);
    void setMiddleClick(bool state);
    void setRightClick(bool state);
    void setScroll(int delta);
};