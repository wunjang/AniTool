#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"
#include "Collider.h"
BEGIN(ENGINE)
class CCollider;
class ENGINE_DLL CGameObject : public CBase
{



	
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);

public:
	CComponent*			Find_Component(const COMPONENT::TAG eComponentTag, COMPONENT::ID eID);
	CComponent*			Get_Component(const COMPONENT::TAG eComponentTag, COMPONENT::ID eID);
	_float				Get_ViewZ(void) const { return m_fViewZ; }

public:
	HRESULT				Add_Component(_uint eSceneID, const wstring pPrototypeTag, const COMPONENT::TAG eComponentTag, CComponent** ppOutComponent, COMPONENT::ID eComponentID = COMPONENT::ID::ID_STATIC, void* pArg = nullptr);
	HRESULT				Add_Component(const COMPONENT::TAG eComponentTag, CComponent* pComponent, COMPONENT::ID eComponentID = COMPONENT::ID::ID_STATIC);
	HRESULT				Replace_Component(const COMPONENT::TAG eComponentTag, CComponent* pNewComponent, CComponent** pOutOldComponent, COMPONENT::ID eComponentID);
	void				Compute_ViewZ(const _vec3* pPos);
	void				Set_Index(_ulong dwIndex)
	{
		m_dwIndex = dwIndex;
	}

public:
	virtual HRESULT		Initialize(void);
	virtual _int		Update(const _float& fTimeDelta);
	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta);

	virtual void		Render_PostProcess(const _float& fTimeDelta);

	void				Set_Dead() { m_bIsDead = true; }
	_bool				Get_Dead() { return m_bIsDead; }

	void				Set_ColMsg(ColMsg tColMsg) { m_vecColMsg.push_back(tColMsg); }
	ColMsg				Get_ColMsg(_int iIdx = 0) { return m_vecColMsg[iIdx]; }

	_int				Get_MyColMsg()				{ return m_iMyColMsg; }
	void				Set_MyColMsg(_int iMyColMsg) { m_iMyColMsg = iMyColMsg; }

protected:
	LPDIRECT3DDEVICE9								m_pGraphicDev;
	typedef map<const COMPONENT::TAG, CComponent*>	COMPONENTS;
	COMPONENTS										m_Components[COMPONENT::ID::ID_END];


	_int											m_iMaxHp = 10;
	_int											m_iHp = m_iMaxHp;
	_ulong											m_dwIndex = 0;
	_float											m_fViewZ = 0.f;
	_bool											m_bIsDead = false;

	vector<ColMsg>									m_vecColMsg;
	
	_int											m_iMyColMsg;
										
	vector<ENGINE::CCollider*>						m_vecCollider;

public:
	virtual void	Free(void);

};

END
#endif // GameObject_h__
