#include "CCube.h"
#include "CustomExceptions.h"
#include <array>
#include <D3DCompiler.h>
#include <dxgi.h>
#include "TCube.h"
#include "testmacro.h"

#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"DXGI.lib")
using namespace DirectX;

Cube::Cube(CGraphics& graphics)
{
    position = { 0.0f, 0.0f, 5.0f };
    size = { 1.0f,1.0f,1.0f };
    rotation = { 0.0f,0.0f,0.0f };
    if (pVertexBuffer == nullptr)
    {
        HRESULT hr; 

        // Create vertex buffer
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.ByteWidth = sizeof(vertices);
        bufferDesc.StructureByteStride = sizeof(Vertex);

        D3D11_SUBRESOURCE_DATA sData = {};
        sData.pSysMem = vertices;
        TEST_FAILED(getDevice(graphics)->CreateBuffer(&bufferDesc, &sData, &pVertexBuffer));
        // Create index buffer
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.ByteWidth = sizeof(indices);
        bufferDesc.StructureByteStride = sizeof(int);
        sData.pSysMem = indices;
        TEST_FAILED(getDevice(graphics)->CreateBuffer(&bufferDesc, &sData, &pIndicesBuffer));
        // Create pixel shader
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        TEST_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
        TEST_FAILED(getDevice(graphics)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
        // Create vertex shader
        TEST_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
        TEST_FAILED(getDevice(graphics)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
        // Create input layer
        const D3D11_INPUT_ELEMENT_DESC iElemDesc[] =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        TEST_FAILED(getDevice(graphics)->CreateInputLayout(
            iElemDesc,
            (UINT)std::size(iElemDesc),
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            &pInputLayout
        ));
        // Create constant buffer
        constantBuffer.matrix = DirectX::XMMatrixIdentity();

        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;
        bufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
        bufferDesc.StructureByteStride = 0;

        sData.pSysMem = &constantBuffer;

        getDevice(graphics)->CreateBuffer(&bufferDesc, &sData, &pConstantBuffer);
    }
}

void Cube::draw(CGraphics& graphics)
{
    //constantBuffer.matrix = DirectX::XMMatrixIdentity();
    // Bind constant buffer
    getContext(graphics)->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
    D3D11_MAPPED_SUBRESOURCE sData = {};
    getContext(graphics)->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sData);
    constantBuffer.matrix = XMMatrixTranspose(    XMMatrixRotationRollPitchYaw(rotation.x*XM_2PI, rotation.y* XM_2PI, rotation.z* XM_2PI)*
                                                  XMMatrixScaling(size.x, size.y, size.z)*
                                                  XMMatrixTranslation(position.x,position.y,position.z)*
                                                  getCamera(graphics)*
                                                  getPerspective(graphics));
    memcpy(sData.pData, &constantBuffer, sizeof(constantBuffer));
    getContext(graphics)->Unmap(pConstantBuffer.Get(), 0);
    
    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0;
    getContext(graphics)->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
    // Bind pixel shader
    getContext(graphics)->PSSetShader(pPixelShader.Get(), nullptr, 0);
    // Bind vertex shader
    getContext(graphics)->VSSetShader(pVertexShader.Get(), nullptr, 0);
    // Bind input layout
    getContext(graphics)->IASetInputLayout(pInputLayout.Get());
    // Bind render target
    getContext(graphics)->OMSetRenderTargets(1, getTarget(graphics).GetAddressOf(), getDepthStencilView(graphics).Get());
    // Bind index buffer
    getContext(graphics)->IASetIndexBuffer(pIndicesBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    // Set primitive topology to triangle
    getContext(graphics)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Config viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = 800;
    viewport.Height = 600;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    getContext(graphics)->RSSetViewports(1, &viewport);

    getContext(graphics)->DrawIndexed(std::size(indices), 0, 0);
}
