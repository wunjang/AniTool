#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "Component.h"
#include "GameObject.h"

BEGIN(ENGINE)
class CTexture;
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
	void	Add_ScreenRenderGroup(SCREEN_RENDER_ID eGroup, CGameObject* pGameObject);
	void	Render(LPDIRECT3DDEVICE9 & pGraphicDev, const _float& fTimeDelta);
	void	Render_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev, const _float& fTimeDelta, _int dwRenderTargetTag = RenderTargetTag_Albedo);
	void	Clear_RenderGroup(void);
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);


	void	Set_DebugBuffer_Switch() { m_bRenderDebug = !m_bRenderDebug; }

	void	SetIBL(wstring wstrTexKey);
	void	SwicthRenderPBR() {m_bRenderPBR = !m_bRenderPBR;}

private:
	/////////////////////////////////////////////////////////
	//Render Object
	void	Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Refraction(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_PostProcess(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Shadow(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_ObjectShadow(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_SSAO(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_UI(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_PBR_Debug(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_DepthProjection(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_ScreenCtrl(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);

	void	Render_Forward(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);

	/////////////////////////////////////////////////////////
	//Render PostProcess
	void	Render_Bloom(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_GodRay(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Smoke(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_Luminance(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);
	void	Render_MotionBlur(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);

	void	Render_Defferd(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_Fog(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	PostProcess(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_Blend(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);
	void	Render_BackBuffer(LPDIRECT3DDEVICE9& pGraphicDev, const _float & fTimeDelta);


	void	Draw_CurScreen(LPDIRECT3DDEVICE9& pGraphicDev);

	/////////////////////////////////////////////////////////
	//Render PASS
	void	Render_HorizontalBlur(LPDIRECT3DDEVICE9& pGraphicDev, LPD3DXEFFECT& pEffect, D3DSURFACE_DESC tSurface_Desc);
	void	Render_VerticalBlur(LPDIRECT3DDEVICE9& pGraphicDev, LPD3DXEFFECT& pEffect, D3DSURFACE_DESC tSurface_Desc);

	void	Render_DOF_HorizontalBlur(LPDIRECT3DDEVICE9& pGraphicDev, LPD3DXEFFECT& pEffect, D3DSURFACE_DESC tSurface_Desc);
	void	Render_DOF_VerticalBlur(LPDIRECT3DDEVICE9& pGraphicDev, LPD3DXEFFECT& pEffect, D3DSURFACE_DESC tSurface_Desc);

	//////////////////////////////////////////////////////////
	//Shader Managed
public:
	void	ResetMeshConstant();
private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];
	list<CGameObject*>			m_ScreenRenderGroup[SCREEN_END];
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_bool						m_bRenderDebug;
	_bool						m_bClick = false;
	_bool						m_bUseBloom;

	_bool						m_bRenderPBR;

	_float						m_fGamma;
	_float						m_fExposure;
	_float						m_fBrightPassThreshold;
	_float						m_fGaussianInterval;
	_float						m_fGaussianPower;

	////////////////////////////////
	//MotionBlur
	_int						m_iMotionBlurUpdateFrame;
	_int						m_iMotionBlurUpdateFre;
	////////////////////////////////
	_bool						m_bHalfRender_Shader;
	ENGINE::CTexture*			m_pIBLTex;
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
