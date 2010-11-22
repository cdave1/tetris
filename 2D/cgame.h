#pragma once

class cGame
{
public:
	cGame();
	HRESULT StartGame(HWND hWnd);
	void NextCycle();
	void UpdateGame(MSG* msg);
	VOID CleanUp();
	void SetupData();
private:
	CDirectx* gfx;
	MSG* m_msg;
};
