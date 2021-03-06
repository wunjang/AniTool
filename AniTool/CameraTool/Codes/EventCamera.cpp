#include "stdafx.h"
#include "EventCamera.h"
#include "FreeCamera.h"

CEventCamera::CEventCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev)
	, m_vAngle(0.f, 0.f, 0.f)
{
}

CEventCamera::~CEventCamera(void)
{

}


void CEventCamera::Set_Action(vector<CAMERAACTION>& vecAction, pair<_float, _float> fSmoothInOut)
{
	m_fActionCounter = 0.f;

	m_listActionQueue.clear();
	for (size_t i = 0; i < vecAction.size(); ++i)
	{
		m_listActionQueue.push_back(vecAction[i]);
	}

	if (fSmoothInOut != pair<_float, _float>(0.f, 0.f))
	{
		CAMERAACTION CurCamera;
		CFreeCamera* pFreeCam = dynamic_cast<CFreeCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_FREE));
		CurCamera.vMoveTo = pFreeCam->Get_Pos();
		CurCamera.vRotateTo = pFreeCam->Get_Angle();

		CurCamera.fViewAngleTo = 45.f;
		_vec3 vTargetPos = *dynamic_cast<ENGINE::CTransform*>(ENGINE::Get_Component(ENGINE::LAYER_GAMEOBJECT, ENGINE::PLAYER, ENGINE::COMPONENT::TAG_TRANSFORM, ENGINE::COMPONENT::ID_DYNAMIC))->Get_Info(ENGINE::INFO_POS);
		CurCamera.fDistance = D3DXVec3Length(&(vTargetPos - pFreeCam->Get_Pos()));

		if(fSmoothInOut.first > 0.f)
		{
			m_listActionQueue.front().fLength = fSmoothInOut.first;

			CurCamera.fLength = 0.01f;
			m_listActionQueue.push_front(CurCamera);
		}

		if (fSmoothInOut.second > 0.f)
		{
			CurCamera.fLength = fSmoothInOut.second;
			m_listActionQueue.push_back(CurCamera);
		}
	}
	else
	{
		m_listActionQueue.front().fLength = 0.01f;
	}

	m_LastLastAction = m_LastAction = m_listActionQueue.front();

	m_pvecLine->clear();
}

void CEventCamera::Stop(void)
{
	m_listActionQueue.clear();
	m_fActionCounter = 0.f;
	m_LastLastAction = m_LastAction = CAMERAACTION();

	GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_FREE);
}

_bool CEventCamera::EscapeStop(void)
{
	if (m_listActionQueue.front().EffectOption == CAMERAEFFECT::EFFECT_STOP)
	{
		m_listActionQueue.front().EffectOption = CAMERAEFFECT::EFFECT_NONE;
		return true;
	}
	return false;
}

HRESULT CEventCamera::Initialize()
{
	m_vEye = { 0.f, 0.f, -1.f };
	m_vAt = { 0.f, 0.f, 0.f };
	m_vUp = { 0.f, 1.f, 0.f };

	m_fFovY = 45.f;
	m_fAspect = (_float)VIEWCX / VIEWCY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	FAILED_CHECK_RETURN(ENGINE::CCamera::Initialize(), E_FAIL);

	FAILED_CHECK(CGameObject::Add_Component(0, L"Renderer", ENGINE::COMPONENT::TAG_RENDERER, (ENGINE::CComponent**)&m_pRendererCom));

	GET_INSTANCE(CCameraMgr)->AddCamera(CAM_EVENT, this);
	m_pvecLine = GET_INSTANCE(CCameraMgr)->Get_Line();

	D3DXCreateSphere(m_pGraphicDev, 0.1f, 16, 16, &m_pMesh, nullptr);

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);
	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	m_pTexture->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.5f, 1.f, 0.5f, 1.f);
	m_pTexture->UnlockRect(0);

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture2, NULL);
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	m_pTexture2->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f);
	m_pTexture2->UnlockRect(0);

	return S_OK;
}

