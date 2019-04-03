#pragma once


#include<D3DX10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float s);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR4 GetSpecularColor();
	D3DXVECTOR3 GetDirection();
	float GetSpecularPower();

private:
	D3DXVECTOR4 m_AmbientColor;
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR3 m_LightDirection;
	D3DXVECTOR4 m_SpecularColor;
	float       m_SpecularPower;
};

