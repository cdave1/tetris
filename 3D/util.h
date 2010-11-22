// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define SQUAREFVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define TEXTUREFVF (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define SafeRelease(pObject) if (pObject != NULL) {pObject->Release(); pObject=NULL;}

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }