#include "CameraClass.h"



CameraClass::CameraClass()
{
	m_PositionX = 0;
	m_PositionY = 0;
	m_PositionZ = 0;

	m_RotationX = 0;
	m_RotationY = 0;
	m_RotationZ = 0;
}


CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_PositionX = x;
	m_PositionY = y;
	m_PositionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_RotationX = x;
	m_RotationY = y;
	m_RotationZ = z;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_PositionX, m_PositionY, m_PositionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_RotationX, m_RotationY, m_RotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// setup the vector that points upwards;
	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	// setup the position of the camera in the world
	position.x = m_PositionX;
	position.y = m_PositionY;
	position.z = m_PositionZ;

	// setup where the camera is looking in the world
	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_RotationX * 0.0174532925f;
	yaw = m_RotationY * 0.0174532925f;
	roll = m_RotationZ * 0.0174532925f;

	// create the rotation matrix from the yaw, pitch, roll
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// transform the lookat and up vector by the rotation
	// matrix so the view is correctly rotated at the orgin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	// finally create view matrix from the three updated values
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &lookAt, &up);
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_ViewMatrix;
}
