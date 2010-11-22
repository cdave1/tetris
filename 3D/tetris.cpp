#include "stdafx.h"

cGame*		g_pApp = NULL;
cBlock*		g_pBlock = NULL;
cPile*		g_pPile = NULL;
LPSTR		g_sNextMessage = "Press enter to start";
int			g_iScore = 0;
int			g_iLevel = 1;
int			g_iLevelRows = 0;
int			g_iTotalRows = 0;
DWORD		g_dwStopWatch = 0;
DWORD		g_dwTimeLimit = 0;
GAME_STATE	g_gameState = STOPPED;
const int	SCORE_INCREMENT = 50;


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	cGame game;
    // Register the window class
    if( FAILED( game.Create( hInst ) ) )
        return 0;

    return game.Run();
}

cGame::cGame() 
{
	g_pApp = this;
	m_3DRenderer = new c3DRenderer();
	g_pPile = new cPile();
}

HRESULT cGame::Create(HINSTANCE hInstance) 
{
   WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, StaticMsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "3DTetris", NULL };
    RegisterClassEx( &wc );

	// Create the application's window
    HWND hWnd = CreateWindow( "3DTetris", "3DTetris",
                              WS_OVERLAPPEDWINDOW, 50, 50, (int)TETRIS_AREA_WIDTH, (int)TETRIS_AREA_HEIGHT,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	
	if (NULL == hWnd) 
	{
		return E_FAIL;
	}

	UpdateWindow( hWnd );
	ShowWindow( hWnd, SW_SHOWDEFAULT );
 
    if(FAILED(Init(hInstance, hWnd)))
    {
       	CleanUp();
		UnregisterClass( "3DTetris", wc.hInstance );
		return E_FAIL; 
    }
	return S_OK;
}

INT cGame::Run() 
{
	// seed rand
	srand((unsigned)timeGetTime());
	StartGame();
	return GameLoop();
}

INT cGame::GameLoop() 
{
	BOOL bGotMsg;
    MSG  msg;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
    {
        // Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
        bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

        if( bGotMsg )
        {
            // Translate and dispatch the message
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		} else {
			switch (g_gameState) {
				case STOPPED:
					m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
					break;
				case RUNNING:
					// ProcessKeyboard();
					// Check if stop watch is at limit
					if ((timeGetTime() - g_dwStopWatch) >= g_dwTimeLimit) 
					{
						CompleteCycle();
					} 
					else 
					{
						m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
					}
					break;
				case PAUSED:
					g_sNextMessage = "PAUSED (press f1 to resume)";
					m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
					break;
				case GAMEOVER:
					g_sNextMessage = "Game over man.";
					g_gameState = STOPPED;
					m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
					break;
			}
		}
	}
	return (int)msg.wParam;
}

void cGame::StartGame()
{
	// Create first block
	if (g_pBlock == NULL) 
	{
		g_pBlock = new cBlock(m_3DRenderer->m_pd3dDevice);
	}
	if (g_pPile != NULL)
	{
		g_pPile->DestroyAllCubes();
	}
	g_pBlock->CreateCubes(0, TETRIS_AREA_WIDTH/2, TETRIS_AREA_HEIGHT+BLOCK_SQUARE_SIZE);
	// Initial time limit
	g_dwTimeLimit = 500;
	g_iScore = 0;
	g_iLevel = 1;
	g_iTotalRows = 0;
	g_iLevelRows = 0;

}

LRESULT CALLBACK StaticMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return g_pApp->MsgProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI cGame::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
		break;
		case WM_KEYUP:
			switch (g_gameState) 
			{
				case STOPPED:
					switch (wParam)
					{
						case VK_RETURN:
							StartGame();
							g_sNextMessage = (LPSTR)GetScoreMessage();
							g_dwStopWatch = timeGetTime();
							g_gameState = RUNNING;
							
							break;
					}
					break;
				case RUNNING:
					switch (wParam)
					{
						case VK_F1:
							g_gameState = PAUSED;
							break;
						case VK_DOWN:
							AttemptMove(0.0f, -BLOCK_SQUARE_SIZE);
							break;
						case VK_UP:
							AttemptRotate(D3DX_PI/2);
							break;
						case VK_RIGHT:
							AttemptMove(BLOCK_SQUARE_SIZE, 0.0f);
							break;
						case VK_LEFT:
							AttemptMove(-BLOCK_SQUARE_SIZE, 0.0f);
							break;
						case VK_SPACE:
							// Repeat move down until false.
							while(AttemptMove(0.0f, -BLOCK_SQUARE_SIZE))
							{
								m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
							}
							break;
						case VK_ESCAPE:
							DestroyWindow(hWnd);
							return 0;
						break;
						break;
					}
					break;
				case PAUSED:
					switch (wParam)
					{
						case VK_F1:
							g_gameState = RUNNING;
							break;
					}
			}
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

