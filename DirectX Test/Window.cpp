#include "Window.h"
#include <Windowsx.h>
#include "CustomExceptions.h"

MyWindow::MyWindow(int length, int height, LPCSTR title, HINSTANCE hInstance)
{
    const auto pClassName = title;
    // Register window 
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = this->StartupWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(this);
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;
    if (RegisterClassEx(&wc) == 0)
    {
        throw WindowException();
    }
    // Create window instance
    this->hWindow = CreateWindowEx(
        0, pClassName, title,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
        200, 200, length, height,
        nullptr, nullptr,
        hInstance,
        this
    );
    if (this->hWindow == nullptr)
    {
        throw WindowException();
    }

    this->graphics = std::make_unique<CGraphics>(hWindow);
}

MyWindow::~MyWindow()
{
    DestroyWindow(this->hWindow);
}

void MyWindow::setTitle(std::string newTitle)
{
    SetWindowText(this->hWindow, newTitle.c_str());
}

CKeyboard& MyWindow::getKeyboard()
{
    return keyboard;
}

CMouse& MyWindow::getMouse()
{
    return mouse;
}

CGraphics& MyWindow::getGraphics()
{
    return *graphics;
}

LRESULT WINAPI MyWindow::StartupWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        CREATESTRUCT* cStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        MyWindow* window = reinterpret_cast<MyWindow*>(cStruct->lpCreateParams);
        SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONGLONG>(window));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONGLONG>(RedirectingWindowProc));
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI MyWindow::RedirectingWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MyWindow* window = reinterpret_cast<MyWindow*>(GetWindowLongPtr(hWnd, 0));
    return window->RealWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT MyWindow::RealWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    /********* START KEYBOARD MESSAGES ***********/
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        keyboard.keyDown(wParam);
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        keyboard.keyUp(wParam);
        break;
    case WM_CHAR:
        keyboard.charPressed(wParam);
        break;
    /********* END KEYBOARD MESSAGES *************/
    /********* START MOUSE MESSAGES **************/
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        mouse.setLocation(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_MOUSEWHEEL:
        mouse.setScroll(GET_WHEEL_DELTA_WPARAM(wParam));
        mouse.setLeftClick(GET_KEYSTATE_WPARAM(wParam) & MK_LBUTTON);
        mouse.setMiddleClick(GET_KEYSTATE_WPARAM(wParam) & MK_MBUTTON);
        mouse.setRightClick(GET_KEYSTATE_WPARAM(wParam) & MK_RBUTTON);
        break;
    /********* END MOUSE MESSAGES ****************/

    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