_int CEventCamera::Update(const _float& fTimeDelta)
{

	if (m_listActionQueue.empty())
	{
		m_LastLastAction = m_LastAction = CAMERAACTION();

		return 0;
	}
	else
	{
		m_fLineCounter += fTimeDelta;
		if(m_fLineCounter > 0.1f)
		{
			m_pvecLine->push_back(m_vEye);
			m_fLineCounter = 0.f;
		}
	}

	if (GET_INSTANCE(CCameraMgr)->Get_CurCameraType() == CAM_FREE)
		m_pRendererCom->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);

	if (!ReadActionData(fTimeDelta))
	{
		GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_FREE);
		return 0;
	}




	_int	iExit = ENGINE::CCamera::Update(fTimeDelta);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);
	return iExit;
}

void CEventCamera::Render(const _float & fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld._41 = m_vEye.x;
	matWorld._42 = m_vEye.y;
	matWorld._43 = m_vEye.z;

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	ENGINE::CShader* pShader = ENGINE::Get_Shader(ENGINE::CShaderMgr::ShaderType_Mesh);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();


	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &GET_INSTANCE(CCameraMgr)->Get_View());
	pEffect->SetMatrix("g_matProj", &GET_INSTANCE(CCameraMgr)->Get_Proj());

	_uint iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);

	if(m_listActionQueue.empty())
		m_pGraphicDev->SetTexture(0, m_pTexture2);
	else
		m_pGraphicDev->SetTexture(0, m_pTexture);

	m_pMesh->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

_bool CEventCamera::ReadActionData(const _float & fTimeDelta)
{
	if (m_listActionQueue.front().EffectOption == CAMERAEFFECT::EFFECT_STOP)
	{
		return true;
	}

	m_fActionCounter += fTimeDelta;
	if (m_fActionCounter > m_listActionQueue.front().fLength)
	{
		// 다음 액션으로 전환
		m_pTargetPos = nullptr;

		m_fActionCounter -= m_listActionQueue.front().fLength;
		m_LastLastAction = m_LastAction;
		m_LastAction = m_listActionQueue.front();
		m_listActionQueue.pop_front();

		if (m_listActionQueue.size() == 0)
			return false;
	}

	CAMERAACTION CurAction = m_listActionQueue.front();
	// 여기서부터 복사본을 사용


	if (CurAction.bIsFollow)
	{
		ActionTarget(fTimeDelta, CurAction);
	}
	else
	{
		ActionFree(fTimeDelta, CurAction);
	}
	Shake(fTimeDelta, CurAction);

	return true;
}

void CEventCamera::ActionTarget(const _float & fTimeDelta, const CAMERAACTION& CurAction)
{
	_float fTimeRate = m_fActionCounter / CurAction.fLength;

	m_pTargetPos = dynamic_cast<ENGINE::CTransform*>(ENGINE::Get_Component(ENGINE::LAYER_GAMEOBJECT, ENGINE::PLAYER, ENGINE::COMPONENT::TAG_TRANSFORM, ENGINE::COMPONENT::ID_DYNAMIC))->Get_Info(ENGINE::INFO_POS);
	m_vAngle = m_LastAction.vRotateTo + (CurAction.vRotateTo - m_LastAction.vRotateTo) * fTimeRate;

	m_vAt = *m_pTargetPos + m_LastAction.vMoveTo * (1.f - fTimeRate) + CurAction.vMoveTo * fTimeRate;

	_vec3 vLook = { 0.f, 0.f, 1.f };
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.z));
	D3DXVec3TransformCoord(&vLook, &vLook, &matRot);

	m_vEye = m_vAt - vLook * (m_LastAction.fDistance * (1.f - fTimeRate) + CurAction.fDistance * fTimeRate);


	m_fFovY = m_LastAction.fViewAngleTo + (CurAction.fViewAngleTo - m_LastAction.fViewAngleTo) * fTimeRate;
}

