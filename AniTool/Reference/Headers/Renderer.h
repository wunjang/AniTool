#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "Component.h"
#include "GameObject.h"

BEGIN(ENGINE)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	//explicit CRenderer(const CRenderer& rhs);

	virtual ~CRenderer(void);

public:
	void	Set_GraphicDev(LPDIRECT3DDEVICE9 pGraphic_Device)
	{
		m_pGraphic_Device = pGraphic_Device;
		Safe_AddRef(m_pGraphic_Device);
	}
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);


	void	Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render(LPDIRECT3DDEVICE9 & pGraphicDev, const _float& fTimeDelta);
	void	Render_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev, const _float& fTimeDelta, _int dwRenderTargetTag = RenderTargetTag_Albedo);
	void	Clear_RenderGroup(void);
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);


	void	Set_DebugBuffer_Switch() { m_bRenderDebug = !m_bRenderDebug; }
private:
	void	Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_PostProcess(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_SSAO(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_UI(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);

	void	Render_Bloom(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Luminance(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);

	void	Render_Defferd(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	PostProcess(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_Blend(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_BackBuffer(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);

	void	Draw_CurScreen(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_bool						m_bRenderDebug;
	_bool						m_bClick = false;
	_bool						m_bUseBloom;

	_float						m_fGamma;
	_float						m_fExposure;
	_float						m_fBrightPassThreshold;
	_float						m_fGaussianInterval;
	_float						m_fGaussianPower;
public:
	virtual		CComponent*		Clone(void* pArg)
	{
		AddRef();
		return this;
	}

private:
	virtual		void	Free(void);
};

END
#endif // Renderer_h__
