#include "CKeyboard.h"
#include "CustomExceptions.h"
#define CHAR_BUFFER_SIZE 16

bool CKeyboard::isDown(int keyCode) const
{
    return keys[keyCode];
}

bool CKeyboard::isUp(int keyCode) const
{
    return !keys[keyCode];
}

bool CKeyboard::keyState(int keyCode) const
{
    return keys[keyCode];
}

char CKeyboard::getChar()
{
    if (charBuffer.size() == 0)
    {
        throw KeyboardException();
    }
    char c = charBuffer.back();
    charBuffer.pop();
    return c;
}

bool CKeyboard::isChar()
{
    return charBuffer.size();
}

void CKeyboard::keyDown(int keyCode)
{
    keys.set(keyCode);
}

void CKeyboard::keyUp(int keyCode)
{
    keys.reset(keyCode);
}

void CKeyboard::charPressed(int keyCode)
{
    char chr = static_cast<char>(keyCode);
    charBuffer.push(chr);
    while (charBuffer.size() > CHAR_BUFFER_SIZE)
    {
        charBuffer.pop();
    }
}

void CKeyboard::clearCharBuffer()
{
    charBuffer = std::queue<char>();
}

void CKeyboard::releaseKeys()
{
    keys.reset();
}
