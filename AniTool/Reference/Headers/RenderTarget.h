#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderTarget(void);
	
public:
	HRESULT		Ready_RenderTarget(const _uint& iWidth, 
									const _uint& iHeight,
									D3DFORMAT Format,
									D3DXCOLOR Color,
									_bool bUseDepth);

	void		SetUp_OnGraphicDev(const _uint& iIndex);
	void		Clear_RenderTarget(_bool bCheckClearZBuffer = false);
	void		Release_OnGraphicDev(const _uint& iIndex);

	HRESULT		Ready_DebugBuffer(const _float& fX,
										const _float& fY,
										const _float& fSizeX,
										const _float& fSizeY);
	HRESULT		Ready_TargetBuffer(const _float& fWidth, const _float& fHeight);
	void		RenderBuffer();


	void			Render_DebugBuffer(void);
	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const char* pConstantName);
	void			SetUp_OnRenderTargetDesc(D3DSURFACE_DESC* pDesc);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev; 

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	LPDIRECT3DVERTEXBUFFER9		m_pTargetVB;
	LPDIRECT3DINDEXBUFFER9		m_pTargetIB;

	LPDIRECT3DTEXTURE9			m_pTargetTexture;
	LPDIRECT3DSURFACE9			m_pTargetSurface;
	
	LPDIRECT3DSURFACE9			m_pOldTargetSurface;

	D3DXCOLOR					m_ClearColor;
	/////////////////////////////////////////////////
	//Depth Buffer
	_bool						m_bUseDepthBuffer;

	LPDIRECT3DTEXTURE9			m_pDepthTexture;
	LPDIRECT3DSURFACE9			m_pDepthSurface;

	LPDIRECT3DSURFACE9			m_pOldDepthSurface;


public:
	static CRenderTarget*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _uint& iWidth, 
										const _uint& iHeight,
										D3DFORMAT Format,
										D3DXCOLOR Color,
										_bool bUseDepth =false);

	virtual void Free(void);

};
END
#endif // RenderTarget_h__
