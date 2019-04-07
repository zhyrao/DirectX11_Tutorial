#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	//m_Model = 0;
	//m_ColorShader = 0;
	//m_TextureShader = 0;
	//m_LightShader = 0;
	//m_LightClass = 0;
	//m_Bitmap = 0;
	m_Text = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int width, int height, HWND hWnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

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

	// create camera object
	m_Camera = new CameraClass();
	if (!m_Camera)
		return false;

	// create new camera for text class using
	m_Camera->SetPosition(0.f, 0.f, -1.f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// create text object
	m_Text = new TextClass;
	if (!m_Text) return false;

	// init text object
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hWnd, width, height, baseViewMatrix);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the text object.", "Error", MB_OK);
		return false;
	}

	// set the initial position of the camera
	// m_Camera->SetPosition(0.0f, 0.f, -10.f);


	// create the model object
	//m_Model = new ModelClass();
	//if (!m_Model) return false;

	// initialize model class	
	//result = m_Model->Initialize(m_D3D->GetDevice(), "D:\\Projects\\DirectX11_Tutorial\\Resources\\Models\\model.txt", "D:\\Projects\\DirectX11_Tutorial\\Resources\\Textures\\earth.dds");
	//if (!result)
	//{
	//	MessageBox(hWnd, "Could not initialize the model object.", "Error", MB_OK);
	//	return false;
	//}

	//// create color shader object
	//m_ColorShader = new ColorShaderClass();
	//if (!m_ColorShader) return false;
	//
	//// initialize the color shader object
	//result = m_ColorShader->Initialize(m_D3D->GetDevice(), hWnd);
	//if (!result)
	//{
	//	MessageBox(hWnd, "Could not initialize the color shader object.", "Error", MB_OK);
	//	return false;
	//}
	//
	// create texture shader
	//m_TextureShader = new TextureShaderClass();
	//if (!m_TextureShader) return false;
	//
	//// init the texture shader
	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hWnd);
	//if(!result)
	//{
	//	MessageBox(hWnd, "Could not initialize the texture shader object.", "Error", MB_OK);
	//	return false;
	//}

	//// create te light shader object
	//m_LightShader = new LightShaderClass;
	//if (!m_LightShader)
	//{
	//	return false;
	//}

	// initialize the light shader projection
	//result = m_LightShader->Initialize(m_D3D->GetDevice(), hWnd);
	//if (!result)
	//{
	//	MessageBox(hWnd, "Could not initialize the light shader object.", "Error", MB_OK);
	//	return false;
	//}
	////THE NEW LIGHT OBJECT IS CREATED HERE
	//m_LightClass = new LightClass;
	//if (!m_LightClass) return false;
	//
	//// initialize light object
	//m_LightClass->SetAmbientColor(0.15f, .15f, .15f, 1.f);
	//m_LightClass->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	//m_LightClass->SetDirection(1.f, 0.f, 1.f);
	//m_LightClass->SetSpecularColor(1.0f, 0.f, 0.f, 1.f);
	//m_LightClass->SetSpecularPower(64.0f);

	// create bitmap object
	//m_Bitmap = new BitmapClass;
	//if (!m_Bitmap) return false;
	//
	//// initialize the bitmap object
	//result = m_Bitmap->Initialize(m_D3D->GetDevice(), width, height, "D:\\Projects\\DirectX11_Tutorial\\Resources\\Textures\\earth.dds", 256, 256);
	//if (!result)
	//{
	//	MessageBox(hWnd, "Could not initialize the bitmap object.", "Error", MB_OK);
	//	return false;
	//}

	return true;
}

void GraphicsClass::ShutDown()
{
	//if (m_LightClass)
	//{
	//	delete m_LightClass;
	//	m_LightClass = 0;
	//}
	//
	//if (m_LightShader)
	//{
	//	m_LightShader->Shutdown();
	//	delete m_LightShader;
	//	m_LightShader = 0;
	//}

	// release the texture shader
	//if (m_TextureShader)
	//{
	//	m_TextureShader->Shutdown();
	//	delete m_TextureShader;
	//	m_TextureShader = 0;
	//}

	// release the color shader
	//if (m_ColorShader)
	//{
	//	m_ColorShader->Shutdown();
	//	delete m_ColorShader;
	//	m_ColorShader = 0;
	//}

	//if (m_Bitmap)
	//{
	//	m_Bitmap->Shutdown();
	//	delete m_Bitmap;
	//	m_Bitmap = 0;
	//}

	// release the model object
	//if (m_Model)
	//{
	//	m_Model->Shutdown();
	//	delete m_Model;
	//	m_Model = 0;
	//}

	// release text object
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// release the camera object
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

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
	
	static float rotation = 0.f;

	// update the rotation varialbe each frame
	rotation += (float)D3DX_PI* 0.0015f;
	if (rotation > 360.f)
		rotation = -360.f;

	// render the graphic scene
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// clear the buffers to begin the scene
	m_D3D->BeginScene(0.1f, 0.2f, 0.3f, 1.0f);

	// generate the view matrix based on camera's position
	m_Camera->Render();

	// get the world, view and projection matrices from camera and d3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// get ortho matrix
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// turn zbuffer off
	m_D3D->TurnZbufferOff();

	// turn on alpha blending
	m_D3D->TurnOnAlphaBlending();


	// render text string
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result) return false;

	// put the bitmap vertex and index buffers on the graphics pipeline to prepare drawing
	// result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100 + rotation * 10, 100 + rotation * 10);
	// if (!result) return false;

	// rotate the world matrix by the rotation value so that the triangle wil spin
	//D3DXMatrixRotationY(&worldMatrix, rotation);

	// put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	// m_Model->Render(m_D3D->GetDeviceContext());

	// result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	// render the model using color shader
	// result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	// result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	// result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_Model->GetTexture(), m_LightClass->GetDirection(), m_LightClass->GetAmbientColor(), m_LightClass->GetDiffuseColor(),
	//	m_Camera->GetPosition(), m_LightClass->GetSpecularColor(), m_LightClass->GetSpecularPower());
	// if (!result) return false;


	// turn off
	m_D3D->TurnOffAlphaBlending();

	// TURN ON zbuffer
	m_D3D->TurnZbufferOff();

	// present the rendered scene to the screen
	m_D3D->EndScene();
	return true;
}
