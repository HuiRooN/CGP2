#include "GameObject.h"

#include "modelclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"

GameObject::GameObject()
{
	Init();
}

GameObject::GameObject(const GameObject & other)
{
}

GameObject::~GameObject()
{
	Shutdown();
}

void GameObject::SetPos(D3DXVECTOR3 vPos)
{
	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;
}

void GameObject::SetRight(D3DXVECTOR3 vRight)
{
	D3DXVec3Normalize(&vRight, &vRight);

	m_matWorld._11 = vRight.x;
	m_matWorld._12 = vRight.y;
	m_matWorld._13 = vRight.z;
}

void GameObject::SetUp(D3DXVECTOR3 vUp)
{
	D3DXVec3Normalize(&vUp, &vUp);

	m_matWorld._21 = vUp.x;
	m_matWorld._22 = vUp.y;
	m_matWorld._23 = vUp.z;
}

void GameObject::SetLook(D3DXVECTOR3 vNewLook)
{
	D3DXVECTOR3 vRight, vUp, vLook;

	D3DXVECTOR3 tempRight = GetRight();
	D3DXVECTOR3 tempUp = GetUp();
	D3DXVECTOR3 tempLook = GetLook();

	D3DXVECTOR3	vScale = D3DXVECTOR3(D3DXVec3Length(&tempRight), D3DXVec3Length(&tempUp), D3DXVec3Length(&tempLook));

	D3DXVECTOR3		vPosition = GetPos();
	vUp = GetUp();

	D3DXVECTOR3		vDir = vNewLook;

	D3DXVec3Cross(&vRight, &vUp, &vDir);
	D3DXVec3Cross(&vLook, &vRight, &vUp);

	SetRight(*D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	m_matWorld._31 = D3DXVECTOR3(*D3DXVec3Normalize(&vLook, &vLook) * vScale.z).x;
	m_matWorld._32 = D3DXVECTOR3(*D3DXVec3Normalize(&vLook, &vLook) * vScale.z).y;
	m_matWorld._33 = D3DXVECTOR3(*D3DXVec3Normalize(&vLook, &vLook) * vScale.z).z;
}

void GameObject::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_eTag = TAG_END;
}

void GameObject::Initialize(ID3D11Device *device, const wchar_t *textureFilename)
{
	m_Model = new ModelClass;
	m_Model->Initialize(device, textureFilename);
}

void GameObject::InitializeObj(ID3D11Device *device, const char * modelFilename, const wchar_t * textureFilename)
{
	m_Model = new ModelClass;
	m_Model->InitializeObj(device, modelFilename, textureFilename);
}

void GameObject::InitializeSkymap(ID3D11Device* device)
{
	m_Model = new ModelClass;
	m_Model->InitializeSkymap(device, "../Engine/data/Skybox/Cube.txt", L"../Engine/data/Skybox/Skybox.png");
}

void GameObject::Shutdown()
{
	if (m_eTag == TAG_AI)
		return;

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}
}

