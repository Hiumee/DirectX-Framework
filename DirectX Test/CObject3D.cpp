#include "CObject3D.h"

Microsoft::WRL::ComPtr<ID3D11Device> CCObject3D::getDevice(CGraphics& graphics)
{
    return graphics.pDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> CCObject3D::getContext(CGraphics& graphics)
{
    return graphics.pContext;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CCObject3D::getTarget(CGraphics& graphics)
{
    return graphics.pTarget;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CCObject3D::getDepthStencilView(CGraphics& graphics)
{
    return graphics.pDSV;
}

DirectX::XMMATRIX CCObject3D::getPerspective(CGraphics& graphics)
{
    return graphics.perspective;
}

DirectX::XMMATRIX CCObject3D::getCamera(CGraphics& graphics)
{
    return graphics.camera;
}
