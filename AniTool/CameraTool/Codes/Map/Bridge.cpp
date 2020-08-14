#include "stdafx.h"
#include "Bridge.h"

CBridge::CBridge(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMapObject(pGraphicDev)
{
}


CBridge::~CBridge()
{
}

HRESULT CBridge::Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	FAILED_CHECK_RETURN(CMapObject::Initialize(tInfo, wstrResourseName), E_FAIL);

	return S_OK;
}

_int CBridge::Update(const _float & fTimeDelta)
{
	CMapObject::Update(fTimeDelta);

	return ENGINE::NO_EVENT;
}

_int CBridge::LateUpdate(const _float & fTimeDelta)
{
	CMapObject::LateUpdate(fTimeDelta);

	return ENGINE::NO_EVENT;
}

void CBridge::Render(const _float & fTimeDelta)
{
	CMapObject::Render(fTimeDelta);
}

void CBridge::Render_PostProcess(const _float & fTimeDelta)
{
	CMapObject::Render_PostProcess(fTimeDelta);
}

HRESULT CBridge::Add_Component(const wstring wstrResourseName)
{
	return S_OK;
}

HRESULT CBridge::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	CMapObject::SetUp_ConstantTable(pEffect);

	return S_OK;
}

CBridge * CBridge::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	CBridge* pInstance = new CBridge(pGraphicDev);

	if (FAILED(pInstance->Initialize(tInfo, wstrResourseName)))
	{
		ENGINE::Safe_Release(pInstance);
	}
	return pInstance;
}

void CBridge::Free(void)
{
	CMapObject::Free();
	if (m_pNavObj != nullptr)
	{
		ENGINE::Safe_Release(m_pNavObj);
	}
}
