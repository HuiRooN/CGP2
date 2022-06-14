#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::InitSphere(COLLISION_TAG eColltag, const D3DXVECTOR3 vPos, const float fRadius)
{
	m_eColTag = eColltag;
	m_vPos = vPos;
	m_fRadius = fRadius;
}

void Collision::InitAABB(COLLISION_TAG eTag, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize)
{
	m_eColTag = eTag;
	m_vPos = vPos;
	m_vSize = vSize;

	m_vMax = D3DXVECTOR3(vPos.x + vSize.x, vPos.y + vSize.y, vPos.z + vSize.z);
	m_vMin = D3DXVECTOR3(vPos.x - vSize.x, vPos.y - vSize.y, vPos.z - vSize.z);
}

void Collision::UpdatePos(D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
}

void Collision::UpdateAABB(D3DXVECTOR3 vPos)
{
	m_vPos = vPos;

	m_vMax = D3DXVECTOR3(m_vPos.x + m_vSize.x, m_vPos.y + m_vSize.y, m_vPos.z + m_vSize.z);
	m_vMin = D3DXVECTOR3(m_vPos.x - m_vSize.x, m_vPos.y - m_vSize.y, m_vPos.z - m_vSize.z);
}




