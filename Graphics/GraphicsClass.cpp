#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int width, int height, HWND hWnd)
{
	bool result;

	// create the Direct3D object
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// initialize the Direct3D object
	result = m_D3D->Initialize(width, height, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	//char videoCardInfo[128];
	//int mem;
	//m_D3D->GetVideoCardInfo(videoCardInfo, mem);
	//MessageBox(hWnd, videoCardInfo, "Info", MB_OK);
	return true;
}

void GraphicsClass::ShutDown()
{
	if (m_D3D)
	{
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	// render the graphic scene
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	// clear the buffers to begin the scene
	m_D3D->BeginScene(1.f, 1.f, 0.f, 1.0f);


	// present the rendered scene to the screen
	m_D3D->EndScene();
	return true;
}
