#include "stdafx.h"
#include "Breakable.h"

CBreakable::CBreakable(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMapObject(pGraphicDev)
{
}


CBreakable::~CBreakable()
{
}

HRESULT CBreakable::Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	FAILED_CHECK_RETURN(CMapObject::Initialize(tInfo, wstrResourseName), E_FAIL);

	return S_OK;
}

_int CBreakable::Update(const _float & fTimeDelta)
{
	CMapObject::Update(fTimeDelta);

	return ENGINE::NO_EVENT;
}

_int CBreakable::LateUpdate(const _float & fTimeDelta)
{
	CMapObject::LateUpdate(fTimeDelta);

	return ENGINE::NO_EVENT;
}

void CBreakable::Render(const _float & fTimeDelta)
{
	CMapObject::Render(fTimeDelta);
}

void CBreakable::Render_PostProcess(const _float & fTimeDelta)
{
	CMapObject::Render_PostProcess(fTimeDelta);
}

HRESULT CBreakable::Add_Component(const wstring wstrResourseName)
{
	return S_OK;
}

HRESULT CBreakable::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	CMapObject::SetUp_ConstantTable(pEffect);

	return S_OK;
}

CBreakable * CBreakable::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	CBreakable* pInstance = new CBreakable(pGraphicDev);

	if (FAILED(pInstance->Initialize(tInfo, wstrResourseName)))
	{
		ENGINE::Safe_Release(pInstance);
	}
	return pInstance;
}

void CBreakable::Free(void)
{
	CMapObject::Free();
	if (m_pNavObj != nullptr)
	{
		ENGINE::Safe_Release(m_pNavObj);
	}
}
