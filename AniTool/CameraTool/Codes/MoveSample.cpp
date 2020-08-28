#include "stdafx.h"
#include "MoveSample.h"


CMoveSample::CMoveSample(LPDIRECT3DDEVICE9 pGraphicDev)
	:ENGINE::CGameObject(pGraphicDev)
{
}


CMoveSample::~CMoveSample()
{
}

HRESULT CMoveSample::Initialize(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	return S_OK;
}

_int CMoveSample::Update(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update(fTimeDelta);


	return iExit;
}

_int CMoveSample::LateUpdate(const _float & fTimeDelta)
{
	return 0;
}

void CMoveSample::Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel)
{
	
}

HRESULT CMoveSample::Add_Component()
{
	ENGINE::CComponent*		pComponent = nullptr;


	return S_OK;
}

void CMoveSample::Input_Key(const _float & fTimeDelta)
{
	
}

CMoveSample * CMoveSample::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoveSample*	pInstance = new CMoveSample(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CMoveSample::Free(void)
{
	CGameObject::Free();
}
