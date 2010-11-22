#include <d3dx9.h>

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z; // The transformed position for the vertex
    FLOAT nx, ny, nz; // Lighting normals
	FLOAT tu, tv; // - units across, units down
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

class Cuboid
{
public:
	float w,h,d,x,y,z;
	char* szTextureFilePath;
	Cuboid() {}

	Cuboid(float fw, float fh, float fd, float fx, float fy, float fz, char *texFilePath)
	{
		w=fw, h=fh, d=fd, x=fx, y=fy, z=fz;
		szTextureFilePath = texFilePath;
/*
		D3DCOLORVALUE rgbaDiffuse = {1.0, 1.0, 1.0, 0.0,};
		D3DCOLORVALUE rgbaAmbient = {1.0, 1.0, 1.0, 0.0,};
		D3DCOLORVALUE rgbaSpecular = {0.0, 0.0, 0.0, 0.0,};
		D3DCOLORVALUE rgbaEmissive = {0.0, 0.0, 0.0, 0.0,};
		SetMaterial(rgbaDiffuse, rgbaAmbient, rgbaSpecular, rgbaEmissive, 0);
		*/
		m_dwNumOfIndices = 36;
		m_dwNumOfVertices = 24;
		m_dwNumOfPolygons = 12;
	}

	void SetPosition(float fx, float fy, float fz) 
	{
		x=fx, y=fy, z=fz;
	}

