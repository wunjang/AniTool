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
	virtual void		Render(const _float& fTimeDelta);

	virtual void		Render_PostProcess(const _float& fTimeDelta);

	void ParticleStartSetting(_vec3 vPos, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, _bool bUseBloom, _int iMaxParticle, _matrix* pParent = nullptr);

	_bool GetUseCheck() { return m_fLife > 0.f; };

protected:
	void BufferUpdate(VTXPARTICLEMATRIX* pVtxArry, _uint iSzie);
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
	VTXPARTICLEMATRIX* m_pVBArray;

	DWORD m_dwCurDrawCount;

	_int m_iMaxParticle;

	_float m_fCreateTimer;
public:
	static CEffectParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual void Free(void) override;
};

END