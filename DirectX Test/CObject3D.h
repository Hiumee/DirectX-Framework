#pragma once
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include "CGraphics.h"
#include <dxgi.h>
#include "point3d.h"
#include <DirectXMath.h>

#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DXGI.lib")
class CCObject3D
{
protected:
    Microsoft::WRL::ComPtr<ID3D11Device> getDevice(CGraphics& graphics);
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext(CGraphics& graphics);
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getTarget(CGraphics& graphics);
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDepthStencilView(CGraphics& graphics);
    DirectX::XMMATRIX getPerspective(CGraphics& graphics);
    DirectX::XMMATRIX getCamera(CGraphics& graphics);
};

template <class T>
class CObject3D : public CCObject3D
{
protected:
    point3d position;
    point3d size;
    point3d rotation;

protected:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> pIndicesBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    static Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    static Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
public:
    virtual void draw(CGraphics& graphics) = 0;
    virtual ~CObject3D() = default;
    void setPosition(point3d position);
    void setSize(point3d size);
    void setRotation(point3d rotation);
};

template <class T>
Microsoft::WRL::ComPtr<ID3D11Buffer> CObject3D<T>::pVertexBuffer = nullptr;
template <class T>
Microsoft::WRL::ComPtr<ID3D11PixelShader> CObject3D<T>::pPixelShader = nullptr;
template <class T>
Microsoft::WRL::ComPtr<ID3D11VertexShader> CObject3D<T>::pVertexShader = nullptr;
template <class T>
Microsoft::WRL::ComPtr<ID3D11InputLayout> CObject3D<T>::pInputLayout = nullptr;
template <class T>
Microsoft::WRL::ComPtr<ID3D11Buffer> CObject3D<T>::pIndicesBuffer = nullptr;
template <class T>
Microsoft::WRL::ComPtr<ID3D11Buffer> CObject3D<T>::pConstantBuffer = nullptr;


template<class T>
inline void CObject3D<T>::setPosition(point3d position)
{
    this->position = position;
}

template<class T>
inline void CObject3D<T>::setSize(point3d size)
{
    this->size = size;
}

template<class T>
inline void CObject3D<T>::setRotation(point3d rotation)
{
    this->rotation = rotation;
}
