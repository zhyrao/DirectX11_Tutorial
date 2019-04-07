#include "Font.h"

FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;
}

FontClass::FontClass(const FontClass & other)
{
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device * device, LPCSTR fontFilename, LPCSTR textureFilename)
{
	bool result;

	// load in the text file containing the font data
	result = LoadFontData(fontFilename);
	if (!result) return false;

	// load the texture that has the font characters on it
	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

ID3D11ShaderResourceView * FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, letter;
	int index, i;

	// coerce the input vertices to a vertextype structure
	vertexPtr = (VertexType*)vertices;

	// get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	// init the index to the vertex array
	index = 0;

	// draw each letter onto a quad
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// if the letter is a space then just move over three pixels
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// first triangle in quad
			// top left
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);				
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			// bottom right
			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY - 16, 0.0f); 
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			// bottom left
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY - 16, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			// second triangle in quad
			// top left
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			// top right
			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			// bottom right
			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY - 16, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			// update the x location for drawing by the size of the letter and one pixel
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}

bool FontClass::LoadFontData(LPCTSTR fontDataname)
{
	ifstream fin;
	int i;
	char temp;

	// create the font spacing buffer
	m_Font = new FontType[95];
	if (!m_Font) return false;

	// read in the font size and spacing between chars
	fin.open(fontDataname);
	if (fin.fail()) return false;

	// read in the 95 used ascii characters for text
	for (i = 0; i < 95; i ++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// close file
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device * device, LPCSTR textureFilename)
{
	bool result;

	// create the texture obj
	m_Texture = new TextureClass;
	if (!m_Texture) return false;

	// init the texture obj
	result = m_Texture->Initialize(device, textureFilename);
	if (!result) return false;
	return true;
}

void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}
