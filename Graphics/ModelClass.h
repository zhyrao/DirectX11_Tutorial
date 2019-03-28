#pragma once


/*
 *	INCLUDES
 */
#include <d3d11.h>
#include <d3dx10math.h>


class ModelClass {
private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount;
};