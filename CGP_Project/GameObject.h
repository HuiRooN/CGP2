#pragma once
#include "ModelClass.h"

class ModelClass;
class Collision;
class LightClass;

class GameObject
{
public:
	enum TAG {TAG_SKYBOX, TAG_BOUNDARY, TAG_PLAYER, TAG_AI, TAG_ITEM, TAG_SCENEUI, TAG_UI, TAG_EFFECT, TAG_END};

public:
	GameObject();
	GameObject(const GameObject&);
	virtual ~GameObject();

public:
	const TAG GetTag() { return m_eTag; }
	const D3DXMATRIX GetWorld() const { return m_matWorld; }
	ModelClass* GetModel() { return m_Model; }
	Collision* GetCollision() { return m_Collision; }

	void SetWorld(D3DXMATRIX matWorld) { m_matWorld = matWorld; }
	void SetRight(D3DXVECTOR3 vRight);
	void SetUp(D3DXVECTOR3 vUp);
	void SetLook(D3DXVECTOR3 vLook);
	void SetPos(D3DXVECTOR3 vPos);

	const D3DXVECTOR3 GetRight() { return D3DXVECTOR3(m_matWorld._11, m_matWorld._12, m_matWorld._13); }
	const D3DXVECTOR3 GetUp() { return D3DXVECTOR3(m_matWorld._21, m_matWorld._22, m_matWorld._23); }
	const D3DXVECTOR3 GetLook() { return D3DXVECTOR3(m_matWorld._31, m_matWorld._32, m_matWorld._33); }
	const D3DXVECTOR3 GetPos() { return D3DXVECTOR3(m_matWorld._41, m_matWorld._42, m_matWorld._43); }
	float GetScale(int eType) { return D3DXVec3Length((D3DXVECTOR3*)&m_matWorld.m[eType][0]); }
	
public:
	void Init();
	void Initialize(ID3D11Device*, const wchar_t*);
	void InitializeObj(ID3D11Device*, const char*, const wchar_t*);
	void InitializeSkymap(ID3D11Device*);
	void InitializePlane(ID3D11Device*, int, int);
	virtual bool Frame(float) PURE;
	void Shutdown();

protected:
	ModelClass* m_Model;
	Collision* m_Collision;

	D3DXMATRIX m_matWorld;
	TAG m_eTag;
};

