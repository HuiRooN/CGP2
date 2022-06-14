#include "GameObjectMgr.h"

#include "D3DClass.h"
#include "GameObject.h"
#include "Collision.h"

#include "LightClass.h"
#include "LightShaderClass.h"
#include "textureshaderclass.h"

#include "SkyboxClass.h"
#include "AIClass.h"
#include "ItemClass.h"
#include "SceneUIClass.h"
#include "UIClass.h"
#include "EffectClass.h"

GameObjectMgr::GameObjectMgr()
{
}

GameObjectMgr::GameObjectMgr(const GameObjectMgr & other)
{
}

GameObjectMgr::~GameObjectMgr()
{
	Shutdown();
}

bool GameObjectMgr::PushGameObject(GameObject * pGameObject)
{
	m_GameObjectLst.push_back(pGameObject);

	return true;
}

bool GameObjectMgr::PushAI(int eLine, vector<AIClass *> AI)
{
	if (AI.size() == 0)
		return false;

	m_mapAI.emplace(eLine, AI);
	return true;
}

GameObject * GameObjectMgr::GetGameObject(int eTag, Collision* coll)
{
	if (m_GameObjectLst.size() == 0)
		return nullptr;

	for (auto iter : m_GameObjectLst)
	{
		if (int(iter->GetTag()) == eTag)
		{
			if (!coll)
				return iter;
			else
			{
				if (iter->GetCollision() == coll)
					return iter;
			}
		}
	}

	return nullptr;
}

list<GameObject*> GameObjectMgr::GetGameObjectListByTag(int eTag)
{
	if (m_GameObjectLst.size() == 0)
		return list<GameObject*>();

	list<GameObject*> tagList;
	for (auto iter : m_GameObjectLst)
	{
		if (int(iter->GetTag()) == eTag)
			tagList.push_back(iter);
	}

	return tagList;
}

bool GameObjectMgr::Frame(float fFrameTime)
{
	if (m_GameObjectLst.size() == 0)
		return false;

	int i = 0;
	for (auto iter : m_GameObjectLst)
	{
		if (!iter->Frame(fFrameTime))
		{
			return false;
		}
	}
	return true;
}

bool GameObjectMgr::AIFrame(float frameTime)
{
	for (int i = 0; i < int(AIClass::LINE_END); ++i)
	{
		for (auto iter : m_mapAI[i])
		{
			if (iter->ReachMiddle())
			{
				if (iter->GetNumType() == AIClass::NUM_1)
				{
					for (auto iter : m_mapAI[i])
					{
						if (iter->GetNumType() == AIClass::NUM_2)
							iter->SetCanMove(true);
					}
				}
				else if (iter->GetNumType() == AIClass::NUM_2)
				{
					for (auto iter : m_mapAI[i])
					{
						if (iter->GetNumType() == AIClass::NUM_3)
							iter->SetCanMove(true);
					}
				}
				else if (iter->GetNumType() == AIClass::NUM_3)
				{
					for (auto iter : m_mapAI[i])
					{
						if (iter->GetNumType() == AIClass::NUM_1)
							iter->SetCanMove(true);
					}
				}
			}
		}
	}

	return true;
}

