#include "stdafx.h"
#include "Decoration.h"

CDecoration::CDecoration(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMapObject(pGraphicDev)
{
}


CDecoration::~CDecoration()
{
}

HRESULT CDecoration::Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	FAILED_CHECK_RETURN(CMapObject::Initialize(tInfo, wstrResourseName), E_FAIL);

	return S_OK;
}

_int CDecoration::Update(const _float & fTimeDelta)
{
	CMapObject::Update(fTimeDelta);

	return ENGINE::NO_EVENT;
}

_int CDecoration::LateUpdate(const _float & fTimeDelta)
{
	CMapObject::LateUpdate(fTimeDelta);

	return ENGINE::NO_EVENT;
}

void CDecoration::Render(const _float & fTimeDelta)
{
	CMapObject::Render(fTimeDelta);
}

void CDecoration::Render_PostProcess(const _float & fTimeDelta)
{
	CMapObject::Render_PostProcess(fTimeDelta);
}

HRESULT CDecoration::Add_Component(const wstring wstrResourseName)
{
	return S_OK;
}

HRESULT CDecoration::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	CMapObject::SetUp_ConstantTable(pEffect);

	return S_OK;
}

CDecoration * CDecoration::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	CDecoration* pInstance = new CDecoration(pGraphicDev);

	if (FAILED(pInstance->Initialize(tInfo, wstrResourseName)))
	{
		ENGINE::Safe_Release(pInstance);
	}
	return pInstance;
}

void CDecoration::Free(void)
{
	CMapObject::Free();
}