void CEventCamera::ActionFree(const _float & fTimeDelta, const CAMERAACTION& CurAction)
{
	_float fTimeRate = m_fActionCounter / CurAction.fLength;


	CAMERAACTION NextAction;
	auto iter_next = m_listActionQueue.begin();
	iter_next++;

	if (iter_next != m_listActionQueue.end())
	{
		// 다음 액션이 있을 경우
		NextAction = *iter_next;
	}
	else
	{
		// 현재 마지막 액션인 경우
		NextAction = CurAction;
	}

	D3DXVec3CatmullRom(&m_vEye, &m_LastLastAction.vMoveTo, &m_LastAction.vMoveTo, &CurAction.vMoveTo, &NextAction.vMoveTo, fTimeRate);
	//m_vEye = m_LastAction.vMoveTo + (CurAction.vMoveTo - m_LastAction.vMoveTo) * fTimeRate;

	m_vAngle = m_LastAction.vRotateTo + (CurAction.vRotateTo - m_LastAction.vRotateTo) * fTimeRate;
	m_fFovY = m_LastAction.fViewAngleTo + (CurAction.fViewAngleTo - m_LastAction.fViewAngleTo) * fTimeRate;

	_vec3 vLook = { 0.f, 0.f, 1.f };
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.z));
	D3DXVec3TransformCoord(&vLook, &vLook, &matRot);

	m_vAt = m_vEye + vLook;
}

void CEventCamera::Shake(const _float & fTimeDelta, CAMERAACTION & CurAction)
{
	const _int& EffectOption = CurAction.EffectOption;

	function<_vec3(SHAKESTAT&)> fnShake = [&](SHAKESTAT& stat)->_vec3 {
		if (D3DXVec3Length(&(stat.vDest - stat.vPos)) < fTimeDelta * stat.fSpeed)
		{
			stat.vPos = stat.vDest;

			_float fShakeAngle = D3DXToRadian(ENGINE::CFunc::GetRandomFloat(0.f, 360.f));
			stat.vDest = _vec3(cos(fShakeAngle), sin(fShakeAngle), 0.f) * ENGINE::CFunc::GetRandomFloat(0.f, 0.1f);
		}
		else
		{
			_vec3 vShakeDir = *D3DXVec3Normalize(&vShakeDir, &(stat.vDest - stat.vPos));
			stat.vPos += vShakeDir * fTimeDelta * stat.fSpeed;
		}

		_vec3 vShake;
		_matrix matCamWorld;
		Get_matCameraWorld(&matCamWorld);
		D3DXVec3TransformNormal(&vShake, &stat.vPos, &matCamWorld);

		return vShake;
	};

	switch (CurAction.EffectOption)
	{
	case CAMERAEFFECT::EFFECT_SHAKEEYE:
		m_vEye += fnShake(m_Shake_Eye);
		break;
	case CAMERAEFFECT::EFFECT_SHAKEAT:
		m_vAt += fnShake(m_Shake_At);
		break;
	case CAMERAEFFECT::EFFECT_SHAKEEYEANDATDFFRENT:
		m_vEye += fnShake(m_Shake_Eye);
		m_vAt += fnShake(m_Shake_At);
		break;
	case CAMERAEFFECT::EFFECT_SHAKEEYEANDATSAME: {
		_vec3 vMove = fnShake(m_Shake_Eye);
		m_vEye += vMove;
		m_vAt += vMove;
	}	break;
	default:
		return;
	}
}


CEventCamera* CEventCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEventCamera*	pInstance = new CEventCamera(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CEventCamera::Free(void)
{
	m_listActionQueue.clear();
	ENGINE::Safe_Release(m_pMesh);
	ENGINE::Safe_Release(m_pTexture);
	ENGINE::Safe_Release(m_pTexture2);
	ENGINE::CCamera::Free();
}