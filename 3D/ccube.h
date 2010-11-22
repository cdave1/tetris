#pragma once
const float TETRIS_AREA_WIDTH = 400.0f;
const float TETRIS_AREA_HEIGHT = 800.0f;
const float BLOCK_SQUARE_SIZE = 40.0f;
class cCube
{
public:
	cCube();
	~cCube();
	cCube(LPDIRECT3DDEVICE9 pDevice,float x, float y, float z, DWORD dwColor);
	cCube(LPDIRECT3DDEVICE9 pDevice, float w, float h, float d, float x, float y, float z, DWORD dwColor);
	
	bool SetTexture(LPDIRECT3DDEVICE9 pDevice, const char *szTextureFilePath);
	bool CreateVertexBuffer();
	bool UpdateVertices();
	DWORD Render();
	void CleanUp();
	void Release();
	void MoveTo(float dx, float dy, float dz);
	void Rotate(float xRot, float yRot,float* dx, float* dy, float dRads);

	float m_width, m_height, m_depth;
	float m_x, m_y, m_z;
	float m_xStart, m_yStart, m_zStart;
private:
	D3DXMATRIX matNextMove;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;
	DWORD m_dwColor;
};
