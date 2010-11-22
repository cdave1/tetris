#include "stdafx.h"

// A structure for our custom vertex type
struct CUBE_CUSTOMVERTEX
{
    FLOAT x, y, z; // The transformed position for the vertex
    DWORD color;        // The vertex color
	FLOAT tu, tv; // - units across, units down
};

cCube::cCube() {}

cCube::~cCube()
{
	SafeRelease(m_pVertexBuffer);
}

cCube::cCube(LPDIRECT3DDEVICE9 pDevice,float x, float y, float z, DWORD dwColor)
{
	m_pd3dDevice = pDevice;
	m_pVertexBuffer = NULL;
	m_width = BLOCK_SQUARE_SIZE;
	m_height = BLOCK_SQUARE_SIZE;
	m_depth = BLOCK_SQUARE_SIZE;
	m_x = x;
	m_y = y;
	m_z = z;
	m_xStart = x;
	m_yStart = y;
	m_zStart = z;
	m_dwColor = dwColor;
	
	D3DXMatrixIdentity(&matNextMove);

	if (CreateVertexBuffer())
	{
		if (UpdateVertices())
		{
			return;
		}
	}
}

cCube::cCube(LPDIRECT3DDEVICE9 pDevice, float w, float h, float d, float x, float y, float z, DWORD dwColor)
{
	m_pd3dDevice = pDevice;
	m_pVertexBuffer = NULL;
	m_width = w;
	m_height = h;
	m_depth = d;
	m_x = x;
	m_y = y;
	m_z = z;
	m_xStart = x;
	m_yStart = y;
	m_zStart = z;
	m_dwColor = dwColor;
	
	D3DXMatrixIdentity(&matNextMove);

	if (CreateVertexBuffer())
	{
		if (UpdateVertices())
		{
			return;
		}
	}
}


bool cCube::CreateVertexBuffer() 
{
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(18 * sizeof( CUBE_CUSTOMVERTEX),
												0, TEXTUREFVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL)))
	{
		return false;
	}
	return true;
}

bool cCube::UpdateVertices() 
{
	VOID* pVertices;

	CUBE_CUSTOMVERTEX vertices[] =
	{
		// Top Face
		{m_x, m_y, m_z, 0xff333333, 0.0f, 1.0f,},
		{m_x, m_y, m_z+m_depth, 0xff333333, 0.0f, 0.0f,},
		{m_x+m_width, m_y, m_z, m_dwColor, 1.0f, 1.0f,},
		{m_x+m_width, m_y, m_z+m_depth, 0xff333333, 1.0f, 0.0f,},

		// Face 1
		{m_x, m_y-m_height, m_z, 0xff333333, 0.0f, 1.0f,},
		{m_x, m_y, m_z, 0xff333333, 0.0f, 0.0f,},
		{m_x+m_width, m_y-m_height, m_z, m_dwColor, 1.0f, 1.0f,},
		{m_x+m_width, m_y, m_z, 0xff333333, 1.0f, 0.0f,},

		// Face 2
		{m_x+m_width, m_y-m_height, m_z+m_depth, 0xff333333, 0.0f, 1.0f,},
		{m_x+m_width, m_y, m_z+m_depth, m_dwColor, 0.0f, 0.0f},

		// Face 3
		{m_x, m_y-m_height, m_z+m_depth, 0xff333333, 1.0f, 1.0f,},
		{m_x, m_y, m_z+m_depth, m_dwColor, 1.0f, 0.0f},

		// Face 4
		{m_x, m_y-m_height, m_z, 0xff333333, 0.0f, 1.0f,},
		{m_x, m_y, m_z, m_dwColor, 0.0f, 0.0f,},

		// Bottom Face
		{m_x+m_width, m_y-m_height, m_z, 0xff333333, 0.0f, 1.0f, },
		{m_x+m_width, m_y-m_height, m_z+m_depth, 0xff333333, 0.0f, 0.0f,},
		{m_x, m_y-m_height, m_z, m_dwColor, 1.0f, 1.0f,},
		{m_x, m_y-m_height, m_z+m_depth, 0xff333333, 1.0f, 0.0f},
	};

	if( FAILED( m_pVertexBuffer->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ) ) )
		return false;
	memcpy( pVertices, vertices, sizeof(vertices) );
	m_pVertexBuffer->Unlock();

	return true;
}

DWORD cCube::Render()
{
	//if (m_pTexture != NULL)
	//{
	//	m_pd3dDevice->SetTexture(0, m_pTexture);
	//	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//}
	//else 
	//{
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//}
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matNextMove);
	m_pd3dDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(CUBE_CUSTOMVERTEX) );
	m_pd3dDevice->SetFVF(TEXTUREFVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); //Draw the first two triangles
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 8); //From the 4th vertex, draw 8 triangles
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 14, 2); // From the 14th vertex, draw 2 triangles
	return 12;
}

void cCube::Release() 
{
	SafeRelease(m_pd3dDevice);
}

void cCube::MoveTo(float dx, float dy, float dz) 
{
	m_x += dx;
	m_y += dy;
	m_z += dz;
	D3DXMATRIX matTrans;
	// Translates the origin x and y points from original spot
	D3DXMatrixTranslation(&matNextMove, m_x-m_xStart, m_y-m_yStart, m_z-m_zStart);
}

void cCube::Rotate(float xRot, float yRot,float* dx, float* dy, float dRads)
{
	int x = 0,y = 0;

	D3DXMATRIX matTrans, matRotate, matFinal;
	D3DXMatrixTranslation(&matTrans, -xRot, -yRot, 0.0f);
	D3DXMatrixRotationZ(&matRotate, dRads);
	D3DXMatrixTranslation(&matFinal, xRot, yRot, 0.0f);
	matFinal = matTrans * matRotate * matFinal;

	// Standard matrix calculation; cast as an integer
	// so the blocks stay within their correct squares in case
	// of an imprecise result.
	x = (int)(m_x * ((D3DMATRIX)matFinal)._11) +
		(m_y * ((D3DMATRIX)matFinal)._21) +
		(1.0f * ((D3DMATRIX)matFinal)._31) +
		(1.0f * ((D3DMATRIX)matFinal)._41);
	y = (int)(m_x * ((D3DMATRIX)matFinal)._12) +
		(m_y * ((D3DMATRIX)matFinal)._22) +
		(1.0f * ((D3DMATRIX)matFinal)._32) +
		(1.0f * ((D3DMATRIX)matFinal)._42);
	
	// Precision check - rounds the x position to
	// the nearest multiple of BLOCK_SQUARE_SIZE
	if (x%(int)BLOCK_SQUARE_SIZE != 0)
	{
		x = x+(x%(int)BLOCK_SQUARE_SIZE)+BLOCK_SQUARE_SIZE;
	}

	// Precision check - rounds the y position to
	// the nearest multiple of BLOCK_SQUARE_SIZE
	if (y%(int)BLOCK_SQUARE_SIZE != 0)
	{
		y=y-(y%(int)BLOCK_SQUARE_SIZE)+BLOCK_SQUARE_SIZE;
	}

	*dx = (x-m_x);
	*dy = (y-m_y)+BLOCK_SQUARE_SIZE; // offset y movement by square size.
}

bool cCube::SetTexture(LPDIRECT3DDEVICE9 pDevice, const char *szTextureFilePath) 
{
	if(FAILED(D3DXCreateTextureFromFile(pDevice, szTextureFilePath, &m_pTexture))) 
	{
		return false;
	}
	return true;
}

void cCube::CleanUp() 
{
	if( m_pVertexBuffer != NULL )        
		m_pVertexBuffer->Release();
}
