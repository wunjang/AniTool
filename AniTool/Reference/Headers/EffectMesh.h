#pragma once
#include "GameEffect.h"
BEGIN(ENGINE)
class CStaticMesh;

class ENGINE_DLL CEffectMesh :
	public CGameEffect
{
protected:
	CEffectMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectMesh();

protected:
	virtual HRESULT		Initialize(_float fLife,wstring wstrMeshKey, TEX_EFFECT_INFO tEffectInfo, EFFECTMESH_ATT tAtt, _vec3 vPos, _vec3 vScale, _vec3 vAngle, _matrix* pParent, _bool bCheckParentPosOnly, list<_int> listSubsetRenderOp);

	void AttributeUpdate(const _float & fTimeDelta);
public:
	virtual _int	Update(const _float& fTimeDelta);

	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel)override;

	virtual void		Render_PostProcess(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel);

	HRESULT SetUp_ConstantTable(LPD3DXEFFECT & pEffect, _float fTimeDelta);

	void UseEffect(_bool bCheck);
protected:
	CStaticMesh* m_pMesh;
	EFFECTMESH_ATT m_tAttribute;

	_bool m_bRender;
public:
	static CEffectMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fLife,wstring wstrMeshKey,TEX_EFFECT_INFO tEffectInfo, EFFECTMESH_ATT tAtt, _vec3 vPos, _vec3 vScale, _vec3 vAngle, _matrix* pParent, _bool bCheckParentPosOnly = false, list<_int> listSubsetRenderOp = list<_int>());
protected:
	virtual void Free(void) override;
};

END