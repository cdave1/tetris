#pragma once
class cBlock;
class cSquare;
const int SQUARE_PILE_SIZE = 200;
class cPile
{
public:
	cPile();
	void Render();
	void AddBlock(cBlock* block);
	bool DetectMovementCollision(cBlock* block, float dx, float dy);
	bool DetectRotationCollision(cBlock* block, float dRads);
	bool DetectOverlap(cSquare* square, float dx, float dy);
	void DestroyRows(int* lineCount);
	void DestroySquares(float yPos);
	void DestroyAllSquares();
	float GetTopY();

	cSquare* m_SquarePile[SQUARE_PILE_SIZE];
	int m_pileSize;
private:
	float m_fTopY;
};
