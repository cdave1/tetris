#include "stdafx.h"

// Pile is basically a collection of cSquare objects.
cPile::cPile()
{
	m_pileSize = 0;
	for (int i = 0; i < SQUARE_PILE_SIZE; i++)
	{
		m_SquarePile[i] = NULL;
	}
}

void cPile::Render()
{
	for (int i = 0; i < m_pileSize; i++) 
	{		
		if (m_SquarePile[i] != NULL) 
		{
			m_SquarePile[i]->Render();
		}
	}
}

// Checks if the block collides with the pile when
// rotated by dRads.
bool cPile::DetectRotationCollision(cBlock* block, float dRads)
{
	float dx = 0.0f, dy = 0.0f;
	if (block == NULL) 
	{
		return false;
	} 
	for (int i = 0; i < 4; i++) 
	{
		block->m_arrSquares[i]->Rotate(block->m_xCentre, block->m_yCentre, &dx, &dy, dRads);
		if (DetectOverlap(block->m_arrSquares[i], dx, dy))
		{
			return true;
		}
	}
	return false;
}

// Check if the block collides with anything in the pile 
// when moved in a dx or dy direction.
bool cPile::DetectMovementCollision(cBlock* block, float dx, float dy)
{
	if (block == NULL) 
	{
		return false;
	} 
	else 
	{
		for (int i = 0; i < 4; i++) 
		{
			if(DetectOverlap(block->m_arrSquares[i], dx, dy))
			{
				return true;
			}
		}
	}
	return false;
}

// Detects if the movement of the square by dx and dy
// will overlap any square in the pile.
//
// PROBLEM: rotation may bring about both a dx and dy change
// if that is the case, then this method won't register an
// overlap since we are only worried about squares in the 
// same x and y axes.
bool cPile::DetectOverlap(cSquare* square, float dx, float dy)
{
	int i;
	for (i = 0; i < m_pileSize; i++)
	{
		if ((square->m_x + dx) == m_SquarePile[i]->m_x)
		{
			if (square->m_y+dy == m_SquarePile[i]->m_y) 
			{
				return true;
			}
		}
		if ((square->m_y + dy) == m_SquarePile[i]->m_y)
		{
			if (square->m_x+dx == m_SquarePile[i]->m_x) 
			{
				return true;
			}
		}
	}
	return false;
}

// Recursively destroys lines.
// Checks if there are any completed rows. If so,
// destroys all the squares in the row and repeats
// the call with the linecount number.
//
// Recursion is needed because we need to move all rows down one place
// for each destroyed row, and then recheck if any further 
// destroying is needed. This is to get around the problem of have
// two or more non-adjacent rows destroyed and not having them move
// down enough because we don't know how far they are from the 
// row that was just destroyed. 
//
// There might be a more efficient way of doing this. For example
// we recount the number of squares in each line with each successive call
// which is a bit inefficient because we only really need to know this once. 
// The ultimate way would be to retain a lookup of squares by x and y 
// co-ordinates, so we don't need to count the number of squares in each row.
void cPile::DestroyRows(int* lineCount)
{
	int rows = (int)(TETRIS_AREA_HEIGHT/BLOCK_SQUARE_SIZE);
	int i = 0, row = 0;

	int* lines = new int[rows];
	for (i = 0; i < rows; i++) 
		lines[i] = 0;

	// Works out how many squares are in each row.
	for (i = 0; i < m_pileSize; i++) 
	{
		row = ((int)(m_SquarePile[i]->m_y/BLOCK_SQUARE_SIZE))-1;
		lines[row] += 1;
	}

	for (i = 0; i < rows; i++) 
	{
		if (lines[i] >= 10)
		{
			DestroySquares((i+1)*BLOCK_SQUARE_SIZE);
			*lineCount += 1;

			// move all squares above the destroyed line down one position.
			for (int j = 0; j < m_pileSize; j++) 
			{
				if (m_SquarePile[j]->m_y >= (i+1)*BLOCK_SQUARE_SIZE)
				{
					m_fTopY -= BLOCK_SQUARE_SIZE;
					m_SquarePile[j]->MoveTo(0.0f, -BLOCK_SQUARE_SIZE);
				}
			}
			DestroyRows(lineCount);
			break;
		}
	}
}

// Destroys any squares at the supplied y position
void cPile::DestroySquares(float yPos)
{
	for (int i = 0; i < m_pileSize; i++) 
	{
		if (m_SquarePile[i] != NULL) {
			if (yPos == m_SquarePile[i]->m_y) 
			{
				SafeRelease(m_SquarePile[i]);
				m_SquarePile[i] = NULL;
				
				// Move all the blocks in the pile array left one position.
				for (int j = i; j < m_pileSize; j++) {
					m_SquarePile[j] = m_SquarePile[j+1];
					m_SquarePile[j+1] = NULL;
				}
				m_pileSize = m_pileSize - 1;
				i -= 1;
			}
		}
	}
}

// Destroys all squares in the pile
void cPile::DestroyAllSquares()
{
	for (int i = 0; i < m_pileSize; i++) 
	{
		SafeRelease(m_SquarePile[i]);
		m_SquarePile[i] = NULL;
	}
	m_pileSize = 0;
}

// Adds the block to the pile
void cPile::AddBlock(cBlock* block) 
{
	m_fTopY = block->GetTopY();
	for (int i = 0; i < 4; i++) 
	{
		if (block->m_arrSquares[i] != NULL) {
			m_SquarePile[m_pileSize++] = block->m_arrSquares[i];
		}
	}
}

float cPile::GetTopY()
{
	return m_fTopY;
}
