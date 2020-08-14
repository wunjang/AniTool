#include "stdafx.h"
#include "SmapleObject.h"

#include "OrbitalCamera.h"
#include "AniSync.h"

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

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);



	for (int i = 0; i < PARTS_END; ++i)
		m_pMeshCom[i]->Set_AnimationSet(0);

	return S_OK;
}

_int CSampleObject::Update(const _float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);

	LPD3DXANIMATIONSET pAniSet = nullptr;
	m_pMeshCom[0]->Get_AniCtrl()->Get_AniCtrl()->GetAnimationSet(CAniSync::Get_Dialog()->m_lboxMainPart.GetCurSel(), &pAniSet);
	if (pAniSet->GetPeriod() > m_pMeshCom[0]->Get_AniCtrl()->Get_AniCtrl()->GetTime() || CAniSync::Get_Dialog()->Get_IsAnimationLoop())
	{
		for (int i = 0; i < PARTS_END; ++i)
			m_pMeshCom[i]->Play_AnimationSet(fTimeDelta);
	}
	
	
	
	CAniSync::Get_Dialog()->Set_UpdateAniTime((_int)(m_pMeshCom[0]->Get_AniCtrl()->Get_AniCtrl()->GetTime() * 30.0));
	
	_matrix matTwo, matFaceRot, matHandRot;
	D3DXMatrixScaling(&matTwo, 2.f, 2.f, 2.f);

	D3DXMatrixRotationYawPitchRoll(&matFaceRot, D3DXToRadian(90.f), 0.f, D3DXToRadian(270.f));
	D3DXMatrixRotationYawPitchRoll(&matHandRot, D3DXToRadian(90.f), 0.f, D3DXToRadian(90.f));

	_int iExit = CGameObject::Update(fTimeDelta);

	m_matTransform[BODY]	= *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[HEAD]	= matTwo * matFaceRot * m_pMeshCom[BODY]->Get_FrameByName("Head")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[FACE]	= matTwo * matFaceRot * m_pMeshCom[BODY]->Get_FrameByName("Head")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[EYE]		= matTwo * matFaceRot * m_pMeshCom[BODY]->Get_FrameByName("Head")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[HANDL]	= matTwo * matHandRot * m_pMeshCom[BODY]->Get_FrameByName("HandL")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[HANDR]	= matTwo * matHandRot * m_pMeshCom[BODY]->Get_FrameByName("HandR")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();




	return iExit;
}

_int CSampleObject::LateUpdate(const _float & fTimeDelta)
{
	return 0;
}

void CSampleObject::Render(const _float & fTimeDelta)
{
	ENGINE::CShader* pShader = ENGINE::Get_Shader(ENGINE::CShaderMgr::ShaderType_Mesh);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();

	_matrix matView, matProj;
	COrbitalCamera* pCam = dynamic_cast<COrbitalCamera*>(ENGINE::Get_GameObject(ENGINE::LAYER_ENVIRONMENT, ENGINE::CAMERA));
	matView = pCam->Get_matView();
	matProj = pCam->Get_matProjection();

	_uint iPassMax = 0;
	//pEffect->SetTechnique("Hardware_Skin");
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	for (int i = 0; i < PARTS_END; ++i)
	{
		pEffect->SetMatrix("g_matWorld", &m_matTransform[i]);
		m_pMeshCom[i]->Render_Meshes(pEffect);
	}

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CSampleObject::Add_Component()
{
	ENGINE::CComponent*		pComponent = nullptr;

	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Body",	ENGINE::COMPONENT::TAG_MESH, (ENGINE::CComponent**)&m_pMeshCom[BODY]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Face",	ENGINE::COMPONENT::TAG(100), (ENGINE::CComponent**)&m_pMeshCom[FACE]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Eye",		ENGINE::COMPONENT::TAG(101), (ENGINE::CComponent**)&m_pMeshCom[EYE]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_HandL",	ENGINE::COMPONENT::TAG(102), (ENGINE::CComponent**)&m_pMeshCom[HANDL]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_HandR",	ENGINE::COMPONENT::TAG(103), (ENGINE::CComponent**)&m_pMeshCom[HANDR]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Head",	ENGINE::COMPONENT::TAG(104), (ENGINE::CComponent**)&m_pMeshCom[HEAD]), E_FAIL);
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
