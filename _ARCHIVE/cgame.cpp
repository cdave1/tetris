//-----------------------------------------------------------------------------
// File: Vertices.cpp
//
// Desc: In this tutorial, we are rendering some vertices. This introduces the
//       concept of the vertex buffer, a Direct3D object used to store
//       vertices. Vertices can be defined any way we want by defining a
//       custom structure and a custom FVF (flexible vertex format). In this
//       tutorial, we are using vertices that are transformed (meaning they
//       are already in 2D window coordinates) and lit (meaning we are not
//       using Direct3D lighting, but are supplying our own colors).
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <d3dx9.h>
#include "cuboid.cpp"




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
Cuboid* pCubes[5];


#define SafeRelease(pObject) if (pObject != NULL) {pObject->Release(); pObject=NULL;}


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

	// Get the current display mode
	D3DDISPLAYMODE d3ddm;
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
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
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Lights
	/*
		D3DLIGHT9 d3dLight;

	ZeroMemory(&d3dLight, sizeof(D3DLIGHT9));

	d3dLight.Type = D3DLIGHT_POINT;
	d3dLight.Diffuse.r = 1.0f;
	d3dLight.Diffuse.g = 1.0f;
	d3dLight.Diffuse.b = 1.0f;

	d3dLight.Ambient.r = 0.0f;
	d3dLight.Ambient.g = 0.0f;
	d3dLight.Ambient.b = 0.0f;

	d3dLight.Specular.r = 0.0f;
	d3dLight.Specular.g = 0.0f;
	d3dLight.Specular.b = 0.0f;

	d3dLight.Position.x = 0.0f;
	d3dLight.Position.y = 0.0f;
	d3dLight.Position.z = -10.0f;

	d3dLight.Attenuation0 = 1.0f;
	d3dLight.Attenuation1 = 0.0f;
	d3dLight.Attenuation2 = 0.0f;
	d3dLight.Range = 100.0f;

	g_pd3dDevice->SetLight(0, &d3dLight);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(32,32,32));
	*/


	// Set the culling mode - cull back faces with counterclockwise vertices
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

    return S_OK;
}

void SetupCamera() 
{
	// Camera has:
	// - Position (0,0-30)
	// - Look at position (0,0,0)
	// - Up direction (y axis)
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0.0f, 0.0f, -100.0f),
								&D3DXVECTOR3(5.0f, 5.0f, 5.0f), 
								&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}

