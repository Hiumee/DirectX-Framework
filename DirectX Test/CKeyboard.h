#pragma once
#include <bitset>
#include <queue>

class CKeyboard
{
friend class MyWindow;
private:
    std::bitset<256> keys;
    std::queue<char> charBuffer;
public:
    bool isDown(int keyCode) const;
    bool isUp(int keyCode) const;
    bool keyState(int keyCode) const;
    char getChar();
    bool isChar();

private:
    void keyDown(int keyCode);
    void keyUp(int keyCode);
    void charPressed(int keyCode);
    void clearCharBuffer();
    void releaseKeys();
};
