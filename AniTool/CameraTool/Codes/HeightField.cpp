#include "stdafx.h"
#include "HeightField.h"

CHeightField::CHeightField(LPDIRECT3DDEVICE9 pGraphicDev)
	:ENGINE::CGameObject(pGraphicDev)
{
}

CHeightField::~CHeightField()
{
}

HRESULT CHeightField::Initialize()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	GET_INSTANCE(CCameraMgr)->Set_FieldHeight(m_fHeight);


	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);
	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	m_pTexture->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pTexture->UnlockRect(0);

	return S_OK;
}

_int CHeightField::Update(const _float& fTimeDelta)
{
	_long lMouseMove = ENGINE::Get_DIMouseMove(ENGINE::MOUSEMOVESTATE::DIMS_Z);
	_float fMoveY = 1.f;
	if (ENGINE::KeyPressing(DIK_LCONTROL))
		fMoveY = 10.f;


	if (lMouseMove > 0)
	{
		m_fHeight += fMoveY;
	}
	else if (lMouseMove < 0)
	{
		m_fHeight -= fMoveY;
	}

	if (ENGINE::KeyDown(DIK_F1))
	{
		m_fHeight = 0.f;
	}

	GET_INSTANCE(CCameraMgr)->Set_FieldHeight(m_fHeight);


	return ENGINE::NO_EVENT;
}

_int CHeightField::LateUpdate(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return ENGINE::NO_EVENT;
}

void CHeightField::Render(const _float& fTimeDelta)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	ENGINE::CShader* pShader = ENGINE::Get_Shader(ENGINE::CShaderMgr::ShaderType_Mesh);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();

	_matrix		matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld._41 = -5000.f;
	matWorld._42 = m_fHeight;
	matWorld._43 = -5000.f;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &GET_INSTANCE(CCameraMgr)->Get_View());
	pEffect->SetMatrix("g_matProj", &GET_INSTANCE(CCameraMgr)->Get_Proj());

	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);

	m_pGraphicDev->SetTexture(0, m_pTexture);
	m_pBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CHeightField::Add_Component()
{
	ENGINE::CComponent*		pComponent = nullptr;
	
	FAILED_CHECK(CGameObject::Add_Component(0, L"TestTerrain", ENGINE::COMPONENT::TAG_VIBUFFER, (ENGINE::CComponent**)&m_pBuffer));
	FAILED_CHECK(CGameObject::Add_Component(0, L"Renderer", ENGINE::COMPONENT::TAG_RENDERER, (ENGINE::CComponent**)&m_pRendererCom));

	return S_OK;
}

HRESULT CHeightField::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	
	return S_OK;
}

CHeightField * CHeightField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHeightField* pInstance = new CHeightField(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CHeightField::Free(void)
{
	ENGINE::Safe_Release(m_pTexture);
	CGameObject::Free();
}
