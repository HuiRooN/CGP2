#pragma once

#include "GameObject.h"

class GoalClass : 
	public GameObject
{
public:
	GoalClass();
	GoalClass(const GoalClass&);
	~GoalClass();

public:
	virtual bool Frame(float);

private:
	void Initialize();
	void Shutdown();
};

