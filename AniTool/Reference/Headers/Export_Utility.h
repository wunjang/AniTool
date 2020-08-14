#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "RenderTargetMgr.h"
#include "Management.h"
#include "ResourceLoader.h"
#include "Transform.h"

#include "Renderer.h"
#include "Component_Manager.h"
#include "ShaderMgr.h"

//#include "PrototypeMgr.h"
//#include "Calculator.h"
#include "Collider.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"
//#include "Optimization.h"
#include "LightMgr.h"
//#include "Shader.h"
#include "EffectMgr.h"

BEGIN(ENGINE)

// Management
// Get
inline CComponent*			Get_Component(ENGINE::LAYER_TYPE eType,
	const  OBJ_ID eObjectID,
	const COMPONENT::TAG eComponentTag,
	COMPONENT::ID eID);


inline CGameObject*			Get_GameObject(ENGINE::LAYER_TYPE eType,
	const  OBJ_ID eObjectID,
	_int	iIndex = 0);

inline _int			Get_GameObjectCnt(ENGINE::LAYER_TYPE eType,
	const  OBJ_ID eObjectID
	);
// Set
inline HRESULT				SetUp_Scene(CScene* pScene);
// General
inline HRESULT				Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);
inline HRESULT				Add_GameObject(LAYER_TYPE eLayerType, const  OBJ_ID eObjectID,CGameObject* pObject);
inline HRESULT				LateAdd_GameObject(LAYER_TYPE eLayerType, const  OBJ_ID eObjectID, CGameObject* pObject);


// Renderer
// Get
// Set
// General
inline CRenderer*			Get_Renderer(void);
inline void					Set_DebugBuffer_Switch();
//
////// LightMgr
////// Get
inline const D3DLIGHT9*		Get_Light(const _uint& iIndex);
// Set
// General
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex, _matrix* pParent = nullptr);
inline void		Render_Light(LPD3DXEFFECT& pEffect);
HRESULT		Ready_PointLight(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, _matrix* pParent = nullptr);
void		Update_Light(const _float& fTimeDelta);
void		ResetLight();


// ComponentMgr
// Get
// Set
// General
inline HRESULT Reserve_Component_Manager(_uint iNumScenes);
inline 	CComponent* Clone_Component(_uint iSceneID, const wstring wstrPrototypeTag, void* pArg);
inline HRESULT Add_Prototype(_uint iSceneID, const wstring wstrPrototypeTag, CComponent * pPrototype);

///////////////////////////////////////////////////////////////////////////////////
// RenderTargetMgr
// Get
// Set
// General
inline HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
	RenderTargetTag eTargetTag,
	const _uint& iWidth,
	const _uint& iHeight,
	D3DFORMAT Format,
	D3DXCOLOR Color);
inline HRESULT			Ready_LuminanceTarget(LPDIRECT3DDEVICE9 pGraphicDev);
inline vector<CRenderTarget*> GetLuminanceTargetVector();

inline HRESULT			Ready_MRT(MRTTag eMRTTag, RenderTargetTag eTargetTag);
inline HRESULT			Clear_RenderTarget(RenderTargetTag eTargetTag);
inline HRESULT			Begin_MRT(MRTTag eMRTTag);
inline HRESULT			End_MRT(MRTTag eMRTTag);
inline HRESULT			Ready_DebugBuffer(RenderTargetTag eTargetTag,
	const _float& fX,
	const _float& fY,
	const _float& fSizeX,
	const _float& fSizeY);
inline 	HRESULT		SetUp_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev);


inline void			Render_DebugBuffer(MRTTag eMRTTag);

inline void			SetUp_OnShader(LPD3DXEFFECT& pEffect, RenderTargetTag eTargetTag, const char* pConstantName);
inline void			SetUp_OnRenderTargetDesc(D3DSURFACE_DESC* pDesc, RenderTargetTag eTargetTag);

inline CRenderTarget*			Find_RenderTarget(RenderTargetTag eTargetTag);


///////////////////////////////////////////////////////////////////////////////////
//ShaderMgr


//Initialize
inline HRESULT	InItManager(LPDIRECT3DDEVICE9 & pGraphicDev);
inline CShader*	Get_Shader(CShaderMgr::ShaderType eType);
inline void		Reset_Shader(CShaderMgr::ShaderType eType);
// Release Utility
inline void		Release_Utility(void);

///////////////////////////////////////////////////////////////////////////////////
//EffectMgr
inline 	void UseParticle(_vec3 vPos, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, _bool bUseBloom, _int iMaxParticle, _matrix* pParent = nullptr);
inline 	void Set_Current_ViewProj(_matrix matView, _matrix matProj);
inline 	void Get_Current_ViewProj(_matrix* pView, _matrix* pProj);
inline void Update_Manager(const _float& fTimeDelta);
inline void LateUpdate_Manager(const _float& fTimeDelta);
inline HRESULT Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev);
///////////////////////////////////////////////////////////////////////////////////
// Loader
inline HRESULT		Ready_Loading(LPDIRECT3DDEVICE9 pGraphicDev);
inline void			Add_Loading(function<_long(void)> fnLoading, const wstring& wstrLoadingMsg = L"Loading...", const wstring& wstrFinishMsg = L"Load Finish");
inline _ulong		Check_Loading();


#include "Export_Utility.inl"


END
#endif // Export_Utility_h__
