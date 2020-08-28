#include "Export_Utility.h"

// Management
// Get

ENGINE::CComponent*		Get_Component(
	ENGINE::LAYER_TYPE eType,
	const  OBJ_ID eObjectID,
	const COMPONENT::TAG eComponentTag,
	COMPONENT::ID eID)
{
	return CManagement::GetInstance()->Get_Component(eType, eObjectID, eComponentTag, eID);
}

inline CScene * Get_Scene()
{
	return CManagement::GetInstance()->Get_Scene();
}

ENGINE::CGameObject * Get_GameObject(
	ENGINE::LAYER_TYPE eType,
	const  OBJ_ID eObjectID,
	_int iIndex)
{
	return CManagement::GetInstance()->Get_GameObject(eType, eObjectID, iIndex);
}

inline _int Get_GameObjectCnt(ENGINE::LAYER_TYPE eType, const  OBJ_ID eObjectID)
{
	return CManagement::GetInstance()->Get_GameObjectCnt(eType, eObjectID);
}


// Set
HRESULT		SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}
// General
HRESULT	Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;
}

inline HRESULT Add_GameObject(LAYER_TYPE eLayerType, const  OBJ_ID eObjectID, CGameObject * pObject)
{
	return CManagement::GetInstance()->Add_GameObject(eLayerType, eObjectID, pObject);
}

inline HRESULT LateInitialize()
{
	return CManagement::GetInstance()->LateInitialize();
}

inline HRESULT Release_Scene()
{
	return CManagement::GetInstance()->Release_Scene();
}





// Renderer
// Get
// Set
// General
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

inline void Set_DebugBuffer_Switch()
{
	CRenderer::GetInstance()->Set_DebugBuffer_Switch();
}

inline void ResetMeshConstant()
{
	ENGINE::CRenderer::GetInstance()->ResetMeshConstant();
}

inline const D3DLIGHT9 * Get_Light(const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}

inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex, _matrix * pParent)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex, pParent);
}

inline void Render_Light(LPD3DXEFFECT & pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);

}

inline HRESULT Ready_PointLight(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, _matrix * pParent)
{
	return CLightMgr::GetInstance()->Ready_PointLight(pGraphicDev, pLightInfo, pParent);
}

inline void Update_Light(const _float & fTimeDelta)
{
	CLightMgr::GetInstance()->Update_Light(fTimeDelta);

}

inline void ResetLight()
{
	CLightMgr::GetInstance()->ResetLight();

}

inline void Set_GodRay_Focus(_vec3 vPos)
{
	CLightMgr::GetInstance()->Set_GodRay_Focus(vPos);

}


//
//// LightMgr
//// Get
//const D3DLIGHT9*		Get_Light(const _uint& iIndex)
//{
//	return CLightMgr::GetInstance()->Get_Light(iIndex);
//}
//// Set
//// General
//HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
//{
//	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
//}
//void		Render_Light(LPD3DXEFFECT& pEffect)
//{
//	CLightMgr::GetInstance()->Render_Light(pEffect);
//}
//
//// PrototypeMgr
//// Get
//// Set
//// General
//HRESULT		Ready_Prototype(const wstring pProtoKey, CComponent* pInstance)
//{
//	return CPrototypeMgr::GetInstance()->Ready_Prototype(pProtoKey, pInstance);
//}
//CComponent*	Clone_Prototype(const wstring pProtoKey)
//{
//	return CPrototypeMgr::GetInstance()->Clone_Prototype(pProtoKey);
//}
//
//

// ComponentMgr
// Get
// Set
// General

HRESULT		 Reserve_Component_Manager(_uint iNumScenes)
{
	return CComponent_Manager::GetInstance()->Reserve_Component_Manager(iNumScenes);
}
HRESULT		 Add_Prototype(_uint iSceneID, const wstring pPrototypeTag, CComponent * pPrototype)
{
	return CComponent_Manager::GetInstance()->Add_Prototype(iSceneID, pPrototypeTag, pPrototype);
}

inline CComponent * Clone_Component(_uint iSceneID, const wstring wstrPrototypeTag, void * pArg)
{
	return CComponent_Manager::GetInstance()->Clone_Component(iSceneID,wstrPrototypeTag,pArg);
}






//
//// RenderTargetMgr
//// Get
//// Set
HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, RenderTargetTag eTargetTag, const _uint & iWidth, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, eTargetTag, iWidth, iHeight, Format, Color);
}

inline HRESULT Ready_LuminanceTarget(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return CRenderTargetMgr::GetInstance()->Ready_LuminanceTarget(pGraphicDev);
}

inline vector<CRenderTarget*> GetLuminanceTargetVector()
{
	return CRenderTargetMgr::GetInstance()->GetLuminanceTargetVector();
}

HRESULT Ready_MRT(MRTTag eMRTTag, RenderTargetTag eTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(eMRTTag, eTargetTag);
}
inline HRESULT Clear_RenderTarget(RenderTargetTag eTargetTag, _bool bCheckClearZBuffer)
{
	return CRenderTargetMgr::GetInstance()->Clear_RenderTarget(eTargetTag, bCheckClearZBuffer);
}
HRESULT Begin_MRT(MRTTag eMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(eMRTTag);
}

