#pragma once
#include "Engine_Defines.h"
#include "Base.h"
BEGIN(ENGINE)
class CEffectParticle;
class ENGINE_DLL CEffectMgr :
	public CBase
{
	DECLARE_SINGLETON(CEffectMgr)

protected:
	CEffectMgr();
	virtual ~CEffectMgr();

public:
	void Update_Manager(const _float& fTimeDelta);
	void LateUpdate_Manager(const _float& fTimeDelta);
	void Reset_EffectManager();

	HRESULT Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev);
	CEffectParticle* UseParticle(CEffectParticle** ppParticle,_vec3 vPos, _vec3 vScale,_vec3 vAngle,PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo,_float fUseTime,_bool bUseBloom, _int iMaxParticle,_matrix* pParent = nullptr, _bool bParentPosOnly = false);
	CEffectParticle* UseMeshParticle(CEffectParticle** ppParticle, _vec3 vPos, _vec3 vScale, _vec3 vAngle, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, LPD3DXMESH pMesh, _int iVertexInterval, _matrix* pParent = nullptr, _bool bParentPosOnly = false);

	void Get_Current_ViewProj(_matrix* pView, _matrix* pProj)
	{
		if(pView)
		*pView = m_matView; 
		if(pProj)
		*pProj = m_matProj; 
	}

	void Set_Current_ViewProj(_matrix matView, _matrix matProj) { m_matView = matView; m_matProj = matProj; }
	// CBase을(를) 통해 상속됨
	virtual void Free(void) override;

	void Return_Particle(CEffectParticle** ppParticle);

protected:
	vector<CEffectParticle*> m_vecParticle;

	_int m_iParticleMaxCount;

	_matrix m_matView;
	_matrix m_matProj;
};

END