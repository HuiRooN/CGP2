#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
using namespace std;

class Collision
{
public:
	enum COLLISION_TAG { COL_PLAYER, COL_AI, COL_ITEM, COL_GOAL, COL_END};

public:
	Collision();
	~Collision();

public:
	void InitSphere(COLLISION_TAG, const D3DXVECTOR3, const float);
	void InitAABB(COLLISION_TAG, D3DXVECTOR3, D3DXVECTOR3);
	void UpdatePos(D3DXVECTOR3);
	void UpdateAABB(D3DXVECTOR3);
	//void Render(ID3D11DeviceContext * divceContext);

public:
	const COLLISION_TAG Get_Tag() { return m_eColTag; }
	const D3DXVECTOR3& Get_Pos() { return m_vPos; }
	float Get_Radius() { return m_fRadius; }
	D3DXVECTOR3 GetMax() { return m_vMax; }
	D3DXVECTOR3 GetMin() { return m_vMin; }

	void SetSize(D3DXVECTOR3 vSize) { m_vSize = vSize; }

private:
	D3DXVECTOR3 m_vPos;
	float m_fRadius;

	D3DXVECTOR3 m_vSize;
	D3DXVECTOR3 m_vMax;
	D3DXVECTOR3 m_vMin;

	COLLISION_TAG m_eColTag;
};
	

