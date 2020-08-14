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

	HRESULT Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev);
	void UseParticle(_vec3 vPos,PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo,_float fUseTime,_bool bUseBloom, _int iMaxParticle,_matrix* pParent = nullptr);

	void Get_Current_ViewProj(_matrix* pView, _matrix* pProj)
	{ *pView = m_matView; *pProj = m_matProj; }

	void Set_Current_ViewProj(_matrix matView, _matrix matProj) { m_matView = matView; m_matProj = matProj; }
	// CBase��(��) ���� ��ӵ�
	virtual void Free(void) override;

protected:
	vector<CEffectParticle*> m_vecParticle;

	_int m_iParticleMaxCount;

	_matrix m_matView;
	_matrix m_matProj;
};

END