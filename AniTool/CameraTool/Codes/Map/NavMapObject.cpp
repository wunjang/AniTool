#include "stdafx.h"
#include "NavMapObject.h"

CNavMapObject::CNavMapObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:ENGINE::CGameObject(pGraphicDev)
{
}


CNavMapObject::~CNavMapObject()
{
	ENGINE::CGameObject::Free();
	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		m_vecCell[i]->Set_IsCollision(false);
	}
	m_vecCell.clear();
}

HRESULT CNavMapObject::Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	FAILED_CHECK_RETURN(Add_Component(wstrResourseName), E_FAIL);
	m_pTransformCom->m_vInfo[ENGINE::INFO_POS] = tInfo.vPos;
	m_pTransformCom->m_vScale = tInfo.vScale;
	m_pTransformCom->m_vAngle = tInfo.vAngle;



	return S_OK;
}

_int CNavMapObject::Update(const _float & fTimeDelta)
{
	CGameObject::Update(fTimeDelta);

	if (m_bIsDead == true)
	{
		return ENGINE::DEAD_OBJ;
	}

	return ENGINE::NO_EVENT;
}

_int CNavMapObject::LateUpdate(const _float & fTimeDelta)
{

	return ENGINE::NO_EVENT;
}

HRESULT CNavMapObject::Add_Component(const wstring wstrResourseName)
{
	ENGINE::CComponent*		pComponent = nullptr;

	// For.Com_Mesh
	FAILED_CHECK_RETURN(ENGINE::CGameObject::Add_Component(0, wstrResourseName, ENGINE::COMPONENT::TAG_MESH, (ENGINE::CComponent**)&m_pMesh), E_FAIL);

	// For.Com_Transform
	FAILED_CHECK_RETURN(ENGINE::CGameObject::Add_Component(0, L"Transform", ENGINE::COMPONENT::TAG_TRANSFORM, (ENGINE::CComponent**)&m_pTransformCom), E_FAIL);

	return S_OK;
}

void CNavMapObject::MakeNavMesh()
{
	_ulong VtxNum = m_pMesh->Get_NumVtx();
	const _vec3* pPos = m_pMesh->Get_VtxPos();
	vector<_vec3> vecPos;
	int iIndex = 0;
	for (_uint i = 0; i < VtxNum; ++i)
	{
		D3DXVECTOR3 vPos = (pPos[i]);
		D3DXVec3TransformCoord(&vPos, &vPos, &m_pTransformCom->m_matWorld);

		vecPos.push_back(vPos);

		if (vecPos.size() == 3)
		{
			// 시계방향 그리는 코드
			D3DXVECTOR3 vCenter;
			D3DXVECTOR3 vInspectionPos[2];
			D3DXVECTOR3 vResult;
			D3DXVECTOR3 vTemp;

			vCenter = (vecPos[0] + vecPos[1] + vecPos[2]) / 3;

			vInspectionPos[0] = vecPos[0] - vCenter;
			vInspectionPos[1] = vecPos[1] - vCenter;

			D3DXVec3Cross(&vResult, &vInspectionPos[0], &vInspectionPos[1]);

			if (vResult.y < 0)
			{
				vTemp = vecPos[0];
				vecPos[0] = vecPos[1];
				vecPos[1] = vTemp;
			}

			ENGINE::CCell* pCell = ENGINE::CCell::Create(m_pGraphicDev, iIndex, &vecPos[0], &vecPos[1], &vecPos[2]);

			vecPos.clear();

			m_vecCell.push_back(pCell);
		}
	}
}

CNavMapObject * CNavMapObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	CNavMapObject* pInstance = new CNavMapObject(pGraphicDev);

	if (FAILED(pInstance->Initialize(tInfo, wstrResourseName)))
	{
		ENGINE::Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavMapObject::Free(void)
{

}
