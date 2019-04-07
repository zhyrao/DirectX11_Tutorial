#include "TextClass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
}

TextClass::TextClass(const TextClass & other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// store the screen size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// store the base view matrix
	m_baseViewMatrx = baseViewMatrix;

	// create and init the font object
	m_Font = new FontClass;
	if (!m_Font) return false;

	// init font object
	result = m_Font->Initialize(device, "D:/Projects/DirectX11_Tutorial/Resources/Font/data/fontdata.txt", "D:/Projects/DirectX11_Tutorial/Resources/Font/data/font.dds");
	if (!result) 
	{
		MessageBox(hwnd, "Could not initialize the font object.", "Error", MB_OK);
		return false;
	}

	// create font shader class object
	m_FontShader = new FontShaderClass;
	if (!m_FontShader) return false;

	// init font shader object
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the font shader class object.", "Error", MB_OK);
		return false;
	}

	// Create and initialize the two strings that will be used for this tutorial. One string says Hello in white at 100, 100
	// and the other says Goodbye in yellow at 100, 200. The UpdateSentence function can be called to change the contents, 
	// location, and color of the strings at any time.
	// init first sentence
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result)
		return false;

	// now update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence1, (char*)"Hello World!", 100, 100, 1.f, 1.f, 1.f, deviceContext);
	if (!result) return false;

	// init the second sentence
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
		return false;

	// update
	result = UpdateSentence(m_sentence2, (char*)"GoodBye Night", 100, 200, 1.f, 1.f, 0.f, deviceContext);
	if (!result) return false;
	return true;
}

void TextClass::Shutdown()
{
	// release first sentence
	ReleaseSentence(&m_sentence1);

	ReleaseSentence(&m_sentence2);

	// release font shader obj
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// release font object
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}
}

bool TextClass::Render(ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	// darw the first sentence
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result) return false;

	// draw second
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if (!result) return false;

	return true;
}

bool TextClass::InitializeSentence(SentenceType ** sentence, int maxLength, ID3D11Device * device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// create new sentence object
	*sentence = new SentenceType;
	if (!*sentence) return false;

	// init the sentence buffer to null
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// set the maximum length of the sentence
	(*sentence)->maxLength = maxLength;

	// set the number of vertices of the sentence
	(*sentence)->vertexCount = 6 * maxLength;

	// set the number of the indexes in the index array
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// create the vertex array
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices) return false;

	// create index array
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices) return false;

	// init the vertex array to zeros at first
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// init the index array
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// During the creation of the vertex buffer description for the sentence 
	// we set the Usage type to dynamic as we may want to change the contents of the sentence at any time.
	// set up the description of the dynamic vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*(*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType * sentence, char * text, int positionX, int positionY, float r, float g, float b, ID3D11DeviceContext * deviceContext)
{
	int numletters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// store the color of the sentece
	sentence->red = r;
	sentence->green = g;
	sentence->blue = b;

	// get the number of letter in sentence
	numletters = (int)strlen(text);

	// check for position buffer overflow
	if (numletters > sentence->vertexCount)
		return false;

	// create vertex arrray
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices) return false;

	// init vertex array to zeros 
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// calculate the x and y pixel position on the screen to start drawing to
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// use the font class to build the vertex array from 
	// sentence text and sentence draw location
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// lock the vertex buffer
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	// get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	// copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));

	// unlock the vertex buffer
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// release the vertex array 
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType **sentence)
{
	if (*sentence)
	{
		// release the sentence vertex buffer
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// release the sentence index buffer
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// release the sentence
		delete *sentence;
		*sentence = 0;
	}
}

bool TextClass::RenderSentence(ID3D11DeviceContext * deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// set the index buffer
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set the type of primitive that should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// create a pixel color vector with the input sentence color
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.f);

	// render the text using the font shader
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrx, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result) return false;

	return true;
}
