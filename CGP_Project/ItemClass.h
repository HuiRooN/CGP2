#pragma once

#include "GameObject.h"

class D3DClass;
class LightShaderClass;
class LightClass;

class ItemClass :
	public GameObject
{
public:
	enum ITEMTYPE {ITEM_APPLE, ITEM_CAKE, ITEM_HAMBURGER, ITEM_END};
	enum LINE { LINE_1, LINE_2, LINE_3, LINE_4, LINE_5, LINE_6, LINE_7, LINE_END };

public:
	ItemClass();
	ItemClass(const ItemClass&);
	~ItemClass();

public:
	void SetInit(bool init) { m_bInit = init; }
	void SetLine(LINE eLine) { m_eLine = eLine; }
	void SetHidden(bool hidden) { m_bIsHidden = hidden; }

	bool GetHidden() { return m_bIsHidden; }

public:
	void InitializeObj(ID3D11Device*);
	virtual bool Frame(float);

private:
	void Initialize();
	void LateInit();
	void Shutdown();

private:
	ITEMTYPE m_eType;
	LINE m_eLine;

	bool m_bInit = false;
	bool m_bIsHidden = false;
};

