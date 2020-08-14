#pragma once

#include "GameObject.h"

BEGIN(ENGINE)
class CTexture;
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
		, EFFECT_TEX_END
	};
protected:
	explicit CGameEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameEffect();
protected:
	virtual HRESULT				Initialize(_vec3 vPos,_vec3 vScale,_vec3 vAngle, _matrix* pParent);

	void CreateTextureFromEffectStruct(Effect_Texture_Key eTexKey, wstring wstrTex);
	void SetTexture(LPD3DXEFFECT & pEffect);
public:
	//const _vec3*				Get_Scale();
	//void						Get_Scale(_vec3* pVector);

	//const _vec3*				Get_Info(INFO eType);
	//void						Get_Info(INFO eType, _vec3* pVector);

	//const _vec3*				Get_Angle();
	//void						Get_Angle(_vec3* pVector);

	void						Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void						Set_Scale(const _vec3* vScale);

	void						Set_Angle(ROTATION eType, const _float& fAngle);
	void						Set_Angle(_vec3& vAngle);




	void						Set_Info(INFO eType, _vec3* pVector);
	void						Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void						Set_Pos(const _vec3* pPos);


	void						Move_Pos(const _vec3* pDir, const _float& fTimeDelta, const _float& fSpeed);
	void						Rotation(ROTATION eType, const _float& fAngle);
	virtual _int				Update(const _float& fTimeDelta);
	void						PositionUpdate(const _float& fTimeDelta);
protected:
	TEX_EFFECT_INFO				m_tTextureState;
	CTexture*					m_pDistortion;
	CTexture*					m_pMask;
	CTexture*					m_pAlbedo;
	CTexture*					m_pDisolve;
	CTexture*					m_pMotionBlur;
	vector<CTexture*>			m_vecTexture;

	_vec3						m_vInfo[INFO_END];
	_vec3						m_vScale;
	_vec3						m_vAngle;
	_matrix						m_matWorld;

	_matrix*					m_pParent;

	_float m_fLife;

	_bool m_bUsingBloom;


protected:
	virtual void Free()override;
};

END