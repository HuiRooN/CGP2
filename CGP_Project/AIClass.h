#pragma once

#include "GameObject.h"

class LightShaderClass;
class LightClass;

class AIClass :
	public GameObject
{
public:
	enum NUMTYPE {NUM_1, NUM_2, NUM_3, NUM_4, NUM_END};
	enum OBJTYPE {OBJ_BOAT, OBJ_CAR, OBJ_UNICORN, OBJ_END};
	enum LINE { LINE_1, LINE_2, LINE_3, LINE_4, LINE_5, LINE_6, LINE_7, LINE_8, LINE_END };

public:
	AIClass();
	AIClass(const AIClass&);
	~AIClass();

public:
	void SetNumType(NUMTYPE eType) { m_eNumType = eType; }
	void SetObjType(OBJTYPE eType) { m_eObjType = eType; }
	void SetLine(LINE eLine) { m_eLine = eLine; }
	void SetStartRight(bool bRight) { m_bStartRight = bRight; }
	void SetCanMove(bool bCanMove) { m_bCanMove = bCanMove; }

	NUMTYPE GetNumType() { return m_eNumType; }
	OBJTYPE GetObjType() { return m_eObjType; }
	LINE GetLine() { return m_eLine; }
	bool GetStartRight() { return m_bStartRight; }
	bool GetCanMove() { return m_bCanMove; }
	ModelClass* GetModel(OBJTYPE);
	
	void SetModels(ModelClass*, ModelClass*, ModelClass*);
	bool ReachMiddle();

public:
	virtual bool Frame(float);
	void Render(ID3D11DeviceContext*, D3DMATRIX, D3DXMATRIX, LightShaderClass*, LightClass*, D3DXVECTOR3, D3DXVECTOR4*, D3DXVECTOR4*);

private:
	void Initialize();
	void LateInit();
	void Shutdown();

	void Move(float);
	void ReachEnd();

private:
	NUMTYPE m_eNumType;
	OBJTYPE m_eObjType;
	LINE m_eLine;

	ModelClass* m_model2;
	ModelClass* m_model3;

	bool m_bInit = false;
	bool m_bReInit = false;
	bool m_bStartRight;
	bool m_bCanMove = false;

	D3DXMATRIX m_matSaveWorld;
	float m_fSpeed;
};

