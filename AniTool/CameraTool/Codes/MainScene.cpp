#include "stdafx.h"
#include "MainScene.h"

#include "FreeCamera.h"
#include "EventCamera.h"
#include "SmapleObject.h"
#include "HeightField.h"
#include "CameraPath.h"

#include "Function.h"

// Map
#include "Map\MapObject.h"

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

void CMainScene::Render_Scene(const _float& fTimeDelta)
{
	// DEBUG 코드 출력 용도
}

HRESULT CMainScene::Load_MapData(LPDIRECT3DDEVICE9 pGraphicDev, const ENGINE::LAYER_TYPE eLayerType, wstring wstrFilePath)
{
	// GetPathName - 파일 경로 얻어오는 함수. 
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;

	ENGINE::OBJ_ID eID;
	ENGINE::_WorldInfo tInfo;
	TCHAR szString[128] = L"";

	while (true)
	{
		ReadFile(hFile, &szString, sizeof(szString), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		ReadFile(hFile, &eID, sizeof(ENGINE::OBJ_ID), &dwByte, nullptr);
		ReadFile(hFile, &tInfo.vPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &tInfo.vAngle, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &tInfo.vScale, sizeof(D3DXVECTOR3), &dwByte, nullptr);

		ENGINE::Add_GameObject(ENGINE::LAYER_GAMEOBJECT, ENGINE::TERRAIN, CMapObject::Create(pGraphicDev, tInfo, szString));
	}
	CloseHandle(hFile);
	return S_OK;
}

void CMainScene::LoadTexture()
{
	map<CString, CString> mapTexFilePath;
	CFunction::FindFileByExtention(L".png", L"..\\Resource\\Texture", &mapTexFilePath);

	for (auto rTexPath : mapTexFilePath)
	{
		GET_INSTANCE(ENGINE::CComponent_Manager)->Add_Texture(m_pGraphicDev, ENGINE::SCENE_STATIC, rTexPath.first.GetString(), rTexPath.second.GetString());
	}
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

HRESULT CMainScene::Enter_Scene()
{
	return E_NOTIMPL;
}

HRESULT CMainScene::Ready_Light()
{
	return E_NOTIMPL;
}


HRESULT CMainScene::Ready_Environment_Layer(const ENGINE::LAYER_TYPE eLayerType)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_Layers.emplace(eLayerType, pLayer);

	ENGINE::CGameObject*		pGameObject = nullptr;

	pGameObject = CFreeCamera::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::CAMERA, pGameObject), E_FAIL);
	
	pGameObject = CEventCamera::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::CAMERA, pGameObject), E_FAIL);


	return S_OK;
}


HRESULT CMainScene::Ready_GameLogic_Layer(const ENGINE::LAYER_TYPE eLayerType)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_Layers.emplace(eLayerType, pLayer);

	ENGINE::CGameObject*		pGameObject = nullptr;

	pGameObject = CSampleObject::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::OBJ_ID::PLAYER, pGameObject), E_FAIL);
	
	pGameObject = CHeightField::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::OBJ_ID::TERRAIN, pGameObject), E_FAIL);
	
	pGameObject = CCameraPath::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENGINE::OBJ_ID::UI, pGameObject), E_FAIL);
	



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

	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Transform", ENGINE::CTransform::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Add_Prototype(0, L"Renderer", ENGINE::Get_Renderer()), E_FAIL);
	ENGINE::Get_Renderer()->Set_GraphicDev(m_pGraphicDev);

	FAILED_CHECK(ENGINE::Add_Prototype(0, L"TestTerrain", ENGINE::CVIBuffer_Terrain::Create(m_pGraphicDev, 1000, 1000, 10)));
	//GET_INSTANCE(ENGINE::CComponent_Manager)->Ready_AllTexture(m_pGraphicDev, L"../Data/ImagePath.txt");
	LoadTexture();
	GET_INSTANCE(ENGINE::CComponent_Manager)->Ready_AllMesh(m_pGraphicDev, L"../Data/MeshImagePath.txt");

	return S_OK;
}

HRESULT CMainScene::Ready_LightInfo(void)
{



	return S_OK;
}

