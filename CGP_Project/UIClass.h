#pragma once
#include "GameObject.h"

#include <vector>

class CameraClass;
class PlayerClass;
class TextureShaderClass;

class UIClass :
	public GameObject
{
public:
	enum UIID { UI_TIMERTEXT, UI_TIMER, UI_END };

public:
	UIClass();
	UIClass(const UIClass&);
	~UIClass();

public:
	void SetSceneID(int scene) { m_iSceneID = scene; }
	void SetUIID(UIID eID) { m_eUIID = eID; }
	
	void SetCamera(CameraClass* camera) { m_Camera = camera; }
	void SetPlayer(PlayerClass* player) { m_Player = player; }

	void SetTime(int timer) { m_iTimer = timer; }

	UIID GetUIID() { return m_eUIID; }
	int GetPolycnt();

public:
	void InitializeTexForNum(ID3D11Device*);
	virtual bool Frame(float);
	void Render(ID3D11DeviceContext*, TextureShaderClass*, D3DXMATRIX, D3DXMATRIX);

private:
	void Initialize();
	void ShutDown();
	
	void UpdateNumber();

private:
	int m_iSceneID;
	UIID m_eUIID;

	CameraClass* m_Camera;
	PlayerClass* m_Player;

	int m_iTimer;
	int m_iTimerCnt;

	vector<ModelClass*> m_vecNum;
};

