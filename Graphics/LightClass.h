#pragma once


#include<D3DX10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR3 m_LightDirection;
};

