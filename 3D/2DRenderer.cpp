#include "stdafx.h"

c2DRenderer::c2DRenderer () 
{
	m_pD3D       = NULL; // Used to create the D3DDevice
	m_pd3dDevice = NULL; // Our rendering device
}

void c2DRenderer::SetupCamera() 
{
	D3DXMATRIX matOrtho, matWorldTrans;
	D3DXMATRIX matIdentity;

	D3DXMatrixOrthoLH(&matOrtho, TETRIS_AREA_WIDTH, TETRIS_AREA_HEIGHT, 0.0f, 1.0f);
	D3DXMatrixTranslation(&matWorldTrans, -(TETRIS_AREA_WIDTH/2), -(TETRIS_AREA_HEIGHT/2), 0.0f);
	D3DXMatrixIdentity(&matIdentity);

	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorldTrans);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matWorldTrans);

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void c2DRenderer::SetupPerspective()
{
	// This is the "FOV"
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/2, 1.0f, 1.0f, 500.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID c2DRenderer::Render(LPSTR pText)
{
	if (m_pd3dDevice == NULL) {
		return;
	}

	// Clear the backbuffer to a blue color
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		SetupCamera();
		m_pCFont->DrawFontText(pText, 0, 0);
		m_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT c2DRenderer::InitD3D( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	// Get the current display mode
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = d3ddm.Format;

	// Create the D3DDevice
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp, &m_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Device state would normally be set here

	// Set the culling mode - cull back faces with counterclockwise vertices
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// Turn off lighting - apparently we do this because we are specifying that our
	// vertices have colour
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Set up fonts
	m_pCFont = new cFont(m_pd3dDevice, "Verdana", 18, false, false, false);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID c2DRenderer::CleanUp()
{
	if( m_pd3dDevice != NULL ) 
		m_pd3dDevice->Release();

	if( m_pD3D != NULL )       
		m_pD3D->Release();
}