#pragma once
#include "CObject3D.h"
#include <dxgi.h>
#include <D3DCompiler.h>
#include <DirectXMath.h>
#include <string>
#include "Surface.h"

#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DXGI.lib")

class TSurface : public CObject3D<TSurface>
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
        {-1, -1, 0, 1, 1},
        {-1, 1, 0, 1, 0},
        {1, 1, 0, 0, 0},
        {1, -1, 0, 0, 1}
    };

    inline const static short indices[] =
    {
        0,1,2,
        0,2,3
    };
public:
    static int UPDATE;
    TSurface() {};
    TSurface(CGraphics& graphics, Surface& s);
    void draw(CGraphics& graphics);
};