// Sets up keyboard, mouse, and our renderer
HRESULT cGame::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT res;
	// Create direct input interface.
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDirectInput, NULL)))
	{
		return false;
	}

	// ===== keyboard =====
	// Create keyboard device
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
	{
		CleanUp();
		return false;
	}

	// Keyboard data format
	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		CleanUp();
		return false;
	}

	// Keyboard co-op level
	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		CleanUp();
		return false;
	}

	// Acquire keyboard
	if (m_pKeyboard)
	{
		
		res = m_pKeyboard->Acquire();
		if (res);
	}

	// ===== mouse =====
	// Create mouse device
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		CleanUp();
		return false;
	}

	// Mouse data format
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
	{
		CleanUp();
		return false;
	}

	// Mouse co-op level
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		CleanUp();
		return false;
	}

	// Acquire mouse
	if (m_pMouse)
	{
		m_pMouse->Acquire();
	}

	//return //m_2DRenderer->InitD3D(hWnd) || m_3DRenderer->InitD3D(hWnd);
	return m_3DRenderer->InitD3D(hWnd);
}

void cGame::ProcessKeyboard()
{
	HRESULT hres;
	byte KeyboardState[256];

	hres = m_pKeyboard->GetDeviceState(sizeof(KeyboardState),(LPVOID)&KeyboardState);
	if (FAILED(hres))
	{
		return;
	}

	// Only worry about keyboard input when game is running
	if (RUNNING == g_gameState) 
	{
		if (KEYDOWN(KeyboardState, DIK_DOWN))
		{
			AttemptMove(0.0f, -BLOCK_SQUARE_SIZE);
		}
		else if (KEYDOWN(KeyboardState, DIK_F1))
		{
			g_gameState = PAUSED;
		}
		else if (KEYDOWN(KeyboardState, DIK_DOWN))
		{
			AttemptMove(0.0f, -BLOCK_SQUARE_SIZE);
		}
		else if (KEYDOWN(KeyboardState, DIK_UP))
		{
			AttemptRotate(D3DX_PI/2);
		}
		else if (KEYDOWN(KeyboardState, DIK_RIGHT))
		{
			AttemptMove(BLOCK_SQUARE_SIZE, 0.0f);
		}
		else if (KEYDOWN(KeyboardState, DIK_LEFT))
		{
			AttemptMove(-BLOCK_SQUARE_SIZE, 0.0f);
		}
		else if (KEYDOWN(KeyboardState, DIK_SPACE))
		{
			while(AttemptMove(0.0f, -BLOCK_SQUARE_SIZE))
			{
				m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
			}
		}
	}
}

void cGame::ProcessMouse()
{
}

void cGame::CompleteCycle()
{
	// Check if downward movement is restricted
	if (g_pBlock->GetBottomY() == BLOCK_SQUARE_SIZE)
	{
		NextCycle();
	} 
	else 
	{
		// Check if there is a collision with a downward movement.
		// If so, add to pile and restart.
		if (g_pPile->DetectMovementCollision(g_pBlock, 0.0f, -BLOCK_SQUARE_SIZE))
		{
			NextCycle();
		} 
		else 
		{
			// Move block down
			AttemptMove(0.0f, -BLOCK_SQUARE_SIZE);
		}
	}
	
	// Reset Stopwatch
	g_dwStopWatch = timeGetTime();
}

