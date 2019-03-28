#include "TextureClass.h"



TextureClass::TextureClass()
{
	m_Texture = 0;
}

TextureClass::TextureClass(const TextureClass & other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device * device, LPCSTR textureFileName)
{
	HRESULT result;

	// load texture in
	result = D3DX11CreateShaderResourceViewFromFile(device, textureFileName, NULL, NULL, &m_Texture, NULL);
	if (FAILED(result)) return false;

	return true;
}

void TextureClass::Shutdown()
{
	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = 0;
	}
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
	return m_Texture;
}
