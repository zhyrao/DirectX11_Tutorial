#include "D3DClass.h"
#include <stdlib.h>
#include <string.h>

D3DClass::D3DClass()
{
	m_SwapChain = 0;
	m_Device = 0;
	m_DeviceContext = 0;
	m_RenderTargetView = 0;
	m_DepthStencilBuffer = 0;
	m_DepthStencilState = 0;
	m_DepthStencilView = 0;
	m_RasterState = 0;
}

D3DClass::D3DClass(const D3DClass& dc)
{

}

D3DClass::~D3DClass()
{

}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullScreen, float screenDepth, float screenNear)
{
	HRESULT result;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;

	unsigned int numModes, numerator, denominator;
	size_t stringLength;

	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	int error;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* blackBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewPort;

	float fieldOfView, screenAspect;

	// store the vsync setting
	m_Vsync_Enabled = vsync;

	// create a directx graphics interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// use the factory to create an adapter for the primary graphics interface(video card)
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) {
		return false;
	}

	// enumerate the primary adapter output(monitor)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM
	// display format for the adapter output(monitor)
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// create a list to hold all the possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// now go through all the display modes and find the one that matches the screen width
	// and height. when a match is found store the numerator and denominator of the refresh
	// rate for that monitor
	for (int i = 0; i < (int)numModes; i++)
	{
		if (displayModeList[i].Height == (unsigned int)screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// get the adapter(video card) description
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// store the dedicated video card memory in megabytes
	m_VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// convert the name of the video card to a character array and store it
	error = wcstombs_s(&stringLength, m_VideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// release display mode list
	delete[] displayModeList;
	displayModeList = 0;

	// release the adapter output
	adapterOutput->Release();
	adapterOutput = 0;

	// release the adapter
	adapter->Release();
	adapter = 0;

	// release the factory
	factory->Release();
	factory = 0;

	// initialize the swap chain description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// set to a single back buffer
	swapChainDesc.BufferCount = 1;

	// set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// set regular 32-bit surface for the back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// set refresh rate of the back buffer
	if (m_Vsync_Enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// set the handle for the window to render to
	swapChainDesc.OutputWindow = hWnd;

	// turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// set to full screen or windowed mode
	if (fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// don't set the advanced flags
	swapChainDesc.Flags = 0;

	// set the feature level to dx 11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// create the swapchain, Direct3D device, Direct3D device context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);

	if (FAILED(result))
	{
		return false;
	}

	// get the pointer to the back buffer
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&blackBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// create the render target view with the back buffer pointer
	result = m_Device->CreateRenderTargetView(blackBufferPtr, NULL, &m_RenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// release pointer to the back buffer as we no longer need it.
	blackBufferPtr->Release();
	blackBufferPtr = 0;

	/////////////////////////////////DEPTH BUFFER --- STENCIL BUFFER /////////////////////////////////////////
	// initialize the description of the depth buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// set up the description of the depth buffer
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// create the texture for the depth buffer using the filled out description
	result = m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// initialize the description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// set up the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;


	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// set the depth stencil state
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	// initialize the depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// create the depth stencil view
	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// SET UP THE raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;// D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterizer state from the description we just filled out
	result = m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState);
	if (FAILED(result))
	{
		return false;
	}

	// now set  the rasterizer state
	m_DeviceContext->RSSetState(m_RasterState);

	// setup the viewport for rendering
	viewPort.Width = (float)screenWidth;
	viewPort.Height = (float)screenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	// create the viewport
	m_DeviceContext->RSSetViewports(1, &viewPort);

	//setup the projection matrix
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// create the projection matrix for 3d rendering
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// initialize the world matrix to the identity matrix
	D3DXMatrixIdentity(&m_WorldMatrix);

	// create an orthographic projection matrix for 2d rendering
	D3DXMatrixOrthoLH(&m_OrthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}


void D3DClass::ShutDown()
{
	// before shuting down set to windowed mode or when you 
	// release the swap chain it will throw an exception
	if (m_SwapChain)
	{
		m_SwapChain->SetFullscreenState(false, NULL);
	}

	if (m_RasterState)
	{
		m_RasterState->Release();
		m_RasterState = 0;
	}

	if (m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = 0;
	}

	if (m_DepthStencilState)
	{
		m_DepthStencilState->Release();
		m_DepthStencilState = 0;
	}

	if (m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
		m_DepthStencilBuffer = 0;
	}

	if (m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = 0;
	}

	if (m_DeviceContext)
	{
		m_DeviceContext->Release();
		m_DeviceContext = 0;
	}

	if (m_Device)
	{
		m_Device->Release();
		m_Device = 0;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain = 0;
	}

	return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// set up the color to clear the buffer
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// clear the back buffer
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);

	// clear the depth buffer
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// present the back buffer to the screen since the rendering is complete
	if (m_Vsync_Enabled)
	{
		// lock to screen refresh rate
		m_SwapChain->Present(1, 0);
	}
	else
	{
		// present as fast as possible
		m_SwapChain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_Device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_DeviceContext;
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX& proj)
{
	proj = m_ProjectionMatrix;
	return;
}

void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_WorldMatrix;
	return;
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_OrthoMatrix;
	return;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_VideoCardDescription);
	memory = m_VideoCardMemory;
	return;
}

