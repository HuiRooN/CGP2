////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct ModelCount
	{
		int vertexCount;
		int textureCount;
		int normalCount;
		int faceCount;
	};

	struct OBJType
	{
		float x, y, z;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, const wchar_t*);
	bool Initialize(ID3D11Device*, const char*, const wchar_t*);
	bool InitializeSkymap(ID3D11Device*, const char*, const wchar_t*);
	bool InitializePlane(ID3D11Device*, const wchar_t*, int, int);
	bool InitializeObj(ID3D11Device*, const char*, const wchar_t*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(const char*);
	void ReleaseModel();

	bool LoadObjModel(const char*);

	bool ReadFileCounts(const char*, int&, int&, int&, int&);
	bool LoadDataStructures(const char*, int, int, int, int);

private:
	bool InitializeBuffers(ID3D11Device*);
	bool InitializeBuffersForTex(ID3D11Device*);
	bool InitializeBuffersForSkymap(ID3D11Device*);
	bool InitializeBuffersForPlane(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const wchar_t*);
	bool LoadSkyTexture(ID3D11Device*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	TextureClass* m_SkyTexture;

	ModelType* m_model;
};

#endif