#pragma once

#define KEYDOWN(name, key) (name[key] & 0x80)
#define KEYUP(name, key) (name[key]

LRESULT CALLBACK StaticMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
class c3DRenderer;
class CBlock;
class cPile;

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
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	INT     Run();
	LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void ProcessKeyboard();
	void ProcessMouse();

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
	LPDIRECTINPUTDEVICE8 m_pKeyboard;
	LPDIRECTINPUTDEVICE8 m_pMouse;
	LPDIRECTINPUT8 m_pDirectInput;
	c3DRenderer* m_3DRenderer;
};