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
	virtual HRESULT		LateInitialize(void);


	virtual _int		Update(const _float& fTimeDelta);
	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta, SUBSET::RENDER eRenderSel)PURE;
	virtual void		Render_Collider(LPD3DXEFFECT & pEffect, _matrix matView, _matrix matProj);

	virtual void		Render_PostProcess(const _float& fTimeDelta, SUBSET::RENDER eRenderSel);
	virtual void		Render_DepthProjection(const _float& fTimeDelta, SUBSET::RENDER eRenderSel);
	virtual void		Render_Shadow(const _float& fTimeDelta, SUBSET::RENDER eRenderSel);
	virtual void		Render_Screen(const _float& fTimeDelta);
	virtual void		Render_Refract(const _float& fTimeDelta, SUBSET::RENDER eRenderSel);

	virtual void		Render_PBR_Debug(const _float& fTimeDelta);

	void				Set_Dead() { m_bIsDead = true; }
	_bool				Get_Dead() { return m_bIsDead; }

	void				Set_Hp(_int iHp) { m_iHp = iHp; }
	_int*				Get_Hp() { return &m_iHp; }

	void				Set_ColMsg(ColMsg tColMsg) { m_vecColMsg.push_back(tColMsg); }
	ColMsg				Get_ColMsg(_int iIdx = 0) { return m_vecColMsg[iIdx]; }
	vector<ColMsg>&		Get_ColVector() { return m_vecColMsg; }
	_int				Get_MyColMsg()				{ return m_iMyColMsg; }
	void				Set_MyColMsg(_int iMyColMsg) { m_iMyColMsg = iMyColMsg; }

	OBJ_ID				Get_ID() { return m_eMyID; }
	void				Set_ID(OBJ_ID eID) { m_eMyID = eID; }

	_bool				Get_IsCol() { return m_bIsCol; }
	void				Set_IsCol(_bool bIsCol) { m_bIsCol = bIsCol; }

	_float				Get_MyDeltaTime() { return m_fMyDeltaTime; }
	void				Set_MyDeltaTime(_float fTime) { m_fMyDeltaTime = fTime; }


	void				Collider_Initialize();
protected:
	LPDIRECT3DDEVICE9								m_pGraphicDev;
	typedef map<const COMPONENT::TAG, CComponent*>	COMPONENTS;
	COMPONENTS										m_Components[COMPONENT::ID::ID_END];

	_int											m_iHp;
	_ulong											m_dwIndex;
	_float											m_fViewZ;
	_bool											m_bIsDead;

	vector<ColMsg>									m_vecColMsg;
	
	_int											m_iMyColMsg;
										
	vector<ENGINE::CCollider*>						m_vecCollider;
	_bool											m_bIsDraw;

	OBJ_ID											m_eMyID;

	_float											m_fMyDeltaTime;

	_bool											m_bIsCol;
public:
	virtual void	Free(void);

};

END
#endif // GameObject_h__
