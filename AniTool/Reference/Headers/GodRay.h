#ifndef GodRay_h__
#define GodRay_h__
#pragma once
#include "Engine_Defines.h"
#include "Base.h"
BEGIN(ENGINE)
class ENGINE_DLL CGodRay :
	public CBase
{
	protected:
	explicit CGodRay(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGodRay();

public:
	HRESULT			Ready_Light();
	int			Update_Light(const _float& fTimeDelta);
	void			Set_Focus(_vec3 vWorldPos);
	void			Render_GodRay(LPD3DXEFFECT& pEffect);

	_uint			GetIndex() { return m_iIndex; }

	void		Set_GodRay_RenderInfo(LPD3DXEFFECT& pEffect);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_uint					m_iIndex;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	GOD_RAY_INFO m_tInfo;

	_matrix		m_matWorld;
	_matrix*	m_pParent;

	_bool		m_bDead;

	list<_vec2> m_listFocusProjPos;
public:
	static CGodRay*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free(void);
};

END
#endif // Light_h__
