#pragma once
#include "d3d11.h"
#include <wrl.h>
#include <DirectXMath.h>

class CGraphics
{
friend class CCObject3D;
private:
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    D3D_FEATURE_LEVEL FeatureLevel;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

    DirectX::XMMATRIX camera;
    DirectX::XMMATRIX perspective;
public:
    CGraphics(HWND hWindow);
    CGraphics operator=(CGraphics&) = delete;
    ~CGraphics();

    void setCamera(DirectX::XMMATRIX);
    DirectX::XMMATRIX getCamera();
    void endFrame();
    void clearBuffer(float red, float green, float blue);
    void draw(float alpha);
};
