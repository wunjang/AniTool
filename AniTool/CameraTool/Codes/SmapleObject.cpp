#include "stdafx.h"
#include "SmapleObject.h"

CSampleObject::CSampleObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:ENGINE::CGameObject(pGraphicDev)
{
}


CSampleObject::~CSampleObject()
{
}

void CSampleObject::Set_Animation(ANITAG::PLAYER_BODY eAni)
{
	auto iter = m_mapAniSet.find(eAni);
	if (iter == m_mapAniSet.end())
	{
		MSG_BOX(L"Animation Set Not Exist !!! \n -Create Animation Set With Animation Tool");
		return;
	}

	for (int i = 0; i < PARTS_END; ++i)
		m_pMeshCom[i]->Set_AnimationSet(iter->second[i]);
}

HRESULT CSampleObject::Load_AnimationComboData(const wstring & wstrFileName)
{
	m_mapAniSet.clear();

	HANDLE hFile = CreateFile((L"../Data/" + wstrFileName + L".dat").c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX(L"Failed to Create Load File .dat !!!");
		return E_FAIL;
	}

	// Buffer
	TCHAR szAnimationName[64] = L"";
	vector<_uint> vecAniSet;
	vecAniSet.resize(PARTS_END);

	DWORD dwByte = 0;
	while (true)
	{
		ReadFile(hFile, &szAnimationName, sizeof(szAnimationName), &dwByte, nullptr); //여기서는 버리는 데이터
		for (int i = 0; i < PARTS_END; ++i)
			ReadFile(hFile, &vecAniSet[i], sizeof(_uint), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		m_mapAniSet.emplace(vecAniSet[PARTS_BODY], vecAniSet);
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CSampleObject::Initialize(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Load_AnimationComboData(L"PlayerAnimationSet"), E_FAIL);

	m_pTransformCom->Set_Scale(0.003f, 0.003f, 0.003f);



	for (int i = 0; i < PARTS_END; ++i)
		m_pMeshCom[i]->Set_AnimationSet(ANITAG::Body_AreaWaitDance01);

	return S_OK;
}

_int CSampleObject::Update(const _float & fTimeDelta)
{
	//if(GET_INSTANCE(CCameraMgr)->Get_EventCameraActionLeft() > 0)
	m_pRendererCom->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);

	for (int i = 0; i < PARTS_END; ++i)
		m_pMeshCom[i]->Play_AnimationSet(fTimeDelta);
	
	_matrix matTwo, matFaceRot, matHandRot;

	D3DXMatrixScaling(&matTwo, 2.f, 2.f, 2.f);

	D3DXMatrixRotationYawPitchRoll(&matFaceRot, D3DXToRadian(90.f), 0.f, D3DXToRadian(270.f));
	D3DXMatrixRotationYawPitchRoll(&matHandRot, D3DXToRadian(90.f), 0.f, D3DXToRadian(90.f));

	_int iExit = CGameObject::Update(fTimeDelta);

	m_matTransform[PARTS_BODY]	= *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[PARTS_HEAD]	= matTwo * matFaceRot * m_pMeshCom[PARTS_BODY]->Get_FrameByName("Head")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[PARTS_FACE]	= matTwo * matFaceRot * m_pMeshCom[PARTS_BODY]->Get_FrameByName("Head")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[PARTS_EYE]	= matTwo * matFaceRot * m_pMeshCom[PARTS_BODY]->Get_FrameByName("Head")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[PARTS_HANDL]	= matTwo * matHandRot * m_pMeshCom[PARTS_BODY]->Get_FrameByName("HandL")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_matTransform[PARTS_HANDR]	= matTwo * matHandRot * m_pMeshCom[PARTS_BODY]->Get_FrameByName("HandR")->CombinedTransformationMatrix * *m_pTransformCom->Get_WorldMatrix();




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
	
	matView = GET_INSTANCE(CCameraMgr)->Get_View();
	matProj = GET_INSTANCE(CCameraMgr)->Get_Proj();

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

	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Body",	ENGINE::COMPONENT::TAG_MESH, (ENGINE::CComponent**)&m_pMeshCom[PARTS_BODY]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Face",	ENGINE::COMPONENT::TAG(100), (ENGINE::CComponent**)&m_pMeshCom[PARTS_FACE]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Eye",		ENGINE::COMPONENT::TAG(101), (ENGINE::CComponent**)&m_pMeshCom[PARTS_EYE]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_HandL",	ENGINE::COMPONENT::TAG(102), (ENGINE::CComponent**)&m_pMeshCom[PARTS_HANDL]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_HandR",	ENGINE::COMPONENT::TAG(103), (ENGINE::CComponent**)&m_pMeshCom[PARTS_HANDR]), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Add_Component(0, L"Mario_Head",	ENGINE::COMPONENT::TAG(104), (ENGINE::CComponent**)&m_pMeshCom[PARTS_HEAD]), E_FAIL);
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
