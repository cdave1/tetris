#pragma once
class cSquare;
class cBlock;
class cPile;
class CDirectx {
public:
	LPDIRECT3D9             m_pD3D; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice; // Our rendering device
	cFont* m_pCFont;
	
	CDirectx ();

	void SetupCamera();
	void Setup2DCamera();
	void SetupPerspective();
	VOID Render(LPSTR pText, cBlock* block, cPile* pile);
	void PrintMessage(LPSTR pText);
	HRESULT InitD3D( HWND hWnd );
	VOID CleanUp();
};