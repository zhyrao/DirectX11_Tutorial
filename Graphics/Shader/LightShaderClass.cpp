#include "LightShaderClass.h"



LightShaderClass::LightShaderClass()
{
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_Layout = 0;
	m_SampleState = 0;

	m_MatrixBuffer = 0;
	m_LightBuffer = 0;
}

LightShaderClass::LightShaderClass(const LightShaderClass & other)
{
}


LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, "D:\\Projects\\DirectX11_Tutorial\\Shaders\\06\\vlight.hlsl", "D:\\Projects\\DirectX11_Tutorial\\Shaders\\06\\plight.hlsl");
	if (!result) return false;
	return true;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
}

bool LightShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* texture, D3DXVECTOR3 dir,
	D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor)
{
	bool result;

	// set the shader parameters that it will be use for rendering
	result = SetShaderParameters(deviceContext, world, view, proj, texture, ambientColor, diffuseColor, dir);
	if (!result) return false;

	// now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);
	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device * device, HWND hwnd, LPCSTR vertexFileName, LPCTSTR pixelFileName)
{
	HRESULT result;

	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// change polygonlayout to 3 so that it can accommodate a normal vector in the layout
	D3D11_INPUT_ELEMENT_DESC pologonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	// new description for light constant buffer
	D3D11_BUFFER_DESC lightBufferDesc;

	// initialize the pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// load in new light vertex shader
	// compile vertex shader code 
	result = D3DX11CompileFromFile(vertexFileName, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		                          &vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vertexFileName);
		}
		{
			MessageBox(hwnd, vertexFileName, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// compile pixel shader
	result = D3DX11CompileFromFile(pixelFileName, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		                          &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, pixelFileName);
		}
		{
			MessageBox(hwnd, pixelFileName, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// create shader from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(result)) return false;

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(result)) return false;

	// create vertex input layout description
	// this setup needs to match the vertextype structure int the modelcalss and in the shader
	pologonLayout[0].SemanticName = "POSITION";
	pologonLayout[0].SemanticIndex = 0;
	pologonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pologonLayout[0].InputSlot = 0;
	pologonLayout[0].AlignedByteOffset = 0;
	pologonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pologonLayout[0].InstanceDataStepRate = 0;

	pologonLayout[1].SemanticName = "TEXCOORD";
	pologonLayout[1].SemanticIndex = 0;
	pologonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	pologonLayout[1].InputSlot = 0;
	pologonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pologonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pologonLayout[1].InstanceDataStepRate = 0;

	pologonLayout[2].SemanticName = "NORMAL";
	pologonLayout[2].SemanticIndex = 0;
	pologonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pologonLayout[2].InputSlot = 0;
	pologonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pologonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pologonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(pologonLayout) / sizeof(pologonLayout[0]);

	// create vertex input layout
	result = device->CreateInputLayout(pologonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_Layout);
	if (FAILED(result)) return false;

	// realse the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// create a texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_SampleState);
	if (FAILED(result))
		return false;

	// setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// create the constant buffer pointer so we can access the vertex shader
	// constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
	if (FAILED(result)) return false;

	// constant buffer for light information
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// create light constant buffer
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_LightBuffer);
	if (FAILED(result)) return false;

	return true;
}

void LightShaderClass::ShutdownShader()
{
	// release all buffers
	if (m_LightBuffer)
	{
		m_LightBuffer->Release();
		m_LightBuffer = 0;
	}

	if (m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	// sampler state
	if (m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState = 0;
	}

	// release layout
	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	// pixel shader
	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	// vertex shader
	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, LPCTSTR filename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream out;

	// get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// get length of the message
	bufferSize = errorMessage->GetBufferSize();

	// open a file to write the error message to
	out.open("shader-error.txt");

	// write out the error message
	for (i = 0; i < bufferSize; ++i)
	{
		out << compileErrors[i];
	}

	// close file
	out.close();

	// release error message
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", filename, MB_OK);
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext * deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
	                                       ID3D11ShaderResourceView * texture, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDir)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* matrixDataPtr;
	LightBufferType* lightDataPtr;

	// transpose the matrices
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	// lock the constant buffer so it can be written to
	result = deviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	// get a pointer to the data in the constant buffer
	matrixDataPtr = (MatrixBufferType*)mappedResource.pData;

	// copy the matrices into the constant buffer
	matrixDataPtr->world = world;
	matrixDataPtr->view = view;
	matrixDataPtr->projection = projection;

	// unlock the cbuffer
	deviceContext->Unmap(m_MatrixBuffer, 0);

	// set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// now set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_MatrixBuffer);
	
	// set shader texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	
	///// set light constant buffer like matrix buffer
	result = deviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	lightDataPtr = (LightBufferType*)mappedResource.pData;

	lightDataPtr->ambientColor = ambientColor;
	lightDataPtr->diffuseColor = diffuseColor;
	lightDataPtr->lightDirection = lightDir;
	lightDataPtr->padding = 0.f;

	// unlock 
	deviceContext->Unmap(m_LightBuffer, 0);

	// set buffer number
	bufferNumber = 0;

	// finally set the light constant buffer in the pixel shader with updated values
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer);

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	// set the vertex input layout
	deviceContext->IASetInputLayout(m_Layout);

	// set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_VertexShader, NULL, 0);
	deviceContext->PSSetShader(m_PixelShader, NULL, 0);

	// set the samplers state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_SampleState);

	// render triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
