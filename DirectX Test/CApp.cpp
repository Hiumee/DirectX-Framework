#include "CApp.h"
#include "CustomExceptions.h"
#include "CCube.h"
#include "point3d.h"
#include "pch.h"
#include "GDIPlusManager.h"
#include "Surface.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>


void saveMaze(int table[20][20])
{
    std::ofstream fout("maze.txt");
    for (int i = 0;i < 20;i++)
    {
        for (int j = 0;j < 20; j++)
        {
            if (table[j][i] == 1)
            {
                fout << 1;
            }
            else
            {
                fout << 0;
            }
        }
        fout << '\n';
    }
    fout.close();
}

int h(std::pair<int,int> p, std::pair<int, int> end)
{
    return abs(p.first - end.first) + abs(p.second - end.second);
};

std::pair<int, int> pathFind(int table[20][20], std::pair<int, int> start, std::pair<int, int> end)
{
    typedef struct std::pair<int, int> position;
    typedef struct std::pair<int, position> qElem;

    if (start == end)
    {
        return start;
    }

    std::priority_queue<qElem> q;
    std::map<position, position> prev;
    std::map<position, int> dist;
    
    q.push(qElem(h(start, end), start));
    dist[start] = 0;
    bool found = false;

    while (!q.empty() && !found)
    {
        qElem x = q.top();
        q.pop();
        position current = x.second;
        for (int i = -1;i < 2;i++)
        {
            for (int j = -1;j < 2;j++)
            {
                if (i == 0 && j != 0 || i != 0 && j == 0)
                {

                    if (current.first + i >= 0 && current.second + j >= 0 && current.first + i < 20 && current.second + j < 20 && table[current.first + i][current.second + j] != 1)
                    {
                        position newPoint(current.first + i, current.second + j);
                        if (dist.find(newPoint) == dist.end() || dist[current] + 1 < dist[newPoint])
                        {
                            dist[newPoint] = dist[current] + 1;
                            q.push(qElem(100-dist[newPoint] - h(newPoint, end), newPoint));
                            prev[newPoint] = current;
                        }
                    }
                }
            }
        }
        if (current == end)
        {
            found = true;
        }
    }

    if (!found)
    {
        return start;
    }

    position current = end;
    while (prev[current] != start)
    {
        current = prev[current];
    }

    return current;
}


GDIPlusManager gdipm;
CApp::CApp(HINSTANCE hInstance)
{
    this->hInstance = hInstance;
    window = std::make_unique<MyWindow>( 816, 639, "Hellow", hInstance );
    testCubes.push_back(Cube(window->getGraphics()));
    testCubes.push_back(Cube(window->getGraphics()));
    testCubes[0].setRotation(point3d{0.0f,0.8f,0.8f});
    testCubes[1].setPosition(point3d{ 0.0f,0.0f,0.0f });
    s = std::make_unique<Surface>(Surface::FromFile("wall.png"));
    auto pathSurfaceT = Surface::FromFile("path.png");
    auto wallSurfaceT = Surface::FromFile("blue.png");
    pTestTexture = std::make_unique<TCube>(window->getGraphics(), wallSurfaceT);
    pTestTexture->setPosition(point3d{ 0.0f,0.0f,0.0f });
    pTestTexture->setSize(point3d{ 0.5f,0.5f,0.5f});
    auto su = Surface::FromFile("macaroana.png");
    pTestTexture2 = std::make_unique<TCube>(window->getGraphics(), su);
    pTestTexture2->setPosition(point3d{ 0.0f,0.0f,0.0f });
    pTestTexture2->setSize(point3d{ 0.3f,0.3f,0.3f });
    camera.setCamera(0.1f, 0.1f, -5.0f, 0, 0, 0);
    window->getGraphics().setCamera(camera.getCamera());
    su = Surface::FromFile("ground.jpg");
    pGround = std::make_unique<TCube>(window->getGraphics(), su);
    pGround->setPosition(point3d{0.0f,-0.05f,0.0f});
    pGround->setSize(point3d{ 10.f,0.1f,10.0f });

    
    pathSurface = std::make_unique<TSurface>(window->getGraphics(), pathSurfaceT);
    pathSurface->setRotation(point3d{ 0.25f,0.0f,0.0f });
    pathSurface->setSize(point3d{ 0.5f,0.5f,0.5f });

    clock.reset();
    mouseX = 400;
    mouseY = 300;
    
    finder = std::make_unique<Cube>(window->getGraphics());
    finder->setSize(point3d{0.15f,0.25f,0.15f});
    finderX = 0;
    finderY = 0;

    tSurface = std::make_unique<TSurface>(window->getGraphics(), *s);
    tSurface->setRotation(point3d{ 0.25f,0.0f,0.0f });
    tSurface->setSize(point3d{ 0.5f,0.5f,0.5f });


    std::ifstream fin("maze.txt");
    std::string line;
    int l = 0;
    while (fin >> line)
    {
        for (int i = 0;i < 20;i++)
        {
            gameTable[i][l] = line[i] - '0';
        }
        l++;
    }

    findX = 0.0f;
    findY = 0.0f;

    //for (int i = 0;i < 200;i++)
    //{
    //    for (int j = 0;j < 200;j++)
    //    {
    //        gameTable[i][j] = 0;
    //    }
    //}
    //gameTable[5][5] = 1;
    //gameTable[5][4] = 1;
    //gameTable[5][3] = 1;
    //gameTable[4][3] = 1;
    //gameTable[3][3] = 1;
}

