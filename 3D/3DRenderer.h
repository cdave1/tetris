#pragma once

class c3DRenderer {
public:
	LPDIRECT3D9             m_pD3D; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice; // Our rendering device
	cFont* m_pCFont;
	
	c3DRenderer ();

	void Setup2DCamera();
	void Setup3DCamera();
	void SetupPerspective();
	VOID Render(LPSTR pText, cBlock* block, cPile* pile);
	void PrintMessage(LPSTR pText);
	HRESULT InitD3D( HWND hWnd );
	VOID CleanUp();
};