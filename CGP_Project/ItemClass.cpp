#include "ItemClass.h"

#include "Collision.h"

ItemClass::ItemClass()
{
	Initialize();
}

ItemClass::ItemClass(const ItemClass & other)
{
}

ItemClass::~ItemClass()
{
	Shutdown();
}

void ItemClass::InitializeObj(ID3D11Device *device)
{
	m_eType = ITEMTYPE(rand() % 3);

	switch (m_eType)
	{
	case ITEM_APPLE:
		dynamic_cast<GameObject*>(this)->InitializeObj(device, "../Engine/data/Apple/Apple.obj", L"../Engine/data/Apple/Apple.png");
		break;
	case ITEM_CAKE:
		dynamic_cast<GameObject*>(this)->InitializeObj(device, "../Engine/data/Cake/Cake.obj", L"../Engine/data/Cake/Cake.png");
		break;
	case ITEM_HAMBURGER:
		dynamic_cast<GameObject*>(this)->InitializeObj(device, "../Engine/data/Hamburger/Hamburger.obj", L"../Engine/data/Hamburger/Hamburger.png");
		break;
	case ITEM_END:
	default:
		break;
	}
}

bool ItemClass::Frame(float frameTime)
{
	if (m_bIsHidden)
		return true;

	if (!m_bInit)
		LateInit();

	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(frameTime * 0.1f)));
	m_matWorld = matRotY * m_matWorld;

	if (m_Collision)
		m_Collision->UpdateAABB(GetPos());

	return true;
}

void ItemClass::Initialize()
{
	m_eTag = TAG_ITEM;

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
	m_matWorld = matScale * m_matWorld;

	m_Collision = new Collision;
	m_Collision->InitAABB(Collision::COL_ITEM, GetPos(), D3DXVECTOR3(0.2f, 0.2f, 0.2f));
}

void ItemClass::LateInit()
{
	D3DXVECTOR3 vPos;

	float PosX = (rand() % 5) + (rand() % 10)*0.1f + (rand() % 10)*0.01f;
	if (rand() % 2 == 0)
		PosX *= -1.f;

	if (PosX < -4.f)
		PosX = -4.f;
	else if (PosX > 4.f)
		PosX = 4.f;

	switch (m_eLine)
	{
	case LINE_1:
		vPos = D3DXVECTOR3(PosX, -9.98f, 5.f);
		break;
	case LINE_2:
		vPos = D3DXVECTOR3(PosX, -9.98f, 9.f);
		break;
	case LINE_3:
		vPos = D3DXVECTOR3(PosX, -9.98f, 13.f);
		break;
	case LINE_4:
		vPos = D3DXVECTOR3(PosX, -9.98f, 17.f);
		break;
	case LINE_5:
		vPos = D3DXVECTOR3(PosX, -9.98f, 21.f);
		break;
	case LINE_6:
		vPos = D3DXVECTOR3(PosX, -9.98f, 25.f);
		break;
	case LINE_7:
		vPos = D3DXVECTOR3(PosX, -9.98f, 29.f);
		break;
	case LINE_END:
	default:
		return;
	}
	SetPos(vPos);

	m_bInit = true;
}

void ItemClass::Shutdown()
{
}
