#include "UIClass.h"

#include "cameraclass.h"
#include "PlayerClass.h"
#include "textureshaderclass.h"

UIClass::UIClass()
{
	Initialize();
}

UIClass::UIClass(const UIClass &other)
{
}

UIClass::~UIClass()
{
	ShutDown();
}

int UIClass::GetPolycnt()
{
	int polyCnt = 0;
	
	for (auto iter : m_vecNum)
		polyCnt += iter->GetIndexCount() / 3;

	return polyCnt;
}

void UIClass::InitializeTexForNum(ID3D11Device *device)
{
	char numString[32];
	char textureString[32];
	wchar_t finalString[32];
	size_t finalStringLength[32];

	for (int i = 0; i < 10; ++i)
	{
		_itoa_s(i, numString, 10);
		
		strcpy_s(textureString, "../Engine/data/UI/Numbers/");
		strcat_s(textureString, numString);
		strcat_s(textureString, ".png");

		mbstowcs_s(finalStringLength, finalString, textureString, size_t(strlen(textureString) + 1));

		m_Model = new ModelClass;
		m_Model->Initialize(device, finalString);

		m_vecNum.push_back(m_Model);
	}

	m_Model = nullptr;
}

bool UIClass::Frame(float frameTime)
{
	if (m_iSceneID == 1 || !m_Player->GetCanMove())
		return true;

	else if (m_eUIID == UI_TIMER)
		UpdateNumber();

	return true;
}

void UIClass::Render(ID3D11DeviceContext *deviceContext, TextureShaderClass *textureShaderClass, D3DXMATRIX matView, D3DXMATRIX matProj)
{
	if (m_iSceneID == 1 || !m_Player->GetCanMove())
		return;

	D3DXMATRIX matScale, matTrans, matBill;

	D3DXVECTOR3 vCamPos = m_Camera->GetPosition();
	D3DXVECTOR3 vCamDir = D3DXVECTOR3(0.f, 0.f, 1.f);

	matBill = m_Camera->GetViewMatrix();
	D3DXMatrixInverse(&matBill, NULL, &matBill);
	matBill._41 = vCamPos.x + vCamDir.x;
	matBill._42 = vCamPos.y + vCamDir.y;
	matBill._43 = vCamPos.z + vCamDir.z;

	switch (m_eUIID)
	{
	case UI_TIMERTEXT:
	{
		D3DXMatrixScaling(&matScale, 0.062f, 0.025f, 0.025f);
		D3DXMatrixTranslation(&matTrans, -0.05f, 0.373f, 0.f);
		m_matWorld = matScale * matTrans * matBill;
		
		m_Model->Render(deviceContext);
		textureShaderClass->Render(deviceContext, m_Model->GetIndexCount(), m_matWorld, matView, matProj, m_Model->GetTexture());
	}
	break;
	case UI_TIMER:
	{
		if (m_iTimerCnt == 0)
			UpdateNumber();

		int iTempCnt = m_iTimerCnt;
		while (m_iTimerCnt > 0)
		{
			D3DXMatrixScaling(&matScale, 0.02f, 0.02f, 0.02f);
			D3DXMatrixTranslation(&matTrans, 0.05f - 0.029f * (iTempCnt - m_iTimerCnt), 0.373f, 0.f);
			m_matWorld = matScale * matTrans * matBill;

			m_vecNum[m_iTimer % 10]->Render(deviceContext);
			textureShaderClass->Render(deviceContext, m_vecNum[m_iTimer % 10]->GetIndexCount(), m_matWorld, matView, matProj, m_vecNum[m_iTimer % 10]->GetTexture());
		
			m_iTimer /= 10;
			--m_iTimerCnt;
		}
	}
	break;
	}
}

void UIClass::Initialize()
{
	m_eTag = TAG_UI;

	m_iSceneID = 3;
	m_eUIID = UI_END;
	m_iTimerCnt = 0;
}

void UIClass::ShutDown()
{
	if (m_vecNum.size() != 0)
	{
		for (auto iter : m_vecNum)
		{
			if (iter)
			{
				iter->Shutdown();
				delete iter;
				iter = 0;
			}
		}

		m_vecNum.clear();
	}
}

void UIClass::UpdateNumber()
{
	int iTempNumber = m_iTimer;

	if (iTempNumber == 0)
		m_iTimerCnt = 1;

	while (iTempNumber > 0)
	{
		++m_iTimerCnt;
		iTempNumber /= 10;
	}
}
