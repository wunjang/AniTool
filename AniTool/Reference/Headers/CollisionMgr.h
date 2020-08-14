#ifndef __COLLISION_MGR_H__
#define __COLLISION_MGR_H__

#include "Engine_Defines.h"
#include "Collider.h"

BEGIN(ENGINE)

class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr(void);
	virtual ~CCollisionMgr(void);

public:
	void	Ready_CollisionMgr(const _uint& iMaxGroup);
	void	Add_CollisionGroup(const _uint& iGroup, CCollider* pCollider);
	void	Add_CheckingPair(_uint iSrc, _uint iDst);
	void	Check_Collision();
	void	Clear_CollisionGroup(void);

private:
	_uint m_iMaxGroup;
	list<CCollider*>*			m_listCollisionGroup;
	list<pair<_uint, _uint>>	m_listCheckingPair;

private:
	virtual		void	Free(void);
};

END
#endif // __COLLISION_MGR_H__
