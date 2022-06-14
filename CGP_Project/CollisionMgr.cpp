#include "CollisionMgr.h"

#include "Collision.h"

CollisionMgr::CollisionMgr()
{
}

CollisionMgr::~CollisionMgr()
{
	Shutdown();
}

bool CollisionMgr::PushCollObject(Collision *coll)
{
	m_listCollision[coll->Get_Tag()].push_back(coll);

	return true;
}

bool CollisionMgr::UpdateCollsion(Collision::COLLISION_TAG coll, Collision::COLLISION_TAG other)
{
	if (m_listCollision[coll].size() == 0)
		return false;
	if (m_listCollision[other].size() == 0)
		return false;

	for (auto iter : m_listCollision[coll])
	{
		for (auto otherIter : m_listCollision[other])
		{			
			if (CheckAABB(iter, otherIter))
			{
				m_TouchedColl = otherIter;
				return true;
			}
		}
	}
	return false;
}

void CollisionMgr::EraseGameObject(Collision::COLLISION_TAG _eTag, Collision** pObject)
{
	if (m_listCollision[_eTag].size() == 0 || !pObject)
		return;

	auto iter = find(m_listCollision[_eTag].begin(), m_listCollision[_eTag].end(), *pObject);

	if (iter == m_listCollision[_eTag].end())
		return;

	delete *pObject;
	*pObject = 0;
	m_listCollision[_eTag].erase(iter);
}

void CollisionMgr::Shutdown()
{
	for (int i = Collision::COL_PLAYER; i < Collision::COL_END; i++)
	{
		if (m_listCollision[i].size() == 0)
		{
			m_listCollision[i].clear();
		}
	
		for (auto iter : m_listCollision[i])
		{
			if (iter)
			{
				delete iter;
				iter = 0;
			}
		}

		m_listCollision[i].clear();
	}
}

bool CollisionMgr::CheckAABB(Collision * A, Collision * B)
{
	if (A->GetMax().x < B->GetMin().x || A->GetMin().x > B->GetMax().x) return false;
	if (A->GetMax().y < B->GetMin().y || A->GetMin().y > B->GetMax().y) return false;
	if (A->GetMax().z < B->GetMin().z || A->GetMin().z > B->GetMax().z) return false;

	return true;
}
