#pragma once

// includes
#include<d3d11.h>
#include<D3DX10math.h>
#include<D3dx11async.h>
#include<fstream>

using namespace std;

// light shader class
class LightShaderClass
{
private:
	struct MatrixBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};

	struct LightBufferType {
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR4 specularColor;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* texture,
		D3DXVECTOR3 dir, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCSTR vertexFileName, LPCTSTR pixelFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* erroBolo, HWND hwnd, LPCTSTR filename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
		                     ID3D11ShaderResourceView* texture, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDir,
							 D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_Layout;
	ID3D11SamplerState* m_SampleState;
	ID3D11Buffer* m_MatrixBuffer;
	ID3D11Buffer* m_CameraBuffer;
	// new constant buffer for light information(color and direction)
	ID3D11Buffer* m_LightBuffer;
};

