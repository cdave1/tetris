#include "stdafx.h"

cBlock::cBlock (LPDIRECT3DDEVICE9 pDevice) 
{
	m_pd3dDevice = pDevice;
}

void cBlock::CreateSquares(int sBlockType, float xStart, float yStart) 
{
	int i;
	DWORD color = 0;
	switch(sBlockType) 
	{
		// long block
		case 0:
			color = D3DXCOLOR(0x00ff0000); //red
			for(i = 0; i < 4; i++) {
				m_arrSquares[i] = new cSquare(m_pd3dDevice, xStart, yStart + (i * -BLOCK_SQUARE_SIZE), color);
			}
			m_xCentre = xStart;
			m_yCentre = yStart - (2.0f * BLOCK_SQUARE_SIZE);
			break;
		// square block
		case 1:
			color = D3DXCOLOR(0xffff00ff); // purple
			m_arrSquares[0] = new cSquare(m_pd3dDevice, xStart, yStart, color);
			m_arrSquares[1] = new cSquare(m_pd3dDevice, xStart, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[2] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart, color);
			m_arrSquares[3] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - BLOCK_SQUARE_SIZE, color);
			m_xCentre = xStart + BLOCK_SQUARE_SIZE;
			m_yCentre = yStart - BLOCK_SQUARE_SIZE;
			break;
		// > block
		case 2:
			color = D3DXCOLOR(0xffffff00); // orange
			m_arrSquares[0] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart, color);
			m_arrSquares[1] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[2] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - (2*BLOCK_SQUARE_SIZE), color);
			m_arrSquares[3] = new cSquare(m_pd3dDevice, xStart, yStart - (2*BLOCK_SQUARE_SIZE), color);
			m_xCentre = xStart + (1.5f*BLOCK_SQUARE_SIZE);
			m_yCentre = yStart - (1.5f*BLOCK_SQUARE_SIZE);
			break;
		// < block
		case 3:
			color = D3DXCOLOR(0xff009900); // green
			m_arrSquares[0] = new cSquare(m_pd3dDevice, xStart, yStart, color);
			m_arrSquares[1] = new cSquare(m_pd3dDevice, xStart, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[2] = new cSquare(m_pd3dDevice, xStart, yStart - (2*BLOCK_SQUARE_SIZE), color);
			m_arrSquares[3] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - (2*BLOCK_SQUARE_SIZE), color);
			m_xCentre = xStart + (0.5f*BLOCK_SQUARE_SIZE);
			m_yCentre = yStart - (1.5f*BLOCK_SQUARE_SIZE);
			break;
		// s block
		case 4:
			color = D3DXCOLOR(0xff00cccc); // aqua
			m_arrSquares[0] = new cSquare(m_pd3dDevice, xStart, yStart, color);
			m_arrSquares[1] = new cSquare(m_pd3dDevice, xStart, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[2] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[3] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - (2*BLOCK_SQUARE_SIZE), color);
			m_xCentre = xStart + BLOCK_SQUARE_SIZE;
			m_yCentre = yStart - BLOCK_SQUARE_SIZE;
			break;
		// z block
		case 5:
			color = D3DXCOLOR(0xff3366ff); // blue
			m_arrSquares[0] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart, color);
			m_arrSquares[1] = new cSquare(m_pd3dDevice, xStart, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[2] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - BLOCK_SQUARE_SIZE, color);
			m_arrSquares[3] = new cSquare(m_pd3dDevice, xStart, yStart - (2*BLOCK_SQUARE_SIZE), color);
			m_xCentre = xStart + BLOCK_SQUARE_SIZE;
			m_yCentre = yStart - BLOCK_SQUARE_SIZE;
			break;
		// t block
		case 6:
			color = D3DXCOLOR(0xffffffff); // white
			m_arrSquares[0] = new cSquare(m_pd3dDevice, xStart, yStart, color);
			m_arrSquares[1] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart, color);
			m_arrSquares[2] = new cSquare(m_pd3dDevice, xStart + (2*BLOCK_SQUARE_SIZE), yStart, color);
			m_arrSquares[3] = new cSquare(m_pd3dDevice, xStart + BLOCK_SQUARE_SIZE, yStart - BLOCK_SQUARE_SIZE, color);
			m_xCentre = xStart + (1.5f*BLOCK_SQUARE_SIZE);
			m_yCentre = yStart - (BLOCK_SQUARE_SIZE/2);
			break;
		default:
			break;
	}
}

void cBlock::Render() 
{
	for (int i = 0; i < 4; i++)
	{
		m_arrSquares[i]->Render();
	}
}

void cBlock::Move(float dx, float dy)
{
	if (m_arrSquares != NULL) 
	{
		for(int i = 0; i < 4; i++) {
			m_arrSquares[i]->MoveTo(dx, dy);
		}
	}
	m_xCentre += dx;
	m_yCentre += dy;
}

// Calculates the new position for each square
// after rotation around the block's center point,
// and then moves the square to that position.
void cBlock::Rotate(float dRads)
{
	float dx = 0.0f, dy = 0.0f;
	if (m_arrSquares != NULL) 
	{
		for(int i = 0; i < 4; i++) 
		{
			m_arrSquares[i]->Rotate(m_xCentre, m_yCentre, &dx, &dy, dRads);
			m_arrSquares[i]->MoveTo(dx, dy);
		}
	}
}

void cBlock::Release()
{
	delete[] m_arrSquares;
}

float cBlock::GetBottomY()
{
	float min = TETRIS_AREA_HEIGHT;
	for(int i = 0; i < 4; i++) 
	{
		if (m_arrSquares[i]->m_y < min) 
		{
			min = m_arrSquares[i]->m_y;
		}
	}
	return min;
}

float cBlock::GetTopY()
{
	float max = 0.0f;
	for(int i = 0; i < 4; i++) 
	{
		if (m_arrSquares[i]->m_y > max) 
		{
			max = m_arrSquares[i]->m_y;
		}
	}
	return max;
}

float cBlock::GetLeftX()
{
	float min = TETRIS_AREA_WIDTH;
	for(int i = 0; i < 4; i++) 
	{
		if (m_arrSquares[i]->m_x < min) 
		{
			min = m_arrSquares[i]->m_x;
		}
	}
	return min;
}

float cBlock::GetRightX()
{
	float max = 0.0f;
	for(int i = 0; i < 4; i++) 
	{
		if (m_arrSquares[i]->m_x > max) 
		{
			max = m_arrSquares[i]->m_x;
		}
	}
	return max;
}

