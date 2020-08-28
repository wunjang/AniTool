#pragma once

#include "GameEffect.h"
BEGIN(ENGINE)
class CParticleBuffer;
class ENGINE_DLL CEffectParticle :
	public CGameEffect
{
protected:
	CEffectParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectParticle();

protected:
	virtual HRESULT		Initialize();
public:
	virtual _int	Update(const _float& fTimeDelta);

	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel);

	virtual void		Render_PostProcess(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel);

	void ParticleStartSetting(_vec3 vPos, _vec3 vScale,_vec3 vAngle, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, _bool bUseBloom, _int iMaxParticle, _matrix* pParent = nullptr, _bool bParentPosOnly = false);

	void ParticleArrayStartSetting(_vec3 vPos, _vec3 vScale, _vec3 vAngle, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, LPD3DXMESH pMesh,_int iVertexInterval, _matrix* pParent = nullptr, _bool bParentPosOnly = false);

	void Return_Particle();

	void SetUseCheck(_bool bCheck) { m_bUse = bCheck; }
	_bool GetUseCheck() { return m_bUse; };

	void UseEffect(_bool bRender) { m_bRender = bRender; }
	//반환값은 리턴이 됐으면 nullptr,안되면 자기자신 
protected:
	void BufferUpdate(VTXPTTEX* pVtxArry, _uint iSzie);
	void ParticleReset(PARTICLE_ATT* pInfo);
	void ParticleAttributeUpdate(const _float& fTimeDelta);
	void CreateParticle(const _float & fTimeDelta);
	_bool DeadCheck(PARTICLE_ATT tInfo) { return tInfo.fLife > 0.f; }

	HRESULT SetUp_ConstantTable(LPD3DXEFFECT & pEffect,_float fTimeDelta);



protected:
	PARTICLE_EFFECT_INFO m_tParticleInfo;
	list<PARTICLE_ATT> m_listParticle;
	list<PARTICLE_ATT> m_listDeadParticle;

	CParticleBuffer* m_pBuffer;
	VTXPTTEX* m_pVBArray;
	DWORD m_dwCurDrawCount;

	_int m_iMaxParticle;

	_float m_fCreateTimer;

	_bool m_bUse;

	_bool m_bRender;
public:
	static CEffectParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual void Free(void) override;
};

END