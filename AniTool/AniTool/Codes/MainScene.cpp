#include "stdafx.h"
#include "MainScene.h"

#include "OrbitalCamera.h"
#include "SmapleObject.h"

CMainScene::CMainScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CScene(pGraphicDev)
{
}

CMainScene::~CMainScene(void)
{
	
}

HRESULT CMainScene::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(ENGINE::LAYER_ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(ENGINE::LAYER_GAMEOBJECT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(ENGINE::LAYER_UI), E_FAIL);

	return S_OK;
}

_int CMainScene::Update_Scene(const _float& fTimeDelta)
{
	ENGINE::CScene::Update_Scene(fTimeDelta);

	return 0;
}

void CMainScene::Render_Scene(void)
{
	// DEBUG 코드 출력 용도
}

CMainScene * CMainScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainScene *	pInstance = new CMainScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CMainScene::Free(void)
{
	ENGINE::CScene::Free();
}


HRESULT CMainScene::Ready_Environment_Layer(const ENGINE::LAYER_TYPE eLayerType)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	ENGINE::CGameObject*		pGameObject = nullptr;

	pGameObject = COrbitalCamera::Create(m_pGraphicDev, 60.f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::CAMERA, pGameObject), E_FAIL);

	m_Layers.emplace(eLayerType, pLayer);

	return S_OK;
}


HRESULT CMainScene::Ready_GameLogic_Layer(const ENGINE::LAYER_TYPE eLayerType)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	ENGINE::CGameObject*		pGameObject = nullptr;

	pGameObject = CSampleObject::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::OBJ_ID::PLAYER, pGameObject), E_FAIL);

	m_Layers.emplace(eLayerType, pLayer);

	return S_OK;
}

HRESULT CMainScene::Ready_UI_Layer(const ENGINE::LAYER_TYPE eLayerType)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	ENGINE::CGameObject*		pGameObject = nullptr;


	m_Layers.emplace(eLayerType, pLayer);

	return S_OK;
}

HRESULT CMainScene::Ready_Prototype(void)
{
	ENGINE::CComponent*		pInstance = nullptr;

	//FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_Body", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/body/", L"body.X")), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_Body", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/body_c/", L"Mario_Body.X")), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_Head", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/head/", L"head.X")), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_Face", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/face/", L"face.X")), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_Eye", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/eye/", L"eye.X")), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_HandL", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/handl/", L"handl.X")), E_FAIL);
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Mario_HandR", ENGINE::CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/Mario/handr/", L"handr.X")), E_FAIL);
	
	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Transform", ENGINE::CTransform::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Renderer", ENGINE::Get_Renderer()), E_FAIL);
	ENGINE::Get_Renderer()->Set_GraphicDev(m_pGraphicDev);

	return S_OK;
}

HRESULT CMainScene::Ready_LightInfo(void)
{



	return S_OK;
}

