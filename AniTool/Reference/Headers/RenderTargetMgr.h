#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "RenderTarget.h"

BEGIN(ENGINE)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

public:
private:
	explicit	CRenderTargetMgr(void);
	virtual		~CRenderTargetMgr(void);

public:
	HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
										RenderTargetTag eTargetTag,
										const _uint& iWidth,
										const _uint& iHeight,
										D3DFORMAT Format,
										D3DXCOLOR Color,
										CRenderTarget** ppOutputTarget = nullptr);
	HRESULT			Ready_LuminanceTarget(LPDIRECT3DDEVICE9 pGraphicDev);


	HRESULT			Ready_MRT(MRTTag eMRTTag, RenderTargetTag eTargetTag);

	HRESULT			Begin_MRT(MRTTag eMRTTag);
	HRESULT			Clear_RenderTarget(RenderTargetTag eTargetTag);
	HRESULT			End_MRT(MRTTag eMRTTag);

	HRESULT			Ready_DebugBuffer(RenderTargetTag eTargetTag,
									const _float& fX, 
									const _float& fY,
									const _float& fSizeX,
									const _float& fSizeY);

	void			Render_DebugBuffer(MRTTag eMRTTag);
	HRESULT		SetUp_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev);

		
	CRenderTarget*			Find_RenderTarget(RenderTargetTag eTargetTag);

	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, RenderTargetTag eTargetTag, const char* pConstantName);
	void			SetUp_OnRenderTargetDesc(D3DSURFACE_DESC* pDesc, RenderTargetTag eTargetTag);

	vector<CRenderTarget*> GetLuminanceTargetVector() { return m_vecLuminanceTarget; }
private:
	HRESULT					Ready_Manager();
	list<CRenderTarget*>*	Find_MRT(MRTTag eMRTTag);


private:
	vector<CRenderTarget*>		m_vecRenderTarget;

	vector<list<CRenderTarget*>>	m_vecMRT;

	vector<CRenderTarget*>		m_vecLuminanceTarget;
	

private:
	virtual void Free(void);
};

END
#endif // RenderTargetMgr_h__
