#pragma once

#include<d3d11.h>
#include<D3DX10math.h>
#include<D3DX11async.h>
#include<fstream>
using namespace std;


class FontShaderClass
{
private:
	struct ConstantBufferType	// per frame
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct PixelBufferType		// pixel color
	{
		D3DXVECTOR4 pixelColor;
	};
public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass& other);
	~FontShaderClass();


	bool Initialize(ID3D11Device* device, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
		ID3D11ShaderResourceView* texture, D3DXVECTOR4 color);

private:
	bool InitializeShader(ID3D11Device* device, HWND hWnd, LPCSTR vertexFilename, LPCSTR pixelFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, LPCSTR filename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view,
		D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR4 color);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_constantBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_pixelBuffer;
};

