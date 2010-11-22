#pragma once

LRESULT CALLBACK StaticMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
class CDirectx;
class CBlock;
class cPile;

enum MOVEMENT_TYPE
{
	NONE = 0,
	DOWN = 1,
	UP = 2,
	RIGHT = 4,
	LEFT = 8,
	ROTATECW = 16,
	ROTATECCW = 32
};

enum GAME_STATE
{
	STOPPED = 0,
	RUNNING = 1,
	PAUSED = 2,
	GAMEOVER = 4
};

class cGame
{
public:
	cGame();
	HRESULT Create( HINSTANCE hInstance );
	HRESULT Init(HWND hWnd);
	INT     Run();
	LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	INT GameLoop();
	void StartGame();
	void NextCycle();
	void CompleteCycle();
	bool AttemptMove(float dx, float dy);
	void AttemptRotate(float dRads);
	void UpdateGame(MSG* msg);
	void StopGame();
	VOID CleanUp();
	void SetupData();
	const char* GetScoreMessage();
private:
	
	CDirectx* gfx;
};