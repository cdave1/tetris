#pragma once

class c2DRenderer {
public:
	LPDIRECT3D9             m_pD3D; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice; // Our rendering device
	cFont* m_pCFont;
	
	c2DRenderer ();

	void SetupCamera();
	void SetupPerspective();
	VOID Render(LPSTR pText);
	void PrintMessage(LPSTR pText);
	HRESULT InitD3D( HWND hWnd );
	VOID CleanUp();
};