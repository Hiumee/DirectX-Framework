#pragma once
#include <DirectXMath.h>

class CCamera
{
public:
    float x, y, z;
    float roll, pitch, yaw;
public:
    CCamera();
    DirectX::XMMATRIX getCamera();
    void setCamera(float x, float y, float z, float roll, float pitch, float yaw);
};