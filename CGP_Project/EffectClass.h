#pragma once
#include "GameObject.h"

class PlayerClass;
class TextureShaderClass;
class LightShaderClass;
class LightClass;
class GameObjectMgr;

class EffectClass :
	public GameObject
{
public:
	enum EFFECTID {EFFECT_PARTI, EFFECT_SPARKL, EFFECT_END};

public:
	EffectClass();
	EffectClass(const EffectClass&);
	~EffectClass();

public:
	void SetEffectID(EFFECTID eID) { m_eID = eID; }
	void SetDead(bool dead) { m_bDead = dead; }

	void SetPlayer(PlayerClass* player) { m_Player = player; }
	void SetGameObjectMgr(GameObjectMgr* gameObjectMgr) { m_GameObjectMgr = gameObjectMgr; }
	
	EFFECTID GetEffectID() { return m_eID; }

public:
	virtual bool Frame(float);
	void Render(ID3D11DeviceContext*, TextureShaderClass*, D3DXMATRIX, D3DXMATRIX);
	void Render(ID3D11DeviceContext*, D3DMATRIX, D3DXMATRIX, LightShaderClass*, LightClass*, D3DXVECTOR3, D3DXVECTOR4*, D3DXVECTOR4*);

private:
	void Initialize();
	void Shutdown();
	
	void LateInit();
	void Move(float);

private:
	EFFECTID m_eID;
	bool m_bInit = false;
	bool m_bDead = false;

	PlayerClass* m_Player;
	GameObjectMgr* m_GameObjectMgr;

	D3DXVECTOR3 m_vDir;
	float m_frameTime;
	float m_floatTime;
};

