#include "PlayerClass.h"

#include "inputclass.h"
#include "Collision.h"

PlayerClass::PlayerClass()
{
	Initialize();
}

PlayerClass::PlayerClass(const PlayerClass &other)
{
}

PlayerClass::~PlayerClass()
{
	Shutdown();
}

void PlayerClass::ReInit()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
	D3DXMatrixTranslation(&matTrans, 0.f, -10.f, 0.f);
	D3DXMatrixIdentity(&m_matWorld);

	m_matWorld = matScale * matTrans * m_matWorld;

	m_TouchedColl = nullptr;

	m_bCanMove = true;
	m_bDead = false;
}

bool PlayerClass::Frame(float frameTime)
{
	if(m_bCanMove && !m_bDead)
		Move(frameTime);

	if (m_Collision)
		m_Collision->UpdateAABB(GetPos());

	return true;
}

void PlayerClass::Initialize()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
	D3DXMatrixTranslation(&matTrans, 0.f, -10.f, 0.f);

	m_matWorld = matScale * matTrans * m_matWorld;
	m_eTag = TAG_PLAYER;

	m_Input = nullptr;
	m_Collision = new Collision;
	m_Collision->InitAABB(Collision::COL_PLAYER, GetPos(), D3DXVECTOR3(0.2f, 0.2f, 0.2f));
	m_TouchedColl = nullptr;

	m_bCanMove = true;

	m_fSpeed = 0.02f;
}

void PlayerClass::Move(float frameTime)
{
	if (m_Input->IsKeyPressing(DIK_W))
	{
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDir = GetLook();

		vPos += vDir * frameTime * m_fSpeed;

		if (vPos.x > 4.7 || vPos.x < -4.7)
			return;

		SetPos(vPos);
	}
	
	if (m_Input->IsKeyPressing(DIK_S))
	{
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDir = GetLook();

		vPos -= vDir * frameTime * m_fSpeed;
		
		if (vPos.x > 4.7 || vPos.x < -4.7)
			return;
		
		SetPos(vPos);
	}

	if (m_Input->IsKeyPressing(DIK_A))
	{
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDir = GetRight();

		vPos -= vDir * frameTime * m_fSpeed;
	
		if (vPos.x > 4.7 || vPos.x < -4.7)
			return;
	
		SetPos(vPos);
	}

	if (m_Input->IsKeyPressing(DIK_D))
	{
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDir = GetRight();

		vPos += vDir * frameTime * m_fSpeed;
		
		if (vPos.x > 4.7 || vPos.x < -4.7)
			return;

		SetPos(vPos);
	}
}

void PlayerClass::Shutdown()
{
}
