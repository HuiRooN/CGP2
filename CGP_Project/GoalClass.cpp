#include "GoalClass.h"

#include "Collision.h"

GoalClass::GoalClass()
{
	m_Collision = new Collision;
	m_Collision->InitAABB(Collision::COL_GOAL, GetPos(), D3DXVECTOR3(0.3f, 0.3f, 0.3f));
}

GoalClass::GoalClass(const GoalClass &other)
{
}

GoalClass::~GoalClass()
{
}

bool GoalClass::Frame(float)
{
	if (m_Collision)
		m_Collision->UpdateAABB(GetPos());
	
	return true;
}

void GoalClass::Initialize()
{
}

void GoalClass::Shutdown()
{
}
