#include "BitmapClass.h"



BitmapClass::BitmapClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{

}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, LPCSTR textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;

	// store screen size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// store the size in pixel that this bitmap should be rendered at
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// initialize the previous rendering position to negative one
	m_previousPosX = -1;
	m_previousPosY = -1;

	// init the vertex and index buffer
	result = InitialBuffers(device);
	if (!result) return false;

	// load the texture for this model
	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}


void BitmapClass::Shutdown()
{
	// release the model texture
	ReleaseTexture();

	// shutdown the vertex and index buffers
	ShutdownBuffers();
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	// rebuild the dynamic vertex buffer for rendering to possibly a different location on the screen
	result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result) return false;

	// put the vertex and index buffers on the graphics pipeline to prepare for drawing
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitialBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// set the number of vertices in the vertex array
	m_vertexCount = 6;

	// set the number of the indices in the index array
	m_indexCount = m_vertexCount;

	// create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;

	// create the indice array
	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	// initialize vertex array to zeros at first
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// load the index array with data
	for (i = 0; i < m_indexCount; i ++)
	{
		indices[i] = i;
	}

	// setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// now create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) return false;

	// setup the description of index
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) return false;

	// release the array now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	// release index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// release the vertex buffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContent, int posx, int posy)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// if the position we are rendering this bitmap has not changed then don't update the vertex
	// buffer since it currently has the correct parameters
	if ((posx == m_previousPosX) && (posy == m_previousPosY))
	{
		return true;
	}

	// if it has changed then update the position it is being rendered to
	m_previousPosX = posx;
	m_previousPosY = posy;

	// the four sides of the image need to be calculate
	// left side of the bitmap
	left = (float)((m_screenWidth / 2) * -1) + (float)posx;

	// right side
	right = left + (float)m_bitmapWidth;

	// top
	top = (float)(m_screenHeight / 2) - (float)posy;

	// bottom
	bottom = top - (float)m_bitmapHeight;

	// create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;

	// load the vertex array with data
	// first triangle
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // top left
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // bottom right
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // bottom left
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);


	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // top left
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // top right
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // bottom right
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// lock the vertex buffer so it can be written to
	result = deviceContent->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	// get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	// copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*m_vertexCount));

	// unlock the vertex buffer
	deviceContent->Unmap(m_vertexBuffer, 0);

	// release the vertex array
	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContent)
{
	unsigned int stride;
	unsigned int offset;

	// set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// set the vertex buffer to active in the input assembler so it can be rendered
	deviceContent->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// set the index buffer
	deviceContent->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set the type of primtive that should be rendered from this vertex buffer
	deviceContent->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool BitmapClass::LoadTexture(ID3D11Device* device, LPCSTR filename)
{
	bool result;

	// create texture obj
	m_Texture = new TextureClass;
	if (!m_Texture) return false;

	// init the texture obj
	result = m_Texture->Initialize(device, (LPCSTR)filename);
	if (!result) return false;

	return true;
}

void BitmapClass::ReleaseTexture()
{
	// release texture obj
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}