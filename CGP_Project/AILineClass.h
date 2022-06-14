#pragma once

#include "GameObject.h"

class AILineClass :
	public GameObject
{
public:
	enum LINE {LINE_1, LINE_2, LINE_3, LINE_4, LINE_5, LINE_6, LINE_7, LINE_8, LINE_END};

public:
	AILineClass();
	AILineClass(const AILineClass&);
	~AILineClass();

public:
	void SetLine(LINE eLine) { m_eLine = eLine; }

public:
	virtual bool Frame(float);

private:
	void Initialize();
	void Shutdown();

private:
	LINE m_eLine;

	bool m_bInit = false;
};

