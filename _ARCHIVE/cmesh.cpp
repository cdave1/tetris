#include <d3dx9.h>

// A structure for our custom vertex type
struct MESHCUSTOMVERTEX
{
    FLOAT x, y, z; // The transformed position for the vertex
    FLOAT nx, ny, nz;        // Normals
	FLOAT tu, tv; // - units across, units down
};

// Our custom FVF, which describes our custom vertex structure
#define MESH_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

#define SafeRelease(pObject) if (pObject != NULL) {pObject->Release(); pObject=NULL;}
#define SafeDelete(pObject) if (pObject != NULL) {pObject=NULL;}

class Mymesh 
{
public:
	Mymesh(LPDIRECT3DDEVICE9 pD3DDevice, LPSTR pFilename)
	{
		LPD3DXBUFFER materialsBuffer = NULL;
		LPD3DXMESH mesh = NULL;

		pd3d = pD3DDevice;

		// second null is D3DXEFFECTINSTANCE.
		if (FAILED(D3DXLoadMeshFromX(pFilename, D3DXMESH_SYSTEMMEM, pd3d, NULL,
			&materialsBuffer, NULL, &dwNumMaterials, &mesh))) 
		{
			// null everything
			pMesh = NULL;
			pMeshMaterials = NULL;
			pMeshTextures = NULL;

			return;
		}

		D3DXMATERIAL* matMaterials = (D3DXMATERIAL*)materialsBuffer->GetBufferPointer();

		pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
		pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

		for (DWORD i = 0; i < dwNumMaterials; i++) 
		{
			pMeshMaterials[i] = matMaterials[i].MatD3D;

			// ambient color
			pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

			if (FAILED(D3DXCreateTextureFromFile(pd3d, 
				matMaterials[i].pTextureFilename, 
				&pMeshTextures[i])))
			{
				pMeshTextures[i] = NULL;
			}
		}
		SafeRelease(materialsBuffer);

		mesh->CloneMeshFVF(D3DXMESH_MANAGED, MESH_D3DFVF_CUSTOMVERTEX, pd3d, &pMesh);
		SafeRelease(mesh);

		D3DXComputeNormals(pMesh,0);
	}

	~Mymesh()
	{
		
		SafeDelete(pMeshMaterials);
		if (pMeshTextures != NULL) 
		{
			for (DWORD i = 0; i < dwNumMaterials; i++)
			{
				if (pMeshTextures[i])
				{
					SafeRelease(pMeshTextures[i]);
				}
			}
		}
		SafeDelete(pMeshTextures);
	}

	DWORD Render()
	{
		if (pMesh != NULL)
		{
			for (DWORD i = 0; i < dwNumMaterials; i++)
			{
				pd3d->SetMaterial(&pMeshMaterials[i]);
				pd3d->SetTexture(0, pMeshTextures[i]);
				pMesh->DrawSubset(i);
			}
			return pMesh->GetNumFaces();
		}
		else
		{
			return 0;
		}
	}

	void CleanUp() 
	{
		
	}

private:
	DWORD dwNumMaterials;
	LPDIRECT3DDEVICE9 pd3d;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	LPD3DXMESH pMesh;
};
