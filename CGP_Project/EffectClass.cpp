#include "EffectClass.h"

#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "PlayerClass.h"
#include "GameObjectMgr.h"

EffectClass::EffectClass()
{
	Initialize();
}

EffectClass::EffectClass(const EffectClass &other)
{
}

EffectClass::~EffectClass()
{
	Shutdown();
}

bool EffectClass::Frame(float frameTime)
{
	if (!m_Player->GetCanMove())
		return true;

	switch (m_eID)
	{
	case EFFECT_PARTI:
	{
		if (!m_bInit)
			LateInit();
	
		if (m_frameTime > 1.5f || m_bDead)
		{
			m_GameObjectMgr->EraseGameObject(this);
			return false;
		}

		m_frameTime += 0.01f;
		Move(frameTime);
	}
		break;
	case EFFECT_SPARKL:
	{
		if (m_frameTime > 1.f || m_bDead)
		{
			m_GameObjectMgr->EraseGameObject(this);
			return false;
		}
		m_frameTime += 0.01f;
	}
		break;
	}

	return true;
}

void EffectClass::Render(ID3D11DeviceContext *deviceContext, TextureShaderClass *textureShaderClass, D3DXMATRIX matView, D3DXMATRIX matProj)
{
	if (!m_Player->GetCanMove())
		return;

	D3DXMATRIX matScale, matBill;
	D3DXMatrixScaling(&matScale, 0.05f, 0.05f, 0.05f);

	D3DXVECTOR3 vPlayerPos = m_Player->GetPos();
	D3DXVECTOR3 vPlayerRight = D3DXVECTOR3(1.f, 0.f, 0.f);

	matBill = matView;
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	matBill._41 = vPlayerPos.x + 0.15f;
	matBill._42 = vPlayerPos.y + 0.185f + m_frameTime * 0.1f;
	matBill._43 = vPlayerPos.z - 0.01f;
	m_matWorld = matScale * matBill;

	m_Model->Render(deviceContext);
	textureShaderClass->Render(deviceContext, m_Model->GetIndexCount(), m_matWorld, matView, matProj, m_Model->GetTexture());

	matBill._41 = vPlayerPos.x - 0.15f;
	matBill._42 = vPlayerPos.y + 0.085f + m_frameTime * 0.1f;
	matBill._43 = vPlayerPos.z - 0.01f;

	m_matWorld = matScale * matBill;

	m_Model->Render(deviceContext);
	textureShaderClass->Render(deviceContext, m_Model->GetIndexCount(), m_matWorld, matView, matProj, m_Model->GetTexture());

	matBill._41 = vPlayerPos.x + 0.1f;
	matBill._42 = vPlayerPos.y + 0.035f + m_frameTime * 0.1f;
	matBill._43 = vPlayerPos.z - 0.1f;

	m_matWorld = matScale * matBill;

	m_Model->Render(deviceContext);
	textureShaderClass->Render(deviceContext, m_Model->GetIndexCount(), m_matWorld, matView, matProj, m_Model->GetTexture());
}

void EffectClass::Render(ID3D11DeviceContext *deviceContext, D3DMATRIX matView, D3DXMATRIX matProj, LightShaderClass* lightShaderClass, LightClass *lightClass, D3DXVECTOR3 camPos, D3DXVECTOR4* pointDiffuseColor, D3DXVECTOR4* lightPosition)
{
	if (!m_Player->GetCanMove() || !m_bInit)
		return;

	D3DXVECTOR3 vPos = GetPos();
	D3DXMATRIX matRotX, matRotY, matRotZ;
	
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	
	vPos += m_vDir * 0.01f * 0.5f;
	SetPos(vPos);

	D3DXMatrixRotationX(&matRotX, float(D3DXToRadian((rand() % 360)*0.01f)));
	D3DXMatrixRotationX(&matRotY, float(D3DXToRadian((rand() % 360)*0.01f)));
	D3DXMatrixRotationX(&matRotZ, float(D3DXToRadian((rand() % 360)*0.01f)));
	m_matWorld = matRotX * matRotY * matRotZ * m_matWorld;

	m_Model->Render(deviceContext);
	lightShaderClass->Render(deviceContext, m_Model->GetIndexCount(), GetWorld(), matView, matProj, m_Model->GetTexture(),
		lightClass->GetDirection(), lightClass->GetAmbientColor(), lightClass->GetDiffuseColor(), camPos,
		lightClass->GetSpecularColor(), lightClass->GetSpecularPower(), pointDiffuseColor, lightPosition);
}

void EffectClass::Initialize()
{
	m_eTag = TAG_EFFECT;

	m_eID = EFFECT_END;

	m_GameObjectMgr = nullptr;
	m_frameTime = 0;
}

void EffectClass::LateInit()
{
	if (m_eID == EFFECT_SPARKL)
		return;

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.05f*(rand() % 3 + 1), 0.05f*(rand() % 3 + 1), 0.05f*(rand() % 3 + 1));
	m_matWorld = matScale * m_matWorld;
	
	m_vDir = D3DXVECTOR3(cosf((float)D3DXToRadian(rand() % 360)), sinf((float)D3DXToRadian(rand() % 180 - 90)), cosf((float)D3DXToRadian(360 - rand() % 360)));

	m_matWorld._42 += 0.2f;

	m_bInit = true;
}

void EffectClass::Move(float frameTime)
{
}

void EffectClass::Shutdown()
{
}
