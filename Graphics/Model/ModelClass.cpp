#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;

	m_TextureClass = 0;
}

ModelClass::ModelClass(const ModelClass & other)
{
}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device * device, LPCSTR textureFileName)
{
	bool result;

	// initialize the vertex and index buffer that hold geometry for the triangle
	result = InitializeBuffers(device);
	if (!result)
		return false;

	// load texture 
	result = LoadTexture(device, textureFileName);
	if (!result)
		return false;

	return true;
}

void ModelClass::Shutdown()
{
	// release texture
	ReleaseTexture();

	// release the vertex and index buffers
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext * deviceContext)
{
	// put the vertex and index buffers on the graphics pipeline 
	// to prepare them for drawing
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D11ShaderResourceView * ModelClass::GetTexture()
{
	return m_TextureClass->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device * device)
{
	//VertexType* vertices;
	TextureVertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;

	// set the number of vertices in the vertex array
	m_VertexCount = 3;

	// set the number of the indices in the index array
	m_IndexCount = 3;

	// create the vertex array
	//vertices = new VertexType[m_VertexCount];
	vertices = new TextureVertexType[m_VertexCount];
	if (!vertices)
	{
		return false;
	}

	// create the index array
	indices = new unsigned long[m_IndexCount];
	if (!indices)
	{
		return false;
	}

	// load the vertex array with data
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.f, 0.f); // bottom left
	//vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.f, 1.f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.f, 0.f); // top middle
	//vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.f, 1.f);
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.f, 0.f); // bottom right
	//vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.f, 1.f);	
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);


	// load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.ByteWidth = sizeof(TextureVertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
	if (FAILED(result)) return false;

	// release the array now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// release the index buffer
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	// release the vertex buffer
	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// set vertex buffer stride and offset
	//stride = sizeof(VertexType);
	stride = sizeof(TextureVertexType);
	offset = 0;

	// set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// set the index buffer to active
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set the type of primitive that should be rendered from this vertex buffer, 
	// in this case, triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device * device, LPCSTR textureFileName)
{
	bool result;

	// create the texture object
	m_TextureClass = new TextureClass();
	if (!m_TextureClass)
		return false;
	// initialize texture object
	result = m_TextureClass->Initialize(device, textureFileName);
	if (!result) return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	// release the texture object
	if (m_TextureClass)
	{
		m_TextureClass->Shutdown();
		delete m_TextureClass;
		m_TextureClass = 0;
	}
}

