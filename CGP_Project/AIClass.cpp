#include "AIClass.h"

#include "lightshaderclass.h"
#include "lightclass.h"
#include "Collision.h"

AIClass::AIClass()
{
	Initialize();
}

AIClass::AIClass(const AIClass &other)
{
}

AIClass::~AIClass()
{
	Shutdown();
}

ModelClass * AIClass::GetModel(OBJTYPE objType)
{
	switch (objType)
	{
	case OBJ_CAR:
		return m_model2;
	case OBJ_UNICORN:
		return m_model3;
	}

	return nullptr;
}

void AIClass::SetModels(ModelClass *model1, ModelClass *model2, ModelClass *model3)
{
	m_Model = model1;
	m_model2 = model2;
	m_model3 = model3;
}

bool AIClass::Frame(float frameTime)
{
	if (!m_bInit && m_eObjType == OBJ_END)
		LateInit();

	if (m_bCanMove)
		Move(frameTime);

	if (m_Collision)
		m_Collision->UpdateAABB(GetPos());

	return true;
}

void AIClass::Render(ID3D11DeviceContext *deviceContext, D3DMATRIX matView, D3DXMATRIX matProj, LightShaderClass *lightShaderClass, LightClass *lightClass, D3DXVECTOR3 camPos, D3DXVECTOR4 * pointDiffuseColor, D3DXVECTOR4 *lightPosition)
{
	if (!m_bCanMove)
		return;

	switch (m_eObjType)
	{
	case OBJ_BOAT:
	{
		m_Model->Render(deviceContext);
		lightShaderClass->Render(deviceContext, m_Model->GetIndexCount(), GetWorld(), matView, matProj, m_Model->GetTexture(),
			lightClass->GetDirection(), lightClass->GetAmbientColor(), lightClass->GetDiffuseColor(), camPos,
			lightClass->GetSpecularColor(), lightClass->GetSpecularPower(), pointDiffuseColor, lightPosition);
	}
	break;
	case OBJ_CAR:
	{
		m_model2->Render(deviceContext);
		lightShaderClass->Render(deviceContext, m_model2->GetIndexCount(), GetWorld(), matView, matProj, m_model2->GetTexture(),
			lightClass->GetDirection(), lightClass->GetAmbientColor(), lightClass->GetDiffuseColor(), camPos,
			lightClass->GetSpecularColor(), lightClass->GetSpecularPower(), pointDiffuseColor, lightPosition);
	}
	break;
	case OBJ_UNICORN:
	{
		m_model3->Render(deviceContext);
		lightShaderClass->Render(deviceContext, m_model3->GetIndexCount(), GetWorld(), matView, matProj, m_model3->GetTexture(),
			lightClass->GetDirection(), lightClass->GetAmbientColor(), lightClass->GetDiffuseColor(), camPos,
			lightClass->GetSpecularColor(), lightClass->GetSpecularPower(), pointDiffuseColor, lightPosition);
	}
	break;
	case OBJ_END:
	default:
		return;
	}
}

void AIClass::Shutdown()
{
}

void AIClass::Initialize()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	m_matWorld = matScale * m_matWorld;
	m_matSaveWorld = m_matWorld;
	m_eTag = TAG_AI;

	m_Collision = new Collision;
	m_Collision->InitAABB(Collision::COL_AI, GetPos(), D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	m_eObjType = OBJ_END;
	m_eLine = LINE_END;
}

void AIClass::LateInit()
{
	D3DXMATRIX matRotY;
	D3DXVECTOR3 vPos;

	if (!m_bReInit)
	{
		if ((int(m_eLine)) % 2 == 0)
			m_bStartRight = false;
		else
			m_bStartRight = true;
	
		if (m_eNumType == NUM_1)
			m_bCanMove = true;
		
		m_bReInit = true;
	}

	switch (m_eLine)
	{
	case LINE_1:
		vPos = D3DXVECTOR3(-4.f, -9.995f, 3.f);
		m_fSpeed = 0.003f;
		break;
	case LINE_2:
		vPos = D3DXVECTOR3(4.f, -9.995f, 7.f);
		m_fSpeed = 0.003f;
		break;
	case LINE_3:
		vPos = D3DXVECTOR3(-4.f, -9.995f, 11.f);
		m_fSpeed = 0.004f;
		break;
	case LINE_4:
		vPos = D3DXVECTOR3(4.f, -9.995f, 15.f);
		m_fSpeed = 0.004f;
		break;
	case LINE_5:
		vPos = D3DXVECTOR3(-4.f, -9.995f, 19.f);
		m_fSpeed = 0.005f;
		break;
	case LINE_6:
		vPos = D3DXVECTOR3(4.f, -9.995f, 23.f);
		m_fSpeed = 0.005f;
		break;
	case LINE_7:
		vPos = D3DXVECTOR3(-4.f, -9.995f, 27.f);
		m_fSpeed = 0.006f;
		break;
	case LINE_8:
		vPos = D3DXVECTOR3(4.f, -9.995f, 31.f);
		m_fSpeed = 0.006f;
		break;
	case LINE_END:
	default:
		return;
	}

	m_eObjType = OBJTYPE(rand() % 3);

	switch (m_eObjType)
	{
	case OBJ_BOAT:
		if (!m_bStartRight)
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(180.f)));
		else
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(0.f)));
		m_Collision->SetSize(D3DXVECTOR3(0.55f, 0.5f, 0.3f));
		break;
	case OBJ_CAR:
		if (!m_bStartRight)
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(-90.f)));
		else
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(90.f)));
		m_Collision->SetSize(D3DXVECTOR3(0.3f, 0.3f, 0.3f));
		break;
	case OBJ_UNICORN:
		vPos.y += 0.2f;
		if (!m_bStartRight)
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(-90.f)));
		else
			D3DXMatrixRotationY(&matRotY, float(D3DXToRadian(90.f)));
		m_Collision->SetSize(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
		break;
	case OBJ_END:
	default:
		return;
	}
	m_matWorld = matRotY * m_matSaveWorld;
	SetPos(vPos);
	
	m_bInit = true;
}

void AIClass::Move(float frameTime)
{
	if (m_eObjType == OBJ_END)
		return;

	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDir = GetLook();
	
	if (m_bStartRight)
	{
		if (vPos.x < -4.f)
			ReachEnd();

		vPos.x -= frameTime * m_fSpeed;
	}
	else
	{
		if (vPos.x > 4.f)
			ReachEnd();

		vPos.x += frameTime * m_fSpeed;
	}
	SetPos(vPos);
}

bool AIClass::ReachMiddle()
{
	D3DXVECTOR3 vPos = GetPos();

	if (m_bStartRight)
	{
		if (vPos.x <= 1.3f)
			return true;
	}
	else
	{
		if (vPos.x >= -1.3f)
			return true;
	}

	return false;
}

void AIClass::ReachEnd()
{
	m_eObjType = OBJ_END;
	m_bInit = false;
	m_bCanMove = false;

	//m_fSpeed += (rand() % 5 + 1)*0.0001f;
}