CApp::~CApp()
{
}

void CApp::tick()
{
    auto eTime = clock.timeElapsed();
    clock.reset();
    std::pair<int, int> pos = window->getMouse().getPosition();
    
    //if (window->getMouse().leftClickPressed())
    //{
    //    testCubes[0].setRotation(point3d{ pos.second / 600.0f, -pos.first / 800.0f,0.0f });
    //    testCubes[1].setRotation(point3d{ -pos.second / 600.0f, pos.first / 800.0f,0.0f });
    //    pTestTexture->setRotation(point3d{ -pos.second / 300.0f + 1.0f, pos.first / 400.0f,0.0f-1.0f });
    //}
    //if (window->getMouse().rightClickPressed())
    //{
    //    testCubes[0].setPosition(point3d{ (pos.first / 400.0f - 1.0f) * 5.0f,0.0f,(pos.second / 300.f - 1.0f) *2.0f });
    //}


    ////
    const float speed = 10.0f;
    if (window->getKeyboard().isDown('W'))
    {
        camera.x -= eTime * speed * sinf(-camera.pitch);
        camera.z += eTime * speed * cosf(-camera.pitch);
    }
    if (window->getKeyboard().isDown('S'))
    {
        camera.x += eTime * speed * sinf(-camera.pitch);
        camera.z -= eTime * speed * cosf(-camera.pitch);
    }
    if (window->getKeyboard().isDown('D'))
    {
        camera.x += eTime * speed * cosf(camera.pitch);
        camera.z -= eTime * speed * sinf(camera.pitch);
    }
    if (window->getKeyboard().isDown('A'))
    {
        camera.x -= eTime * speed * cosf(camera.pitch);
        camera.z += eTime * speed * sinf(camera.pitch);
    }
    if (window->getKeyboard().isDown(VK_SPACE))
    {
        camera.y += eTime * speed;
    }
    if (window->getKeyboard().isDown(VK_SHIFT))
    {
        camera.y -= eTime * speed;
    }

    // Look at function
    //DirectX::XMVECTOR cameraVector = DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), camera.getCamera());

    DirectX::XMVECTOR cameraVector = DirectX::XMVectorSet(
        -cosf(camera.pitch) * cosf(camera.yaw),
        sinf(camera.yaw),
        -sinf(camera.pitch) * cosf(camera.yaw),
        0.0f
    );

    DirectX::XMVECTOR camP = DirectX::XMVectorSet(camera.z, camera.y, camera.x, 0.0f);
    DirectX::XMVECTOR cam = DirectX::XMPlaneIntersectLine(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), camP,
        DirectX::XMVectorAdd(camP, cameraVector));
    DirectX::XMVECTOR camUP = DirectX::XMPlaneIntersectLine(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, -1.0f), camP,
        DirectX::XMVectorAdd(camP, cameraVector));
    int cameraX = static_cast<int>(floor(DirectX::XMVectorGetZ(cam) + 0.5f));
    int cameraY = static_cast<int>(floor(DirectX::XMVectorGetX(cam) + 0.5f));

    int cameraXUP = static_cast<int>(floor(DirectX::XMVectorGetZ(camUP) + 0.5f));
    int cameraYUP = static_cast<int>(floor(DirectX::XMVectorGetX(camUP) + 0.5f));
    // End look at function
    if (cameraXUP >= 0 && cameraXUP < 20 && cameraYUP >= 0 && cameraYUP < 20 && gameTable[cameraXUP][cameraYUP] == 1)
    {
        cameraX = cameraXUP;
        cameraY = cameraYUP;
    }

    static bool spaceDown = false;
    static int brush = 0;
    if (window->getKeyboard().isDown(VK_CONTROL))
    {
        if (cameraX >= 0 && cameraX < 20 && cameraY >= 0 && cameraY < 20)
        {
            if (spaceDown == false)
            {
                brush = !gameTable[cameraX][cameraY];
                spaceDown = true;
            }
            gameTable[cameraX][cameraY] = brush;
        }
    }
    if (window->getKeyboard().isUp(VK_CONTROL))
    {
        saveMaze(gameTable);
        spaceDown = false;
    }

    const float mouseSensitivity = 0.01f;

    if (window->getMouse().leftClickPressed())
    {
        if (mouseX != -1)
        {
        camera.pitch += (window->getMouse().getX() - mouseX) * mouseSensitivity;

        camera.yaw += (window->getMouse().getY() - mouseY) * mouseSensitivity;
        if (camera.yaw > DirectX::XM_PI / 2.0f)
        {
            camera.yaw = DirectX::XM_PI / 2.0f;
        }
        if (camera.yaw < -DirectX::XM_PI / 2.0f)
        {
            camera.yaw = -DirectX::XM_PI / 2.0f;
        }
        }
        mouseY = window->getMouse().getY();
        mouseX = window->getMouse().getX();
    }
    else
    {
        mouseX = -1;
    }

    ////
    
    static std::string kbdString = "";
    //if(window->getKeyboard().isChar())
    //{
    //    char c = window->getKeyboard().getChar();
    //    if (c != VK_BACK)
    //    {
    //        kbdString += c;
    //    }
    //    else
    //    {
    //        if (kbdString.size())
    //        {
    //            kbdString.pop_back();
    //        }
    //    }
    //}
    static float red = 0;
    const float changePerSecond = 3.0;

    static float titleTime = 0.0f;
    titleTime += eTime;

    if (titleTime > 1.0f)
    {
    titleTime = 0.0f;
    std::string result = "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ") " + kbdString + " " + std::to_string(1/eTime) + " fps";

    window->setTitle(result);
    }
    if (window->getMouse().leftClickPressed())
    {
        red += eTime * changePerSecond;
    }

    const static float finderSpeed = 1.0f;
    const static float epsilon = 0.1f;

    std::pair<int, int> spaceToGo = pathFind(gameTable, std::pair<int, int>(finderX, finderY), std::pair<int, int>(19, 19));
    std::pair<float, float> unitVector{static_cast<float>(spaceToGo.first-finderX), static_cast<float>(spaceToGo.second-finderY)};
    findX += finderSpeed * unitVector.first * eTime;
    findY += finderSpeed * unitVector.second * eTime;

    if (findX > static_cast<float>(spaceToGo.first) - epsilon && findX < static_cast<float>(spaceToGo.first))
    {
        findX = static_cast<float>(spaceToGo.first);
        finderX = spaceToGo.first;
    }
    if (findY > static_cast<float>(spaceToGo.second) - epsilon && findY < static_cast<float>(spaceToGo.second))
    {
        findY = static_cast<float>(spaceToGo.second);
        finderY = spaceToGo.second;
    }

    if (finderX == spaceToGo.first && finderY == spaceToGo.second)
    {
        gameTable[finderX][finderY] = 2;
    }

    window->getGraphics().setCamera(camera.getCamera());
    // Render
    window->getGraphics().clearBuffer((sinf(red)+1.0f)/2.0f, abs(pos.first)/800.0f, abs(pos.second)/600.0f);
    //window->getGraphics().clearBuffer(0, 0, 0);
    //
    //testCubes[0].draw(window->getGraphics());
    //testCubes[1].draw(window->getGraphics());
    //pTestTexture->draw(window->getGraphics());
    //pTestTexture2->draw(window->getGraphics());
    TSurface::UPDATE = 1;
    for (int i = 0;i < 20;i++)
    {
        for (int j = 0;j < 20;j++)
        {
            if (gameTable[i][j] != 1 && !(i == cameraX && j == cameraY))
            {
                tSurface->setPosition(point3d{ (float)i, 0.0f, (float)j });
                tSurface->draw(window->getGraphics());
            }
        }
    }
    TSurface::UPDATE = 1;
    TCube::UPDATE = 1;
    for (int i = 0;i < 20;i++)
    {
        for (int j = 0;j < 20;j++)
        {
            if (gameTable[i][j] == 1)
            {
                pTestTexture->setPosition(point3d{ (float)i, 0.5f, (float)j });
                pTestTexture->draw(window->getGraphics());
            }
        }
    }
    TCube::UPDATE = 1;
    if (cameraX >= 0 && cameraX < 20 && cameraY >= 0 && cameraY < 20)
    {
        TSurface::UPDATE = 1;
        if (gameTable[cameraX][cameraY] == 1)
        {
            pathSurface->setPosition(point3d{ (float)cameraX, 1.01f, (float)cameraY });
        }
        else
        {
            pathSurface->setPosition(point3d{ (float)cameraX, 0.0f, (float)cameraY });
        }
        pathSurface->draw(window->getGraphics());
        TSurface::UPDATE = 0;
    }
    //TSurface::UPDATE = 1;
    //for (int i = 0;i < 20;i++)
    //{
    //    for (int j = 0;j < 20;j++)
    //    {
    //        if (gameTable[i][j] == 2)
    //        {
    //            pathSurface->setPosition(point3d{ (float)i, 0.0f, (float)j });
    //            pathSurface->draw(window->getGraphics());
    //        }
    //    }
    //}
    //TSurface::UPDATE = 1;

    finder->setPosition(point3d{ findX,0.25f,findY});
    finder->draw(window->getGraphics());
    //pGround->draw(window->getGraphics());
    //window->getGraphics().draw(red);
    //
    window->getGraphics().endFrame();

}
