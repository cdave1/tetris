#pragma once

class cFont
{
public:
	cFont(LPDIRECT3DDEVICE9 pd3dDevice, LPSTR pFontFace, int nHeight, bool fBold,
		bool fItalic, bool fUnderlined);
	~cFont();
	void DrawFontText(LPSTR pText, int x, int y);
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXFONT m_pFont;
};
