#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>

using namespace std;

#include "../Model/Texture/TextureClass.h"

class FontClass {


private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass& other);
	~FontClass();

	bool Initialize(ID3D11Device* device, LPCSTR fontFilename, LPCSTR textureFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	bool LoadFontData(LPCTSTR fontDataname);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, LPCSTR textureFilename);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};