void SetupPerspective()
{
	// This is the "FOV"
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 500.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void SetupLights()
{

}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID CleanUp()
{


    if( g_pd3dDevice != NULL ) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )       
        g_pD3D->Release();

	if (pCubes != NULL ) {
		for (int i = 0; i < 5; i++) 
		{
			if (pCubes[i] != NULL) {
				pCubes[i]->CleanUp();
			}
		}
	}
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// 4 matrices
	D3DXMATRIX mRX, mRY, mRZ, mRU1,
		mMR27, mML27, mMR9,
		mML9, mMD15, mMU15,
		mT2, mT3, mT4, mT5,
		mSU15;


	if (g_pd3dDevice == NULL) {
		return;
	}

    // Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
		SetupCamera();
		SetupPerspective();
		// Create the transformation matrices
		D3DXMatrixRotationX(&mRX, timeGetTime()/400.0f);
		D3DXMatrixRotationY(&mRY, timeGetTime()/400.0f);
		D3DXMatrixRotationZ(&mRZ, timeGetTime()/400.0f);

		// Rotation around a custom axis
		D3DXMatrixRotationAxis(&mRU1, &D3DXVECTOR3(1.0f, 1.0f, 0.0f), timeGetTime()/400.0f);

		// Translation matrices
		D3DXMatrixTranslation(&mMR27, 27.0, 0.0, 0.0);
		D3DXMatrixTranslation(&mML27, -27.0, 0.0, 0.0);
		D3DXMatrixTranslation(&mMR9, 9.0, 0.0, 0.0);
		D3DXMatrixTranslation(&mML9, -9.0, 0.0, 0.0);
		D3DXMatrixTranslation(&mMD15, 0.0, -15.0, 0.0);
		D3DXMatrixTranslation(&mMU15, 0.0, 15.0, 0.0);

		// Scalar Matrix
		D3DXMatrixScaling(&mSU15, 1.5,1.5,1.5);

		// Combine for transformation matrices
		D3DXMatrixMultiply(&mT2, &mMR9, &mRY);
		D3DXMatrixMultiply(&mT2, &mT2, &mML9);

		D3DXMatrixMultiply(&mT3, &mML9, &mRZ);
		D3DXMatrixMultiply(&mT3, &mT3, &mMR9);

		D3DXMatrixMultiply(&mT4, &mML27, &mRU1);
		D3DXMatrixMultiply(&mT4, &mT4, &mMR27);

		D3DXMatrixMultiply(&mT5, &mMD15, &mRY);
		D3DXMatrixMultiply(&mT5, &mT5, &mRX);
		D3DXMatrixMultiply(&mT5, &mT5, &mRZ);
		D3DXMatrixMultiply(&mT5, &mT5, &mMU15);
		D3DXMatrixMultiply(&mT5, &mT5, &mSU15);

		


		//for (int i = 0; i < 4; i++) 
		//{
			if (pCubes != NULL) {

				// These lines will move the cubes over, but it will screw up the matrix
				// transformations (the transforms happen around the origin, so as the cubes
				// get further away, they appear to be moving more quickly). Also, it eats
				// up memory because a new vertex buffer gets created each time. Need to 
				// figure out a way to reset the existing buffer.
				//pCubes[i]->x = pCubes[i]->x + 0.01;
				//pCubes[i]->InitVertexBuffer(g_pd3dDevice);
				g_pd3dDevice->SetTransform(D3DTS_WORLD, &mRX);
				pCubes[0]->Render(g_pd3dDevice);

				g_pd3dDevice->SetTransform(D3DTS_WORLD, &mT2);
				pCubes[1]->Render(g_pd3dDevice);

				g_pd3dDevice->SetTransform(D3DTS_WORLD, &mT3);
				pCubes[2]->Render(g_pd3dDevice);

				g_pd3dDevice->SetTransform(D3DTS_WORLD, &mT4);
				pCubes[3]->Render(g_pd3dDevice);

				g_pd3dDevice->SetTransform(D3DTS_WORLD, &mT5);
				pCubes[4]->Render(g_pd3dDevice);
		//	}
		}

        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void GameLoop()
{
	MSG msg;
	BOOL fMessage;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (msg.message != WM_QUIT) 
	{
		fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

		if (fMessage) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			Render();
		}
	}
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
		break;
		case WM_KEYUP:
			switch (wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hWnd);
					return 0;
				break;
			}
		break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

HRESULT InitCubes() 
{
	HRESULT res = true;
	for (int i = 0; i < 5; i++) 
	{
		res = res & SUCCEEDED(pCubes[i]->CreateVertexBuffer(g_pd3dDevice));
		res = res & SUCCEEDED(pCubes[i]->CreateIndexBuffer(g_pd3dDevice));
		res = res & SUCCEEDED(pCubes[i]->UpdateVertices(g_pd3dDevice));
	}
	return res;
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "D3D Tutorial", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( "D3D Tutorial", "D3D Tutorial 02: Vertices",
                              WS_OVERLAPPEDWINDOW, 50, 50, 800, 800,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
		
		//pCubes = new Cuboid[3];
		pCubes[0] = new Cuboid(30.0f, 50.0f, 5.0f, 0.0f, 1.0f, -7.0f, "abc.bmp");
		pCubes[1] = new Cuboid(50.0f, 5.0f, 15.0f, -8.0f, -8.0f, 9.0f, "earth.bmp");
		pCubes[2] = new Cuboid(20.0f, 20.0f, 20.0f, 6.0f, 10.0f, 0.0f, "env0.bmp");
		pCubes[3] = new Cuboid(20.0f, 20.0f, 20.0f, 3.0f, 4.0f, -3.0f, "lake.bmp");
		pCubes[4] = new Cuboid(100.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, "tiger.bmp");
		// Create the vertex buffer
		// Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

		if (SUCCEEDED(InitCubes()))
		{
			GameLoop();
        }
    }

	CleanUp();

    UnregisterClass( "D3D Tutorial", wc.hInstance );
    return 0;
}
