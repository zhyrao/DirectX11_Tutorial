#pragma once

#include<windows.h>


#include "d3d/D3DClass.h"

#include "../Input/Camera/CameraClass.h"
#include "Model/ModelClass.h"
#include "Shader/ColorShaderClass.h"
#include "Shader/TextureShaderClass.h"

//////////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////////
const bool         FULL_SCREEN       = false;
const bool         VSYNC_ENABLED     = true;
const float        SCREEN_DEPTH      = 1000.0f;
const float        SCREEN_NEAR       = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool Initialize(int width, int height, HWND hWnd);
	void ShutDown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
};

