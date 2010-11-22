#include "stdafx.h"

cFont::cFont(LPDIRECT3DDEVICE9 pd3dDevice, LPSTR pFontFace, int nHeight, bool fBold,
		bool fItalic, bool fUnderlined)
{
	HFONT hFont;
	m_pd3dDevice = pd3dDevice;

	int nWeight = FW_NORMAL;
	DWORD dwItalic = 0;
	DWORD dwUnderlined = 0;

	if(fBold)
	{
		nWeight = FW_BOLD;
	}

	if (fItalic) 
	{
		dwItalic = 1;
	}

	if (fUnderlined) 
	{
		dwUnderlined = 1;
	}

	hFont = CreateFont(nHeight, 0, 0, 0, nWeight, dwItalic, dwUnderlined, 0,
		ANSI_CHARSET, 0, 0, 0, 0, pFontFace);

	HRESULT h = D3DXCreateFont(m_pd3dDevice, hFont, &m_pFont);
}

cFont::~cFont()
{
	SafeRelease(m_pFont);
}

void cFont::DrawFontText(LPSTR pText, int x, int y)
{
	RECT Rect;
	Rect.left = x;
	Rect.top = y;
	Rect.right = 0;
	Rect.bottom = 0;

	HRESULT h1 = m_pFont->Begin();
	HRESULT h2 = m_pFont->DrawText(pText, -1, &Rect, DT_CALCRECT, 0);
	HRESULT h3 = m_pFont->DrawText(pText, -1, &Rect, DT_LEFT, 0xffffffff);
	HRESULT h4 = m_pFont->End();
}
