#include "stdafx.h"

c3DRenderer::c3DRenderer () 
{
	m_pD3D       = NULL; // Used to create the D3DDevice
	m_pd3dDevice = NULL; // Our rendering device
}

void c3DRenderer::Setup3DCamera() 
{
	// Camera has:
	// - Position (0,0-30)
	// - Look at position (0,0,0)
	// - Up direction (y axis)
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(TETRIS_AREA_WIDTH/2, TETRIS_AREA_HEIGHT/2, -400.0f),
								&D3DXVECTOR3(TETRIS_AREA_WIDTH/2, TETRIS_AREA_HEIGHT/2, 0.0f),
								&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//D3DXMatrixTranslation(&matView, 100.0f, 200.0f, 50.0f);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	//m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);D3DRS_DITHERENABLE
	m_pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	//m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG); // defaults to GOURAUD
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void c3DRenderer::Setup2DCamera() 
{
	D3DXMATRIX matOrtho, matWorldTrans;
	D3DXMATRIX matIdentity;

	D3DXMatrixOrthoLH(&matOrtho, TETRIS_AREA_WIDTH, TETRIS_AREA_HEIGHT, 0.0f, 1.0f);
	D3DXMatrixTranslation(&matWorldTrans, -(TETRIS_AREA_WIDTH/2), -(TETRIS_AREA_HEIGHT/2), 0.0f);
	D3DXMatrixIdentity(&matIdentity);

	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorldTrans);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matWorldTrans);


}

void c3DRenderer::SetupPerspective()
{
	// This is the "FOV"
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/2, 0.5f, 1.0f, 6000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID c3DRenderer::Render(LPSTR pText, cBlock* block, cPile* pile)
{
	//if (msg != NULL)
	//{
	//	m_oTestSquare->MoveTo((float)msg->pt.x, -(float)msg->pt.y);
	//}
	if (m_pd3dDevice == NULL) 
	{
		return;
	}

	// Clear the backbuffer to a blue color
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		Setup2DCamera();
		m_pCFont->DrawFontText(pText, 0, 0);

		Setup3DCamera();
		SetupPerspective();
		block->Render();
		pile->Render();
		m_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT c3DRenderer::InitD3D( HWND hWnd )
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
VOID c3DRenderer::CleanUp()
{
	if( m_pd3dDevice != NULL ) 
		m_pd3dDevice->Release();

	if( m_pD3D != NULL )       
		m_pD3D->Release();
}