#include "stdafx.h"

// A structure for our custom vertex type
struct SQUARE_CUSTOMVERTEX
{
    FLOAT x, y, z; // The transformed position for the vertex
    DWORD color;  // The vertex color
};

cSquare::cSquare(LPDIRECT3DDEVICE9 pDevice, float x, float y, DWORD dwColor)
{
	m_pd3dDevice = pDevice;
	m_pVertexBuffer = NULL;
	m_x = x;
	m_y = y;
	m_xStart = x;
	m_yStart = y;
	m_dwColor = dwColor;

	if (CreateVertexBuffer())
	{
		if (UpdateVertices())
		{
			return;
		}
	}
}

cSquare::~cSquare() 
{
	SafeRelease(m_pVertexBuffer);
}

bool cSquare::CreateVertexBuffer()
{
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(4 * sizeof( SQUARE_CUSTOMVERTEX),
												0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL)))
	{
		return false;
	}
	return true;
}

bool cSquare::UpdateVertices()
{
	SQUARE_CUSTOMVERTEX* pVertices = NULL;

	m_pVertexBuffer->Lock(0, 4 * sizeof(SQUARE_CUSTOMVERTEX), (void**)&pVertices, 0);

	pVertices[0].color = 0xff333333;
	pVertices[1].color = m_dwColor;
	pVertices[2].color = 0xff333333;
	pVertices[3].color = 0xff333333;

	pVertices[0].x = m_x;
	pVertices[0].y = m_y;

	pVertices[1].x = m_x + BLOCK_SQUARE_SIZE;
	pVertices[1].y = m_y;

	pVertices[2].x = m_x;
	pVertices[2].y = m_y - BLOCK_SQUARE_SIZE;

	pVertices[3].x = m_x + BLOCK_SQUARE_SIZE;
	pVertices[3].y = m_y - BLOCK_SQUARE_SIZE;

	pVertices[0].z = 1.0f;
	pVertices[1].z = 1.0f;
	pVertices[2].z = 1.0f;
	pVertices[3].z = 1.0f;

	m_pVertexBuffer->Unlock();

	return true;
}

DWORD cSquare::Render()
{
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matNextMove);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SQUARE_CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return 2;
}

void cSquare::Release() 
{
	SafeRelease(m_pd3dDevice);
}

// Use a matrix translation to move the square
// We translate the origin to the point
// equal to the new point, less the original starting point.
void cSquare::MoveTo(float xDistance, float yDistance)
{
	m_x += xDistance;
	m_y += yDistance;
	D3DXMATRIX matTrans;
	// Translates the origin x and y points from original spot
	D3DXMatrixTranslation(&matNextMove, m_x-m_xStart, m_y-m_yStart, 0.0f);
}

// Rotate the square around the given x and y point.
// 
// We recalculate the x and y co-ordinates of the square
// and move it to the difference between the current
// point and the new point. This means the squares themselves
// are NOT rotated, just moved to the new position
// relative to rotation of amount rads around the point.
void cSquare::Rotate(float xRot, float yRot, float* dx, float* dy, float dRads) 
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
