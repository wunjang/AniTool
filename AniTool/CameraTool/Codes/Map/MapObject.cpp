#include "stdafx.h"
#include "MapObject.h"

CMapObject::CMapObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:ENGINE::CGameObject(pGraphicDev)
{
}


CMapObject::~CMapObject()
{
}

HRESULT CMapObject::Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
{
	FAILED_CHECK_RETURN(CMapObject::Add_Component(wstrResourseName), E_FAIL);

	m_pTransformCom->m_vInfo[ENGINE::INFO_POS] = tInfo.vPos;
	m_pTransformCom->m_vScale = tInfo.vScale;
	m_pTransformCom->m_vAngle = tInfo.vAngle;

	return S_OK;
}

_int CMapObject::Update(const _float & fTimeDelta)
{
	CGameObject::Update(fTimeDelta);

	return ENGINE::NO_EVENT;
}

_int CMapObject::LateUpdate(const _float & fTimeDelta)
{
	CGameObject::LateUpdate(fTimeDelta);
	ENGINE::Get_Renderer()->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);

	return ENGINE::NO_EVENT;
}

void CMapObject::Render(const _float & fTimeDelta)
{
	ENGINE::CShader* pShader = ENGINE::Get_Shader(ENGINE::CShaderMgr::ShaderType_Mesh);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_matView", &GET_INSTANCE(CCameraMgr)->Get_View());
	pEffect->SetMatrix("g_matProj", &GET_INSTANCE(CCameraMgr)->Get_Proj());

	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);

	m_pMesh->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();
}

void CMapObject::Render_PostProcess(const _float & fTimeDelta)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	LPD3DXEFFECT pEffect = ENGINE::Get_Shader(ENGINE::CShaderMgr::ShaderType::ShaderType_Mesh)->Get_EffectHandle();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );
	_uint iPassMax = 0;

	pEffect->SetVector("g_BloomPower", &_vec4(1.f, 1.f, 1.f, 1.f));

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(1);


	m_pMesh->Render_PostProcess(pEffect);


	pEffect->EndPass();
	pEffect->End();
}

HRESULT CMapObject::Add_Component(const wstring wstrResourseName)
{
	ENGINE::CComponent*		pComponent = nullptr;

	// For.Com_Mesh
	FAILED_CHECK_RETURN(ENGINE::CGameObject::Add_Component(0, wstrResourseName, ENGINE::COMPONENT::TAG_MESH, (ENGINE::CComponent**)&m_pMesh), E_FAIL);

	// For.Com_Transform
	FAILED_CHECK_RETURN(ENGINE::CGameObject::Add_Component(0, L"Transform", ENGINE::COMPONENT::TAG_TRANSFORM, (ENGINE::CComponent**)&m_pTransformCom, ENGINE::COMPONENT::ID_DYNAMIC), E_FAIL);

	return S_OK;
}

HRESULT CMapObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

//CMapObject * CMapObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName)
//{
//	CMapObject* pInstance = new CMapObject(pGraphicDev);
//
//	if (FAILED(pInstance->Initialize(tInfo, wstrResourseName)))
//	{
//		ENGINE::Safe_Release(pInstance);
//	}
//	return pInstance;
//}

void CMapObject::Free(void)
{
	ENGINE::CGameObject::Free();
}
