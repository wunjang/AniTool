#pragma once

#include "GameObject.h"

BEGIN(ENGINE)
class CTexture;
class CTransform;
class ENGINE_DLL CGameEffect :
	public CGameObject
{
public:
	enum Effect_Texture_Key {
		 EFFECT_TEX_DISTORTION
		, EFFECT_TEX_MASK
		, EFFECT_TEX_ALBEDO
		, EFFECT_TEX_DISOLVE
		, EFFECT_TEX_MOTIONBLUR
		, EFFECT_TEX_NORMAL
		, EFFECT_TEX_LUT
		, EFFECT_TEX_END
	};
protected:
	explicit CGameEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameEffect();
protected:
	virtual HRESULT				Initialize(_vec3 vPos,_vec3 vScale,_vec3 vAngle, _matrix* pParent, _bool bCheckParentPosOnly);

	void CreateTextureFromEffectStruct(Effect_Texture_Key eTexKey, wstring wstrTex);
	void SetTexture(LPD3DXEFFECT & pEffect);
	virtual HRESULT		Add_Component();
public:

	virtual _int				Update(const _float& fTimeDelta);

	_bool CheckLife() {return m_fLife > 0.f;}
protected:
	TEX_EFFECT_INFO				m_tTextureState;
	vector<CTexture*>			m_vecTexture;

	_float m_fLife;

	_bool m_bUsingBloom;

	CTransform* m_pTransform;
protected:
	virtual void Free()override;
};

END