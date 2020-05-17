#pragma once
#include "CObject3D.h"
#include <dxgi.h>
#include <D3DCompiler.h>
#include <DirectXMath.h>
#include <string>
#include "Surface.h"

#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DXGI.lib")

class TCube : public CObject3D<TCube>
{
private:
    struct CONSTANT_BUFFER
    {
        DirectX::XMMATRIX matrix;
    };

    CONSTANT_BUFFER constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;

    struct Vertex {
        float x, y, z;
        float u, v;
    };

    inline const static Vertex vertices[] = {
        {-1,-1,-1,0,1},
        {1,-1,-1,1,1},
        {1,1,-1,1,0},
        {-1,1,-1,0,0},
        // 1/6
        {1,-1,-1,0,1},
        {1,-1,1,1,1},
        {1,1,1,1,0},
        {1,1,-1,0,0},
        // 2/6
        {1,-1,1,0,1},
        {-1,-1,1,1,1},
        {-1,1,1,1,0},
        {1,1,1,0,0},
        // 3/6
        {-1,-1,1,0,1},
        {-1,-1,-1,1,1},
        {-1,1,-1,1,0},
        {-1,1,1,0,0},
        // 4/6
        {-1,1,-1,0,1},
        {1,1,-1,1,1},
        {1,1,1,1,0},
        {-1,1,1,0,0},
        // 5/6
        {-1,-1,-1,0,1},
        {-1,-1,1,1,1},
        {1,-1,1,1,0},
        {1,-1,-1,0,0}
        // 6/6
    };

    inline const static short indices[] =
    {
        0,2,1, 0,3,2,
        4,6,5, 4,7,6,
        8,10,9, 8,11,10,
        12,14,13, 12,15,14,
        16,18,17, 16,19,18,
        20,22,21, 20,23,22
    };
public:
    static int UPDATE;
    TCube() {};
    TCube(CGraphics& graphics, Surface& s);
    void draw(CGraphics& graphics);
};