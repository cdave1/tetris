#pragma once
class cBlock;
class cCube;
const int SQUARE_PILE_SIZE = 200;
class cPile
{
public:
	cPile();
	void Render();
	void AddBlock(cBlock* block);
	bool DetectMovementCollision(cBlock* block, float dx, float dy);
	bool DetectRotationCollision(cBlock* block, float dRads);
	bool DetectOverlap(cCube* cube, float dx, float dy);
	void DestroyRows(int* lineCount);
	void DestroyCubes(float yPos);
	void DestroyAllCubes();
	float GetTopY();

	cCube* m_CubePile[SQUARE_PILE_SIZE];
	int m_pileSize;
private:
	float m_fTopY;
};
