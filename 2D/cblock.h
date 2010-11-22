#pragma once
class cSquare;



class cBlock
{
public:
	cBlock (LPDIRECT3DDEVICE9 pDevice);
	void Release();
	void CreateSquares(int sBlockType, float xStart, float yStart);
	void Move(float dx, float dy);
	void Rotate(float dRads);
	void Render();
	float GetTopY();
	float GetBottomY();
	float GetRightX();
	float GetLeftX();
	
	cSquare* m_arrSquares[4];
	float m_xCentre;
	float m_yCentre;
private:

	LPDIRECT3DDEVICE9 m_pd3dDevice;
};