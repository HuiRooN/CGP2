#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "GameObject.h"

class InputClass;

class PlayerClass : public GameObject
{
public:
	PlayerClass();
	PlayerClass(const PlayerClass&);
	~PlayerClass();

public:
	void SetInputClass(InputClass* input) { m_Input = input; }
	void SetTouchedColl(Collision* coll) { m_TouchedColl = coll; }
	void SetCanMove(bool canMove) { m_bCanMove = canMove; }
	void SetDead(bool dead) { m_bDead = dead; }
	
	Collision* GetTouchedColl() { return m_TouchedColl; }
	bool GetCanMove() { return m_bCanMove; }
	bool GetDead() { return m_bDead; }

	void ReInit();

public:
	virtual bool Frame(float);

private:
	void Initialize();
	void Move(float);
	void Shutdown();

private:
	InputClass* m_Input;
	Collision* m_TouchedColl;

	bool m_bCanMove;
	bool m_bDead;

	float m_fSpeed;
};

