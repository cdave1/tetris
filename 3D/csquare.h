#pragma once
const float TETRIS_AREA_WIDTH = 300.0f;
const float TETRIS_AREA_HEIGHT = 600.0f;
const float BLOCK_SQUARE_SIZE = 30.0f;
class cSquare
{
public:
	cSquare(LPDIRECT3DDEVICE9 pDevice, float x, float y, DWORD dwColor);
	~cSquare();
	bool CreateVertexBuffer();
	bool UpdateVertices();
	DWORD Render();
	void Release();
	void MoveTo(float xDistance, float yDistance);
	void Rotate(float xRot, float yRot,float* dx, float* dy, float dRads);

	float m_x;
	float m_y;
	float m_xStart;
	float m_yStart;
private:
	D3DXMATRIX matNextMove;
	D3DXMATRIX matRotationMove;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	DWORD m_dwColor;
};