HRESULT End_MRT(MRTTag eMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(eMRTTag);
}

HRESULT Ready_DebugBuffer(RenderTargetTag eTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(eTargetTag, fX, fY, fSizeX, fSizeY);
}

inline HRESULT SetUp_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	return CRenderTargetMgr::GetInstance()->SetUp_RenderTarget(pGraphicDev);
}

void Render_DebugBuffer(MRTTag eMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(eMRTTag);

}

inline void SetUp_OnShader(LPD3DXEFFECT & pEffect, RenderTargetTag eTargetTag, const char * pConstantName)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnShader(pEffect, eTargetTag, pConstantName);

}

inline void SetUp_OnRenderTargetDesc(D3DSURFACE_DESC * pDesc, RenderTargetTag eTargetTag)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnRenderTargetDesc(pDesc, eTargetTag);
}

inline CRenderTarget*	Find_RenderTarget(RenderTargetTag eTargetTag)
{
	return 	CRenderTargetMgr::GetInstance()->Find_RenderTarget(eTargetTag);
	;
}

inline void Copy_RenderTarget(RenderTargetTag eDst, RenderTargetTag eSrc, LPD3DXEFFECT & pEffect)
{
	CRenderTargetMgr::GetInstance()->Copy_RenderTarget(eDst,eSrc,pEffect);

}

inline HRESULT InItManager(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CRenderer::GetInstance()->Ready_Renderer(pGraphicDev);
	CShaderMgr::GetInstance()->InitManager(pGraphicDev);

	return S_OK;
}

inline CShader * Get_Shader(CShaderMgr::ShaderType eType)
{
	return CShaderMgr::GetInstance()->Get_Shader(eType);
}

HRESULT		Ready_Loading(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return CResourceLoader::GetInstance()->Ready_Loading(pGraphicDev);
}
void		Add_Loading(function<_long(void)> fnLoading, const wstring& wstrLoadingMsg, const wstring& wstrFinishMsg)
{
	CResourceLoader::GetInstance()->Add_Loading(fnLoading, wstrLoadingMsg, wstrFinishMsg);

}
_ulong		Check_Loading()
{
	return CResourceLoader::GetInstance()->Check_Loading();

}


// Release Utility
void		Release_Utility(void)
{
	CFogMgr::GetInstance()->DestroyInstance();
	CResourceLoader::GetInstance()->DestroyInstance();
	CShaderMgr::GetInstance()->DestroyInstance();
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();

	CManagement::GetInstance()->DestroyInstance();
	CEffectMgr::GetInstance()->DestroyInstance();
	CComponent_Manager::GetInstance()->DestroyInstance();
	//CRenderer::GetInstance()->DestroyInstance();
}

inline CEffectParticle* UseParticle(CEffectParticle** ppParticle,_vec3 vPos, _vec3 vScale, _vec3 vAngle, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, _bool bUseBloom, _int iMaxParticle, _matrix * pParent, _bool bParentPosOnly )
{
	return CEffectMgr::GetInstance()->UseParticle(ppParticle,vPos, vScale, vAngle, tInfo, tTexInfo, fUseTime, bUseBloom, iMaxParticle, pParent, bParentPosOnly);
}

inline CEffectParticle * UseMeshParticle(CEffectParticle ** ppParticle, _vec3 vPos, _vec3 vScale, _vec3 vAngle, PARTICLE_EFFECT_INFO tInfo, TEX_EFFECT_INFO tTexInfo, _float fUseTime, LPD3DXMESH pMesh, _int iVertexInterval, _matrix * pParent, _bool bParentPosOnly)
{
	return CEffectMgr::GetInstance()->UseMeshParticle(ppParticle,vPos,vScale,vAngle,tInfo,tTexInfo,fUseTime,pMesh,iVertexInterval,pParent, bParentPosOnly);
}

inline void Set_Current_ViewProj(_matrix matView, _matrix matProj)
{
	CEffectMgr::GetInstance()->Set_Current_ViewProj(matView, matProj);
}

inline void Get_Current_ViewProj(_matrix * pView, _matrix * pProj)
{
	CEffectMgr::GetInstance()->Get_Current_ViewProj(pView, pProj);
}

inline void Update_Manager(const _float & fTimeDelta)
{
	CEffectMgr::GetInstance()->Update_Manager(fTimeDelta);
}

inline void LateUpdate_Manager(const _float & fTimeDelta)
{
	CEffectMgr::GetInstance()->LateUpdate_Manager(fTimeDelta);

}

inline HRESULT Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//if (FAILED_CHECK(CEffectMgr::GetInstance()->Ready_Manager(pGraphicDev)))
	//{
	//	return E_FAIL;
	//}
	CEffectMgr::GetInstance()->Ready_Manager(pGraphicDev);
	CLightMgr::GetInstance()->Ready_Manager(pGraphicDev);
	return S_OK;
}

inline void Reset_EffectManager()
{
	CEffectMgr::GetInstance()->Reset_EffectManager();
}

inline void Return_Particle(CEffectParticle** ppParticle)
{
	CEffectMgr::GetInstance()->Return_Particle(ppParticle);
}