// Rotation "sticky" problem still unresolved:
// Basically, if the block has enough space to rotate
// it will do so until it moves adjacent to the pile.
// when it gets to the pile, it will come against
// the detect rotation collision checker and
// prevent further rotation, even if there is
// enough space for the block to rotate. The block
// will appear to "stick" to the pile (though x y 
// movement is still possible).
void cGame::AttemptRotate(float dRads)
{
	if (g_pPile->DetectRotationCollision(g_pBlock, dRads))
	{
		// collision detected - play a sound?
	} 
	else 
	{
		 
		// a rotation sometimes moves part of the block out of the play area
		// if the block is near one of the boundaries.
		g_pBlock->Rotate(dRads); 

		// if this happens, we want the rotation to happen,
		// but only if the movement amount does not collide with the pile.
		// if there is no collision, then move the block back into the playing
		// area by the amount that is OUT of the playing area by.
		// if there IS a collision, then reverse the rotation.
		if (g_pBlock->GetRightX() > TETRIS_AREA_WIDTH-BLOCK_SQUARE_SIZE) 
		{
			if (FALSE == AttemptMove((TETRIS_AREA_WIDTH-BLOCK_SQUARE_SIZE)-g_pBlock->GetRightX(), 0.0f))
			{
				AttemptRotate((D3DX_PI*2) - dRads);
			}
		}
		if (g_pBlock->GetLeftX() < 0.0f)
		{
			if (FALSE == AttemptMove(0.0f-g_pBlock->GetLeftX(), 0.0f))
			{
				AttemptRotate((D3DX_PI*2) - dRads);
			}
		}
		if (g_pBlock->GetBottomY() <= BLOCK_SQUARE_SIZE) 
		{
			// move up
			AttemptMove(0.0f, BLOCK_SQUARE_SIZE-g_pBlock->GetBottomY());
		}
	}
}

bool cGame::AttemptMove(float dx, float dy)
{
	if (g_pPile->DetectMovementCollision(g_pBlock, dx, dy))
	{
		// collision detected - play a sound?
		return false;
	} 
	else 
	{
		// Move only if the new position is within the play area
		if ((g_pBlock->GetBottomY()+dy >= BLOCK_SQUARE_SIZE) 
					&& (g_pBlock->GetRightX()+dx <= TETRIS_AREA_WIDTH-BLOCK_SQUARE_SIZE)
					&& (g_pBlock->GetLeftX()+dx >= 0.0f))
		{
			g_pBlock->Move(dx, dy);
		} 
		else 
		{
			return false;
		}
	}
	return true;
}


// Must do several things:
// - Adds the current block to the pile
// - checks for "game over" state and ends game if so.
// - Destroys any lines that needs destroying
// - Increments the score
// - Moves to the next level
// - Creates the next block. 
void cGame::NextCycle()
{
	if (g_pBlock != NULL) 
	{ 
		g_pPile->AddBlock(g_pBlock);
		int rows = 0;
		g_pPile->DestroyRows(&rows);
		
		// See spec for scoring equation.
		if (rows > 0) {
			g_iScore += (SCORE_INCREMENT*(rows*rows))-(SCORE_INCREMENT*rows)+SCORE_INCREMENT;
			g_iLevelRows += rows;
			g_iTotalRows += rows;
		}

		// on level up:
		if (g_iLevelRows >= 10) 
		{
			g_iLevel++;
			g_iLevelRows = 0;
			g_dwTimeLimit -= g_dwTimeLimit/12;
		}

		g_sNextMessage = (LPSTR)GetScoreMessage();

		if (g_pPile->GetTopY() >= TETRIS_AREA_HEIGHT+BLOCK_SQUARE_SIZE) 
		{
			StopGame();
		} 
		else 
		{
			// Create next block
			g_pBlock->CreateCubes((rand()%7), TETRIS_AREA_WIDTH/2, TETRIS_AREA_HEIGHT+BLOCK_SQUARE_SIZE);
			m_3DRenderer->Render(g_sNextMessage, g_pBlock, g_pPile);
		}

	}
}

// Stops the game
void cGame::StopGame()
{
	g_pPile->AddBlock(g_pBlock);
	g_gameState = GAMEOVER;
	//g_pBlock = NULL;
}
	

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID cGame::CleanUp()
{	
}

const char* cGame::GetScoreMessage() 
{
	char* buffer;
	buffer = (char *)malloc(50 * sizeof(char));
	sprintf(buffer, "Score: %d  Level: %d\n", g_iScore, g_iLevel);
	return buffer;
}
