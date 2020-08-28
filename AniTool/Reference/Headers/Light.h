#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);

public:
	HRESULT			Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex, _matrix* pParent);
	int			Update_Light(const _float& fTimeDelta);
	void			Render_Light(LPD3DXEFFECT& pEffect);
	const D3DLIGHT9*		Get_Light(void) { return &m_tLightInfo; }
	_uint			GetIndex() { return m_iIndex; }

	void			SetLightCam(LPD3DXEFFECT & pEffect);
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	D3DLIGHT9				m_tLightInfo;
	_uint					m_iIndex;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_matrix		m_matWorld;
	_matrix		m_matProj;
	_matrix*	m_pParent;

	_bool		m_bDead;
public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex, _matrix* pParent);
private:
	virtual void	Free(void);

};

END
#endif // Light_h__
