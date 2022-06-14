#pragma once

#include <vector>
#include <d3d11.h>
#include <D3DX10math.h>

#include "GameObject.h"

class D3DClass;
class TextureShaderClass;

class SkyboxClass : public GameObject
{
public:
	struct MatrixBufferType
	{
		D3DXMATRIX WVP;
		D3DXMATRIX World;
	};

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v)
			: position(x, y, z), tex(u, v){}

		D3DXVECTOR3 position;
		D3DXVECTOR2 tex;
	};

public:
	SkyboxClass();
	SkyboxClass(const SkyboxClass&);
	~SkyboxClass();

public:
	void InitializeSkybox(ID3D11Device*);
	void CreateSphere(D3DClass*, int, int);
	void CreateShader(D3DClass*, HWND hwnd);
	void Render(D3DClass*, TextureShaderClass*, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR3);

private:
	virtual bool Frame(float);
	void Shutdown();

private:
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;

	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11InputLayout* vertLayout;
	ID3D11Buffer* matrixBuffer;

	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11ShaderResourceView* smrv;
	ID3D11DepthStencilState* DSLessEqual;

	int NumSphereVertices;
	int NumSphereFaces;

	D3DXMATRIX Rotationx;
	D3DXMATRIX Rotationy;
	D3DXMATRIX Rotationz;

	ModelClass* m_lf;
	ModelClass* m_up;
	ModelClass* m_dn;
	ModelClass* m_ft;
	ModelClass* m_bk;
};

