#include "CGraphics.h"
#include "CustomExceptions.h"
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <dxgi.h>
#include "testmacro.h"

#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"D3D11.lib")

using namespace Microsoft::WRL;

CGraphics::CGraphics(HWND hWindow)
{    
     //
    perspective = DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 20.0f);
     //
     HRESULT hr;
    /**************** Creating D3D11 Device *************/
     
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
    SwapChainDesc.BufferDesc.Width = 0;
    SwapChainDesc.BufferDesc.Height = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // holy shit
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 2;
    SwapChainDesc.OutputWindow = hWindow;
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc.Flags = 0;

    // TODO Check
    ComPtr<IDXGIFactory> pFactory = nullptr;
    TEST_FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), &pFactory));

    ComPtr<IDXGIAdapter> pAdapter;
    TEST_FAILED(pFactory->EnumAdapters(0, &pAdapter));

#ifdef _DEBUG
    TEST_FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &SwapChainDesc,
        &pSwapChain,
        &pDevice,
        &FeatureLevel,
        &pContext
    ));
#else
    TEST_FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &SwapChainDesc,
        &pSwapChain,
        &pDevice,
        &FeatureLevel,
        &pContext
    ));
#endif

    /*HRESULT READ_THIS = pDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_B8G8R8A8_UNORM,
        4,
        &pNumQualityLevels
    );*/

    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
    TEST_FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    TEST_FAILED(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

    ///////////// Create depth buffer /////////////////////
    ComPtr<ID3D11Texture2D> pDepthStencil = NULL;
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = 798;
    descDepth.Height = 592;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    TEST_FAILED(pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil));

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // Stencil test parameters
    dsDesc.StencilEnable = false;

    // Create depth stencil state
    ComPtr<ID3D11DepthStencilState> pDSState;
    pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
    // Bind depth stencil state
    pContext->OMSetDepthStencilState(pDSState.Get(), 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    TEST_FAILED(pDevice->CreateDepthStencilView(pDepthStencil.Get(), // Depth stencil texture
        &descDSV, // Depth stencil desc
        &pDSV));  // [out] Depth stencil view

    // Bind the depth stencil view
    pContext->OMSetRenderTargets(1,          // One rendertarget view
        pTarget.GetAddressOf(),      // Render target view, created earlier
        pDSV.Get());     // Depth stencil view for the render target

}

CGraphics::~CGraphics()
{
}

void CGraphics::setCamera(DirectX::XMMATRIX newCamera)
{
    camera = newCamera;
}

DirectX::XMMATRIX CGraphics::getCamera()
{
    return camera;
}

void CGraphics::endFrame()
{
    // 0 - VSync Off
    // 1 - VSync On
    pSwapChain->Present(1u, 0u);
}

void CGraphics::clearBuffer(float red, float green, float blue)
{
    const float color[] = { red, green, blue, 1.0f };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
    pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CGraphics::draw(float alpha)
{
    HRESULT hr;
    struct Vertex
    {
        float x;
        float y;
    };

    const Vertex vertices[] =
    {
        { 0.0f, 0.5f },
        { 0.5f*sinf(alpha), -0.5f },
        { -0.5f*sinf(alpha), -0.5f },
        { -0.0f, -0.6f }
    };

    ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sData = {};
    sData.pSysMem = vertices;
    TEST_FAILED(pDevice->CreateBuffer(&bufferDesc, &sData, &pVertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0;
    pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

    // Create pixel shader
    ComPtr<ID3D11PixelShader> pPixelShader;
    ComPtr<ID3DBlob> pBlob;
    TEST_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
    TEST_FAILED(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
    // Bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

    // Create vertex shader
    ComPtr<ID3D11VertexShader> pVertexShader;
    TEST_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
    TEST_FAILED(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // Bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

    // Input layout
    ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC iElemDesc[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    TEST_FAILED(pDevice->CreateInputLayout(
        iElemDesc,
        (UINT)std::size(iElemDesc),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    // Bind input layout
    pContext->IASetInputLayout(pInputLayout.Get());

    // Bind render target
    //pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // Config viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = 800;
    viewport.Height = 600;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    pContext->RSSetViewports(1, &viewport);

    pContext->Draw((UINT)std::size(vertices), 0);
}
