#pragma once

// linking
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

// include headers
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include "D3DX10math.h"

class D3DClass {
public:
	D3DClass();
	D3DClass(const D3DClass& dc);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullScreen, float screenDepth, float screenNear);
	void ShutDown();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX& proj);
	void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* cardName, int& memory);


	// two new functions for turning z buffer on and off
	void TurnZBufferOn();
	void TurnZbufferOff();

	// two new functions for turning on and off alpha blending
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
private:
	bool m_Vsync_Enabled;
	int m_VideoCardMemory;
	char m_VideoCardDescription[128];
	IDXGISwapChain* m_SwapChain;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilView* m_DepthStencilView;
	ID3D11RasterizerState* m_RasterState;

	D3DXMATRIX m_ProjectionMatrix;
	D3DXMATRIX m_WorldMatrix;
	D3DXMATRIX m_OrthoMatrix;

	// new depth stencil state for 2d rendering
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	// two new blending states
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};