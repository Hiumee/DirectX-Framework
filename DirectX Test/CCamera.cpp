#include "CCamera.h"

CCamera::CCamera()
{
    x = y = z = roll = pitch = yaw = 0.00001f;
}

DirectX::XMMATRIX CCamera::getCamera()
{
    //const auto position = DirectX::XMVectorSet(x, y, z, 0.0f);

    //const auto position = DirectX::XMVector3Transform(
    //    DirectX::XMVectorSet(x, y, z, 0.0f),
    //    DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, 0.0f)
    //);
    
    return DirectX::XMMatrixTranslation(-x, -y, -z)* DirectX::XMMatrixRotationNormal(DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), yaw) *
            DirectX::XMMatrixRotationNormal(DirectX::XMVectorSet(-1.0f,0.0f,0.0f,0.0f), roll);
}

void CCamera::setCamera(float x, float y, float z, float roll, float pitch, float yaw)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->roll = roll;
    this->pitch = pitch;
    this->yaw = yaw;
}
