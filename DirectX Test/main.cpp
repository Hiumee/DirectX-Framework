//#include "pch.h"
#include "CApp.h"
#include <Windows.h>
#include "testmacro.h"
#include <sstream>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow )
{
    char errorMessage[] = "Initialization error";
    char errorMessage2[] = "Runtime error";
    try
    {
        CApp myApp{ hInstance };
        MSG message;
        message.message = !WM_QUIT;

        while (WM_QUIT != message.message)
        {
            while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                //gResult = (GetMessage(&message, nullptr, 0, 0)) > 0
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            try
            {
            myApp.tick();
            }
            catch (...)
            {
                MessageBox(nullptr, errorMessage2, errorMessage2, MB_OK);
            }
        }

        return message.wParam;
    }
    catch (__exception& e)
    {
        std::string em(e.what());
        std::stringstream stream;
        stream << std::hex << e.result();
        em += "\nHR: 0x" + std::string(stream.str());
        MessageBox(nullptr, em.c_str(), errorMessage, MB_OK);
    }
    catch (std::exception& e)
    {
        MessageBox(nullptr, e.what(), errorMessage, MB_OK);
    }
    return -1;
}
