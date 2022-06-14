#include "SkyboxClass.h"

#include "d3dclass.h"
#include "textureshaderclass.h"

SkyboxClass::SkyboxClass()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 500.f, 500.f, 500.f);

	m_matWorld = matScale * m_matWorld;

	m_eTag = TAG_SKYBOX;
}

SkyboxClass::SkyboxClass(const SkyboxClass &)
{
}

SkyboxClass::~SkyboxClass()
{
	Shutdown();
}

void SkyboxClass::InitializeSkybox(ID3D11Device *device)
{
	m_lf = new ModelClass;
	m_lf->InitializePlane(device, L"../Engine/data/Skybox/midnight-silence_lf.png", 100, 100);

	m_Model = new ModelClass;
	m_Model->InitializePlane(device, L"../Engine/data/Skybox/midnight-silence_rt.png", 100, 100);

	m_up = new ModelClass;
	m_up->InitializePlane(device, L"../Engine/data/Skybox/midnight-silence_up.png", 100, 100);

	m_dn = new ModelClass;
	m_dn->InitializePlane(device, L"../Engine/data/Skybox/midnight-silence_dn.png", 100, 100);

	m_ft = new ModelClass;
	m_ft->InitializePlane(device, L"../Engine/data/Skybox/midnight-silence_ft.png", 100, 100);

	m_bk = new ModelClass;
	m_bk->InitializePlane(device, L"../Engine/data/Skybox/midnight-silence_bk.png", 100, 100);
}

void SkyboxClass::CreateSphere(D3DClass* d3d, int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	D3DXVECTOR3 currVertPos = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = 1.0f;

	for (DWORD i = 0; i < (unsigned int)LatLines - 2; ++i)
	{
		spherePitch = float((i + 1) * (3.14 / (LatLines - 1)));
		D3DXMatrixRotationX(&Rotationx, spherePitch);
		for (DWORD j = 0; j < (unsigned int)LongLines; ++j)
		{
			sphereYaw = float(j * (6.28 / (LongLines)));
			D3DXMatrixRotationY(&Rotationy, sphereYaw);

			D3DXMATRIX tempMat = Rotationx * Rotationy;
			D3DXVECTOR3 tempVec = D3DXVECTOR3(0.f, 0.f, 1.f);

			D3DXVec3TransformNormal(&currVertPos, &tempVec, &tempMat);
			D3DXVec3Normalize(&currVertPos, &currVertPos);

			vertices[i*LongLines + j + 1].position.x = currVertPos.x;
			vertices[i*LongLines + j + 1].position.y = currVertPos.y;
			vertices[i*LongLines + j + 1].position.z = currVertPos.z;
		}
	}

	vertices[NumSphereVertices - 1].position.x = 0.0f;
	vertices[NumSphereVertices - 1].position.y = 0.0f;
	vertices[NumSphereVertices - 1].position.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	d3d->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);

	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < (unsigned int)LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < (unsigned int)LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < (unsigned int)LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < (unsigned int)LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d->GetDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
}


void SkyboxClass::Render(D3DClass *d3d, TextureShaderClass* textureShaderClass, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos)
{

	D3DXMATRIX matScale, matTrans, matRotX, matRotY, matRotZ;
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 1.5f);

	D3DXMatrixTranslation(&matTrans, camPos.x - 74.f, camPos.y + 74.f, camPos.z - 74.f);
	D3DXMatrixRotationX(&matRotX, (float)D3DXToRadian(90.f));
	D3DXMatrixRotationY(&matRotY, (float)D3DXToRadian(-90.f));
	m_matWorld = matScale * matRotX * matRotY * matTrans;
	m_Model->Render(d3d->GetDeviceContext());
	textureShaderClass->Render(d3d->GetDeviceContext(), m_Model->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, m_Model->GetTexture());

	D3DXMatrixTranslation(&matTrans, camPos.x + 74.f, camPos.y + 74.f, camPos.z + 74.f);
	D3DXMatrixRotationX(&matRotX, (float)D3DXToRadian(90.f));
	D3DXMatrixRotationY(&matRotY, (float)D3DXToRadian(90.f));
	m_matWorld = matScale * matRotX * matRotY * matTrans;
	m_lf->Render(d3d->GetDeviceContext());
	textureShaderClass->Render(d3d->GetDeviceContext(), m_lf->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, m_lf->GetTexture());

	D3DXMatrixTranslation(&matTrans, camPos.x + 74.f, camPos.y + 74.f, camPos.z - 74.f);
	D3DXMatrixRotationY(&matRotY, (float)D3DXToRadian(-90.f));
	m_matWorld = matScale * matRotY* matTrans;
	m_up->Render(d3d->GetDeviceContext());
	textureShaderClass->Render(d3d->GetDeviceContext(), m_up->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, m_up->GetTexture());

	D3DXMatrixTranslation(&matTrans, camPos.x + 74.f, camPos.y - 74.f, camPos.z + 74.f);
	D3DXMatrixRotationY(&matRotY, (float)D3DXToRadian(180.f));
	m_matWorld = matScale * matRotY* matTrans;
	m_dn->Render(d3d->GetDeviceContext());
	textureShaderClass->Render(d3d->GetDeviceContext(), m_dn->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, m_dn->GetTexture());

	D3DXMatrixTranslation(&matTrans, camPos.x - 74.f, camPos.y + 74.f, camPos.z + 74.f);
	D3DXMatrixRotationX(&matRotX, (float)D3DXToRadian(90.f));
	m_matWorld = matScale * matRotX * matTrans;

	m_ft->Render(d3d->GetDeviceContext());
	textureShaderClass->Render(d3d->GetDeviceContext(), m_ft->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, m_ft->GetTexture());

	D3DXMatrixTranslation(&matTrans, camPos.x + 74.f, camPos.y + 74.f, camPos.z - 74.f);
	D3DXMatrixRotationX(&matRotX, (float)D3DXToRadian(90.f));
	D3DXMatrixRotationY(&matRotY, (float)D3DXToRadian(-180.f));
	m_matWorld = matScale * matRotX *matRotY* matTrans;

	m_bk->Render(d3d->GetDeviceContext());
	textureShaderClass->Render(d3d->GetDeviceContext(), m_bk->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, m_bk->GetTexture());
}

bool SkyboxClass::Frame(float frameTime)
{
	return true;
}

void SkyboxClass::Shutdown()
{
	sphereIndexBuffer->Release();
	sphereVertBuffer->Release();

	vertLayout->Release();
	matrixBuffer->Release();

	smrv->Release();
	DSLessEqual->Release();

	if (m_lf)
	{
		m_lf->Shutdown();
		delete m_lf;
		m_lf = 0;
	}

	if (m_up)
	{
		m_up->Shutdown();
		delete m_up;
		m_up = 0;
	}

	if (m_dn)
	{
		m_dn->Shutdown();
		delete m_dn;
		m_dn = 0;
	}

	if (m_ft)
	{
		m_ft->Shutdown();
		delete m_ft;
		m_ft = 0;
	}

	if (m_bk)
	{
		m_bk->Shutdown();
		delete m_bk;
		m_bk = 0;
	}
}