	HRESULT CreateVertexBuffer(LPDIRECT3DDEVICE9 pDevice)
	{
		if(FAILED(D3DXCreateTextureFromFile(pDevice, szTextureFilePath, &pTexture))) 
		{
			return false;
		}
		

		if( FAILED( pDevice->CreateVertexBuffer( m_dwNumOfVertices*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL ) ) )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT CreateIndexBuffer(LPDIRECT3DDEVICE9 pDevice) 
	{
		VOID* pBufferIndices;

		if (FAILED(pDevice->CreateIndexBuffer(m_dwNumOfIndices * sizeof(WORD), 0,
												D3DFMT_INDEX16, D3DPOOL_MANAGED,
												&m_pIndexBuffer, NULL)))
		{
			return false;
		}

		WORD pIndices[] = { 0, 1, 2, 3, 2, 1, //TOP
							4, 5, 6, 7, 6, 5, // Face 1
							8, 9,10,11,10, 9, // Face 2
						   12,13,14,15,14,13, // Face 3
						   16,17,18,19,18,17, // Face 4
						   20,21,22,23,22,21 };

		m_pIndexBuffer->Lock(0, m_dwNumOfIndices * sizeof(WORD),
							(void**)&pBufferIndices, 0);

		memcpy(pBufferIndices, pIndices, m_dwNumOfIndices * sizeof(WORD));

		m_pIndexBuffer->Unlock();

		return true;
	}

	bool SetMaterial(D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient,
		D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower)
	{
		m_matMaterial.Diffuse = rgbaDiffuse;
		m_matMaterial.Ambient = rgbaAmbient;
		m_matMaterial.Specular = rgbaSpecular;
		m_matMaterial.Power = rPower;
		m_matMaterial.Emissive = rgbaEmissive;

		return true;
	}

	HRESULT UpdateVertices(LPDIRECT3DDEVICE9 pDevice) 
	{
		DWORD i;
		VOID* pVertices;
		WORD* pBufferIndices;
		D3DXVECTOR3 vNormal;
		DWORD dwVertex1, dwVertex2, dwVertex3;

		// This array holds the number of times a vertex is shared.
		WORD* pNumOfSharedPolygons = new WORD[m_dwNumOfVertices];

		// This array holds the sum of all face normals for the shared vertex
		D3DVECTOR* pSumVertexNormal = new D3DVECTOR[m_dwNumOfVertices];

		// Clear the mem
		for (i = 0; i < m_dwNumOfVertices; i++)
		{
			pNumOfSharedPolygons[i] = 0;
			pSumVertexNormal[i] = D3DXVECTOR3(0,0,0);
		}
		
		CUSTOMVERTEX vertices[] =
		{
			// Top Face
			{x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,},
			{x, y, z+d, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
			{x+w, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,},
			{x+w, y, z+d, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,},

			// Face 1
			{x, y-h, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,},
			{x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
			{x+w, y-h, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,},
			{x+w, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,},

			// Face 2
			{x+w, y-h, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,},
			{x+w, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,},
			{x+w, y-h, z+d, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,},
			{x+w, y, z+d, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},

			// Face 3
			{x+w, y-h, z+d, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,},
			{x+w, y, z+d, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
			{x, y-h, z+d, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,},
			{x, y, z+d, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},

			// Face 4
			{x, y-h, z+d, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,},
			{x, y, z+d, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
			{x, y-h, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,},
			{x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},

			// Bottom Face
			{x+w, y-h, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
			{x+w, y-h, z+d, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,},
			{x, y-h, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,},
			{x, y-h, z+d, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
		};

		m_pIndexBuffer->Lock(0, m_dwNumOfIndices * sizeof(WORD),
			(void**)&pBufferIndices, D3DLOCK_READONLY);

		for (i = 0; i < m_dwNumOfIndices; i +=3)
		{
			dwVertex1 = pBufferIndices[i];
			dwVertex2 = pBufferIndices[i+1];
			dwVertex3 = pBufferIndices[i+2];

			vNormal = GetTriangleNormal(&D3DXVECTOR3(vertices[dwVertex1].x,
													 vertices[dwVertex1].y,
													 vertices[dwVertex1].z),
									    &D3DXVECTOR3(vertices[dwVertex2].x,
													 vertices[dwVertex2].y,
													 vertices[dwVertex2].z),
									    &D3DXVECTOR3(vertices[dwVertex3].x,
													 vertices[dwVertex3].y,
													 vertices[dwVertex3].z));
			pNumOfSharedPolygons[dwVertex1]++;
			pNumOfSharedPolygons[dwVertex2]++;
			pNumOfSharedPolygons[dwVertex3]++;

			pSumVertexNormal[dwVertex1].x += vNormal.x;
			pSumVertexNormal[dwVertex1].y += vNormal.y;
			pSumVertexNormal[dwVertex1].z += vNormal.z;

			pSumVertexNormal[dwVertex2].x += vNormal.x;
			pSumVertexNormal[dwVertex2].y += vNormal.y;
			pSumVertexNormal[dwVertex2].z += vNormal.z;

			pSumVertexNormal[dwVertex3].x += vNormal.x;
			pSumVertexNormal[dwVertex3].y += vNormal.y;
			pSumVertexNormal[dwVertex3].z += vNormal.z;
			
		}	

		m_pIndexBuffer->Unlock();

		for (i = 0; i < m_dwNumOfVertices; i++)
		{
			vNormal.x = pSumVertexNormal[i].x / pNumOfSharedPolygons[i];
			vNormal.y = pSumVertexNormal[i].y / pNumOfSharedPolygons[i];
			vNormal.z = pSumVertexNormal[i].z / pNumOfSharedPolygons[i];

			D3DXVec3Normalize(&vNormal, &vNormal);

			vertices[i].nx = vNormal.x;
			vertices[i].ny = vNormal.y;
			vertices[i].nz = vNormal.z;
		}

		if (FAILED(m_pVertexBuffer->Lock(0, sizeof(vertices), (VOID**)&pVertices, 0)))
		{
			return false;
		}

		memcpy(pVertices, vertices, sizeof(vertices));

		m_pVertexBuffer->Unlock();

		delete pNumOfSharedPolygons;
		delete pSumVertexNormal;

		pNumOfSharedPolygons = NULL;
		pSumVertexNormal = NULL;

		return true;
	}

	D3DVECTOR GetTriangleNormal(D3DXVECTOR3* vVertex1, D3DXVECTOR3* vVertex2,
		D3DXVECTOR3* vVertex3)
	{
		D3DXVECTOR3 vNormal, v1, v2;

		D3DXVec3Subtract(&v1, vVertex2, vVertex1);
		D3DXVec3Subtract(&v2, vVertex3, vVertex1);

		D3DXVec3Cross(&vNormal, &v1, &v2);

		D3DXVec3Normalize(&vNormal, &vNormal);

		return vNormal;
	}

	void Render(LPDIRECT3DDEVICE9 pd3d)
	{
//		pd3d->SetMaterial(&m_matMaterial);

	
		if (pTexture != NULL)
		{
			pd3d->SetTexture(0, pTexture);
			pd3d->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		}
		else 
		{
			pd3d->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		}
		
		
		
		pd3d->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );
		pd3d->SetFVF(D3DFVF_CUSTOMVERTEX);

		pd3d->SetIndices(m_pIndexBuffer);
		pd3d->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_dwNumOfVertices, 0, 
			0, m_dwNumOfPolygons);
	}

	void CleanUp() 
	{
		if( m_pVertexBuffer != NULL )        
			m_pVertexBuffer->Release();
	}

private:
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	DWORD m_dwNumOfIndices;
	DWORD m_dwNumOfVertices;
	DWORD m_dwNumOfPolygons;
	D3DMATERIAL9 m_matMaterial;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 pTexture;
};
