#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>
#include <list>
#include <map>
using namespace std;

class D3DClass;
class GameObject;
class Collision;

class LightClass;
class LightShaderClass;
class TextureShaderClass;

class AIClass;

class GameObjectMgr
{
public:
	GameObjectMgr();
	GameObjectMgr(const GameObjectMgr&);
	~GameObjectMgr();

public:
	bool PushGameObject(GameObject* pGameObject);
	bool PushAI(int, vector<AIClass*>);
	GameObject* GetGameObject(int eTag, Collision* coll = nullptr);
	list<GameObject*> GetGameObjectListByTag(int eTag);
	
	bool Frame(float);
	bool AIFrame(float);
	void Render(D3DClass*, D3DMATRIX, D3DXMATRIX, TextureShaderClass*, LightShaderClass*, LightClass*, D3DXVECTOR3, D3DXVECTOR4*, D3DXVECTOR4*);
	
	void EraseGameObject(GameObject* pGameObject);
	void Shutdown();

	int GetSize() { return m_GameObjectLst.size(); }
	int GetPolySize();

private:
	list<GameObject*> m_GameObjectLst;
	map<int, vector<AIClass*>> m_mapAI;
};

