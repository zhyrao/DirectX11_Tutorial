#ifndef __TEXTCLASS_H_
#define __TEXTCLASS_H_

// The TextClass handles all the 2D text drawing that the application will need.
// It renders 2D text to the screen and uses the FontClass and FontShaderClass to assist it in doing so.


// includes
#include "Font.h"
#include "../Shader/FontShaderClass.h"

class TextClass {
private:
	struct SentenceType 
	{
		ID3D11Buffer* vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass& other);
	~TextClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

	bool SetMousePosition(int mousex, int mousey, ID3D11DeviceContext* deviceContext);

private:
	bool InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
	bool UpdateSentence(SentenceType* sentence, char* text, int posx, int posy, float r, float g, float b, ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrx;

	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};


#endif
