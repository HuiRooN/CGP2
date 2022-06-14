#pragma once
#include "GameObject.h"

class BoundaryClass :
	public GameObject
{
public:
	BoundaryClass();
	BoundaryClass(const BoundaryClass&);
	~BoundaryClass();

public:
	void SetIsLeft() { m_bIsLeft = true; }

public:
	virtual bool Frame(float);

private:
	void Initialize();
	void Shutdown();

private:
	bool m_bInit = false;
	bool m_bIsLeft = false;
};

