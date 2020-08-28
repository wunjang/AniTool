#pragma once
#include "GameEffect.h"
BEGIN(ENGINE)
class CVIBuffer;

class CScreenEffect :
	public CGameEffect
{
protected:
	CScreenEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScreenEffect();
protected:
	virtual HRESULT		Initialize(TEX_EFFECT_INFO tEffectInfo,SCREEN_ATT tAtt, _float fLife);

	void AttributeUpdate(const _float & fTimeDelta);
public:
	virtual _int	Update(const _float& fTimeDelta);

	virtual _int		LateUpdate(const _float& fTimeDelta);

	// CGameEffect을(를) 통해 상속됨
	virtual void Render(const _float & fTimeDelta, SUBSET::RENDER eRenderSel) override;

	HRESULT SetUp_ConstantTable(LPD3DXEFFECT & pEffect, _float fTimeDelta);


protected:
	vector<CGameEffect*> m_vecObject;

	SCREEN_ATT m_tAttribute;
public:
	static CScreenEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEX_EFFECT_INFO tEffectInfo, SCREEN_ATT tAtt,_float fLife);
protected:
	virtual void Free(void) override;

};

END