#include "SceneUIClass.h"

#include "inputclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"

SceneUIClass::SceneUIClass()
{
	Initialize();
}

SceneUIClass::SceneUIClass(const SceneUIClass &other)
{
}

SceneUIClass::~SceneUIClass()
{
	ShutDown();
}

void SceneUIClass::InitializeTex(ID3D11Device *device)
{
	m_Model = new ModelClass;
	m_Model->Initialize(device, L"../Engine/data/UI/Title.png");

	m_GoalTex = new ModelClass;
	m_GoalTex->Initialize(device, L"../Engine/data/UI/Goal.png");

	m_CntlTex = new ModelClass;
	m_CntlTex->Initialize(device, L"../Engine/data/UI/Control.png");

	m_ClearTex = new ModelClass;
	m_ClearTex->Initialize(device, L"../Engine/data/UI/Clear.png");

	m_FailTex = new ModelClass;
	m_FailTex->Initialize(device, L"../Engine/data/UI/Fail.png");
}

bool SceneUIClass::Frame(float)
{
	switch (m_iSceneID)
	{
	case 0:
		switch (m_eTexID)
		{
		case TEX_TITLE:
		{
			if (!m_bIsFirst)
				return true;

			if (m_Input->IsKeyPressed(DIK_RETURN))
				m_eTexID = TEX_GOAL;
		}
			break;
		case TEX_GOAL:
			if (m_Input->IsKeyPressed(DIK_RETURN))
				m_eTexID = TEX_CONTROL;
			break;
		case TEX_CONTROL:
			m_bIsFirst = false;
			break;
		case TEX_END:
			m_eTexID = TEX_TITLE;
			break;
		}
		break;
	}

	return true;
}

void SceneUIClass::Render(ID3D11DeviceContext *deviceContext, TextureShaderClass *textureShaderClass, D3DXMATRIX matView, D3DXMATRIX matProj)
{
	D3DXMATRIX matScale, matTrans, matBill;
	
	D3DXVECTOR3 vCamPos = m_Camera->GetPosition();
	D3DXVECTOR3 vCamDir = D3DXVECTOR3(0.f, 0.f, 1.f);
	
	matBill = matView;
	D3DXMatrixInverse(&matBill, NULL, &matBill);
	matBill._41 = vCamPos.x + vCamDir.x;
	matBill._42 = vCamPos.y + vCamDir.y;
	matBill._43 = vCamPos.z + vCamDir.z;

	switch (m_eTexID)
	{
	case TEX_TITLE:
		D3DXMatrixScaling(&matScale, 0.85f, 0.42f, 0.85f);
		m_matWorld = matScale * matBill;

		m_Model->Render(deviceContext);
		textureShaderClass->Render(deviceContext, m_Model->GetIndexCount(), m_matWorld, matView, matProj, m_Model->GetTexture());
		break;
	case TEX_GOAL:
		D3DXMatrixScaling(&matScale, 0.85f, 0.42f, 0.85f);
		m_matWorld = matScale * matBill;
		
		m_GoalTex->Render(deviceContext);
		textureShaderClass->Render(deviceContext, m_GoalTex->GetIndexCount(), GetWorld(), matView, matProj, m_GoalTex->GetTexture());
		break;
	case TEX_CONTROL:
		D3DXMatrixScaling(&matScale, 0.85f, 0.42f, 0.85f);
		D3DXMatrixTranslation(&matTrans, 0.02f, 0.f, 0.f);
		m_matWorld = matScale * matTrans * matBill;
		
		m_CntlTex->Render(deviceContext);
		textureShaderClass->Render(deviceContext, m_CntlTex->GetIndexCount(), GetWorld(), matView, matProj, m_CntlTex->GetTexture());
		break;
	case TEX_CLEAR:
		D3DXMatrixScaling(&matScale, 0.85f, 0.42f, 0.85f);
		m_matWorld = matScale * matBill;
	
		m_ClearTex->Render(deviceContext);
		textureShaderClass->Render(deviceContext, m_ClearTex->GetIndexCount(), GetWorld(), matView, matProj, m_ClearTex->GetTexture());
		break;
	case TEX_FAIL:
		D3DXMatrixScaling(&matScale, 0.85f, 0.42f, 0.85f);
		m_matWorld = matScale * matBill;
		
		m_FailTex->Render(deviceContext);
		textureShaderClass->Render(deviceContext, m_FailTex->GetIndexCount(), GetWorld(), matView, matProj, m_FailTex->GetTexture());
		break;
	case TEX_END:
	default:
		break;
	}
}

void SceneUIClass::Initialize()
{
	m_eTag = TAG_SCENEUI;

	m_iSceneID = 3;
	m_eTexID = TEX_END;

	m_bIsFirst = true;
}

void SceneUIClass::ShutDown()
{
	if (m_GoalTex)
	{
		m_GoalTex->Shutdown();
		delete m_GoalTex;
		m_GoalTex = 0;
	}

	if (m_CntlTex)
	{
		m_CntlTex->Shutdown();
		delete m_CntlTex;
		m_CntlTex = 0;
	}

	if (m_ClearTex)
	{
		m_ClearTex->Shutdown();
		delete m_ClearTex;
		m_ClearTex = 0;
	}

	if (m_FailTex)
	{
		m_FailTex->Shutdown();
		delete m_FailTex;
		m_FailTex = 0;
	}
}
