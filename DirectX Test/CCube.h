#pragma once
#include "CObject3D.h"
#include <dxgi.h>
#include <D3DCompiler.h>
#include <DirectXMath.h>


class Cube : public CObject3D<Cube>
{
private:
    struct CONSTANT_BUFFER
    {
        DirectX::XMMATRIX matrix;
    };

    CONSTANT_BUFFER constantBuffer;

    struct Vertex {
        float x, y, z;
    };

    inline const static Vertex vertices[8] = {
        { -1, -1, -1 },
        { 1, -1, -1 },
        { 1, 1, -1 },
        { -1, 1, -1 },
        { -1, -1, 1 },
        { 1, -1, 1 },
        { 1, 1, 1 },
        { -1, 1, 1 } };

    inline const static short indices[36] =
    {
        0,2,1, 0,3,2,
        1,2,6, 1,6,5,
        0,1,5, 0,5,4,
        3,0,4, 3,4,7,
        3,7,6, 3,6,2,
        6,7,4, 6,4,5
    };
public:
    Cube() {};
    Cube(CGraphics& graphics);
    void draw(CGraphics& graphics);
};