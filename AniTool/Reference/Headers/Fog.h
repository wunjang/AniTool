#pragma once
#include "Engine_Defines.h"
#include "Base.h"
BEGIN(ENGINE)
class  CTransform;

class ENGINE_DLL CFog :
	public CBase
{
protected:
	CFog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFog();
public:
	HRESULT			Ready_Fog(ENGINE::FOG::FOG_INFO tFogInfo,_vec3 vPos, const _uint& iIndex, _matrix* pParent);
	int			Update_Fog(const _float& fTimeDelta);
	void			Render_Fog(LPD3DXEFFECT& pEffect);
	const ENGINE::FOG::FOG_INFO		Get_FogInfo(void) { return m_tFogInfo; }
	_uint			GetIndex() { return m_iIndex; }

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_uint					m_iIndex;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	ENGINE::CTransform* m_pTransform;

	_bool		m_bDead;

	ENGINE::FOG::FOG_INFO	m_tFogInfo;
public:
	static CFog*		Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::FOG::FOG_INFO tFogInfo, _vec3 vPos, const _uint& iIndex, _matrix* pParent);
private:
	void Free();
};

END