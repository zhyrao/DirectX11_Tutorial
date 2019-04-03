#include "LightClass.h"



LightClass::LightClass()
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_AmbientColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_DiffuseColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_LightDirection = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_AmbientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_DiffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_LightDirection;
}
