#include "stdafx.h"
#include "SmapleObject.h"


CSampleObject::CSampleObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:ENGINE::CGameObject(pGraphicDev)
{
}


CSampleObject::~CSampleObject()
{
}

HRESULT CSampleObject::Initialize(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CSampleObject::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSampleObject::LateUpdate(const _float & fTimeDelta)
{
	return 0;
}

void CSampleObject::Render(const _float & fTimeDelta)
{
}

HRESULT CSampleObject::Add_Component()
{
	ENGINE::CComponent*		pComponent = nullptr;

	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Renderer",		ENGINE::COMPONENT::TAG_RENDERER, (ENGINE::CComponent**)&m_pRendererCom), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Transform",		ENGINE::COMPONENT::TAG_TRANSFORM, (ENGINE::CComponent**)&m_pTransformCom, ENGINE::COMPONENT::ID_DYNAMIC), E_FAIL);




	return S_OK;
}

CSampleObject * CSampleObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSampleObject*	pInstance = new CSampleObject(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CSampleObject::Free(void)
{
	CGameObject::Free();
}
