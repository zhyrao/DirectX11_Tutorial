#pragma once


/*
 *	INCLUDES
 */
#include <d3d11.h>
#include <d3dx10math.h>

#include <fstream>
using namespace std;

// class includes
#include "Texture/TextureClass.h"

class ModelClass {
private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	struct TextureVertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct LightVertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* device, const char* modelFileName, LPCSTR textureFileName);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	// get texture
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	// texture related
	bool LoadTexture(ID3D11Device* device, LPCSTR textureFileName);
	void ReleaseTexture();

	// model related
	bool LoadModel(const char* modelFilename);
	void ReleaseModel();

private:
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount;

	// own class
	TextureClass* m_TextureClass;

	ModelType* m_Model;
};
