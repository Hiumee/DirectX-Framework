#pragma once
#include "Window.h"
#include "CTime.h"
#include <memory>
#include "CCube.h"
#include "TCube.h"
#include "Surface.h"
#include "CCamera.h"
#include "TSurface.h"

#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DXGI.lib")
#ifdef _DEBUG
#pragma comment(lib,"MSVCRTD.lib")
#else
#pragma comment(lib,"MSVCRT.lib")
#endif

class CApp
{
private:

    int gameTable[20][20];
    std::unique_ptr<TSurface> tSurface;
    std::unique_ptr<TSurface> pathSurface;
    std::unique_ptr<TSurface> wallSurface;

    std::unique_ptr<MyWindow> window;
    CTime clock;
    HINSTANCE hInstance;
    std::vector<Cube> testCubes;
    std::unique_ptr<TCube> pTestTexture;
    std::unique_ptr<TCube> pTestTexture2;
    std::unique_ptr<TCube> pGround;
    std::unique_ptr<Surface> s;
    std::unique_ptr<Cube> finder;
    CCamera camera;
    int mouseX, mouseY;
    int mouseXUP, mouseYUP;

    int finderX, finderY;
    float findX, findY;
public:
    CApp(HINSTANCE hInstance);
    CApp operator=(CApp&) = delete;
    ~CApp();
    void tick();
};