void GameObjectMgr::Render(D3DClass *D3D, D3DMATRIX matView, D3DXMATRIX matProj, TextureShaderClass* textureShaderClass, LightShaderClass* lightShaderClass, LightClass *lightClass, D3DXVECTOR3 camPos, D3DXVECTOR4* pointDiffuseColor, D3DXVECTOR4* lightPosition)
{
	if (m_GameObjectLst.size() == 0)
		return;

	for (auto iter : m_GameObjectLst)
	{
		ID3D11RasterizerState* pRasterState;
		D3D->GetDeviceContext()->RSGetState(&pRasterState);

		D3D11_RASTERIZER_DESC rasterDesc;
		pRasterState->GetDesc(&rasterDesc);

		if (rasterDesc.CullMode == D3D11_CULL_NONE)
		{
			ID3D11RasterizerState* pRasterState;
			D3D->GetDeviceContext()->RSGetState(&pRasterState);

			D3D11_RASTERIZER_DESC rasterDesc;
			pRasterState->GetDesc(&rasterDesc);

			rasterDesc.CullMode = D3D11_CULL_BACK;
			D3D->GetDevice()->CreateRasterizerState(&rasterDesc, &pRasterState);
			D3D->GetDeviceContext()->RSSetState(pRasterState);
		}

		switch (iter->GetTag())
		{
		case GameObject::TAG_SKYBOX:
		{
			rasterDesc.CullMode = D3D11_CULL_NONE;
			D3D->GetDevice()->CreateRasterizerState(&rasterDesc, &pRasterState);
			D3D->GetDeviceContext()->RSSetState(pRasterState);

			dynamic_cast<SkyboxClass*>(iter)->Render(D3D, textureShaderClass, matView, matProj, camPos);
		}
			continue;

		case GameObject::TAG_AI:
			dynamic_cast<AIClass*>(iter)->Render(D3D->GetDeviceContext(), matView, matProj, lightShaderClass, lightClass, camPos, pointDiffuseColor, lightPosition);
			continue;
		case GameObject::TAG_ITEM:
			if (dynamic_cast<ItemClass*>(iter)->GetHidden())
				continue;
			break;
		case GameObject::TAG_SCENEUI:
			dynamic_cast<SceneUIClass*>(iter)->Render(D3D->GetDeviceContext(), textureShaderClass, matView, matProj);
			continue;
		case GameObject::TAG_UI:
		{
			D3D->TurnOnAlphaBlending();
			dynamic_cast<UIClass*>(iter)->Render(D3D->GetDeviceContext(), textureShaderClass, matView, matProj);
			D3D->TurnOffAlphaBlending();
		}
			continue;
		case GameObject::TAG_EFFECT:
		{
			EffectClass* tempEffect = dynamic_cast<EffectClass*>(iter);
			if (tempEffect->GetEffectID() == EffectClass::EFFECT_PARTI)
				tempEffect->Render(D3D->GetDeviceContext(), matView, matProj, lightShaderClass, lightClass, camPos, pointDiffuseColor, lightPosition);
			else
			{
				D3D->TurnOnAlphaBlending();
				tempEffect->Render(D3D->GetDeviceContext(), textureShaderClass, matView, matProj);
				D3D->TurnOffAlphaBlending();
			}
		}
			continue;
		}

		iter->GetModel()->Render(D3D->GetDeviceContext());
		lightShaderClass->Render(D3D->GetDeviceContext(), iter->GetModel()->GetIndexCount(), iter->GetWorld(), matView, matProj, iter->GetModel()->GetTexture(),
			lightClass->GetDirection(), lightClass->GetAmbientColor(), lightClass->GetDiffuseColor(), camPos,
			lightClass->GetSpecularColor(), lightClass->GetSpecularPower(), pointDiffuseColor, lightPosition);
	}
}

void GameObjectMgr::EraseGameObject(GameObject * pGameObject)
{
	if (m_GameObjectLst.size() == 0
		|| !pGameObject)
		return;

	for (auto iter = m_GameObjectLst.begin(); iter != m_GameObjectLst.end(); )
	{
		if (*iter == pGameObject)
		{
			delete *iter;
			*iter = 0;
			iter = m_GameObjectLst.erase(iter);
		}
		else
			++iter;
	}
}

void GameObjectMgr::Shutdown()
{
	if (m_GameObjectLst.size() == 0)
	{
		m_GameObjectLst.clear();
		return;
	};

	for (auto iter : m_GameObjectLst)
	{
		if (iter)
		{
			delete iter;
			iter = 0;
		}
	}

	m_GameObjectLst.clear();
}

int GameObjectMgr::GetPolySize()
{
	int Polycnt = 0;
	for (auto iter : m_GameObjectLst)
	{
		if (iter)
		{
			switch (iter->GetTag())
			{
			case GameObject::TAG_AI:
			{
				AIClass* tempAI = dynamic_cast<AIClass*>(iter);
				if (tempAI->GetObjType() == AIClass::OBJ_BOAT)
					break;

				switch (tempAI->GetObjType())
				{
				case AIClass::OBJ_CAR:
					Polycnt += tempAI->GetModel(AIClass::OBJ_CAR)->GetIndexCount() / 3;
					continue;
				case AIClass::OBJ_UNICORN:
					Polycnt += tempAI->GetModel(AIClass::OBJ_UNICORN)->GetIndexCount() / 3;
					continue;
				}
			}
			break;
			case GameObject::TAG_UI:
			{
				if (dynamic_cast<UIClass*>(iter)->GetUIID() == UIClass::UI_TIMER)
					Polycnt += dynamic_cast<UIClass*>(iter)->GetPolycnt();
			}
			continue;
			}

			Polycnt += (iter->GetModel()->GetIndexCount() / 3);
		}
	}

	return Polycnt;
}