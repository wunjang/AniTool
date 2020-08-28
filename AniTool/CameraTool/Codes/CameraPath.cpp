#include "stdafx.h"
#include "CameraPath.h"
#include "EventCamTool.h"

CCameraPath::CCameraPath(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev)
{
}

CCameraPath::~CCameraPath(void)
{

}

HRESULT CCameraPath::Initialize()
{
	FAILED_CHECK(CGameObject::Add_Component(0, L"Renderer", ENGINE::COMPONENT::TAG_RENDERER, (ENGINE::CComponent**)&m_pRendererCom));

	D3DXCreateSphere(m_pGraphicDev, 0.1f, 16, 16, &m_pMesh, nullptr);

	m_pvecLine = GET_INSTANCE(CCameraMgr)->Get_Line();


	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);
	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	m_pTexture->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	m_pTexture->UnlockRect(0);

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture2, NULL);
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	m_pTexture2->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	m_pTexture2->UnlockRect(0);

	return S_OK;
}

_int CCameraPath::Update(const _float& fTimeDelta)
{
	if (nullptr == m_pvecAction)
	{
		m_pvecAction = CEventCamTool::Get_Dialog()->Get_Actions();
	}

	if (GET_INSTANCE(CCameraMgr)->Get_EventCameraActionLeft() == 0)
		m_pRendererCom->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);






	_int	iExit = CGameObject::Update(fTimeDelta);

	return iExit;
}

void CCameraPath::Render(const _float & fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	for (int i = 0; i < int(m_pvecAction->size()); ++i)
	{
		D3DXMatrixIdentity(&matWorld);
		//matWorld *= 0.5f;
		memcpy(&matWorld._41, &(*m_pvecAction)[i].vMoveTo, sizeof(_vec3));

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


		ENGINE::CShader* pShader = ENGINE::Get_Shader(ENGINE::CShaderMgr::ShaderType_Mesh);

		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();


		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &GET_INSTANCE(CCameraMgr)->Get_View());
		pEffect->SetMatrix("g_matProj", &GET_INSTANCE(CCameraMgr)->Get_Proj());

		_uint iPassMax = 0;
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(0);

		if(CEventCamTool::Get_Dialog()->Get_CurActionIdx() == i)
			m_pGraphicDev->SetTexture(0, m_pTexture);
		else
			m_pGraphicDev->SetTexture(0, m_pTexture2);

		m_pMesh->DrawSubset(0);

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	D3DXMatrixIdentity(&matWorld);

	if(!m_pvecLine->empty())
	{
		ID3DXLine *Line;
		D3DXCreateLine(m_pGraphicDev, &Line);
		Line->SetWidth(3.f);
		//Line->SetAntialias(true);
		Line->Begin();
		Line->DrawTransform(&m_pvecLine->front(), m_pvecLine->size(), &(matWorld*GET_INSTANCE(CCameraMgr)->Get_View()*GET_INSTANCE(CCameraMgr)->Get_Proj()), D3DXCOLOR(0.f, 1.f, 0.f, 1.0f));
		Line->End();
		Line->Release();
	}

}

CCameraPath* CCameraPath::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraPath*	pInstance = new CCameraPath(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CCameraPath::Free(void)
{
	ENGINE::Safe_Release(m_pMesh);
	ENGINE::Safe_Release(m_pTexture);
	ENGINE::Safe_Release(m_pTexture2);
	ENGINE::CGameObject::Free();
}
