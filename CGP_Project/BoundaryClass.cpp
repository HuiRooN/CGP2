#include "BoundaryClass.h"

#include "Collision.h"

BoundaryClass::BoundaryClass()
{
	Initialize();
}

BoundaryClass::BoundaryClass(const BoundaryClass &other)
{
}

BoundaryClass::~BoundaryClass()
{
	Shutdown();
}

bool BoundaryClass::Frame(float)
{
	if (!m_bInit)
	{
		D3DXMATRIX matRotY;
		
		if (m_bIsLeft)
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(-90.f)));
		else
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(90.f)));

		m_matWorld = matRotY * m_matWorld;

		m_bInit = true;
	}

	return true;
}

void BoundaryClass::Initialize()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);

	m_matWorld = matScale * m_matWorld;

	m_eTag = TAG_BOUNDARY;
}

void BoundaryClass::Shutdown()
{
}
