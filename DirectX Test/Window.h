#pragma once
#include <Windows.h>
#include <string>
#include "CKeyboard.h"
#include "CMouse.h"
#include "CGraphics.h"
#include "d3d11.h"
#include <memory>

class MyWindow
{
private:
    HWND hWindow;
    CKeyboard keyboard;
    CMouse mouse;
    std::unique_ptr<CGraphics> graphics;

public:
    MyWindow(int length, int height, LPCSTR title, HINSTANCE hInstance);
    MyWindow operator=(MyWindow&) = delete; // no copy constructor
    ~MyWindow();
    void setTitle(std::string newTitle);
    CKeyboard& getKeyboard();
    CMouse& getMouse();
    CGraphics& getGraphics();

private:
    static LRESULT WINAPI StartupWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WINAPI RedirectingWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT RealWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};