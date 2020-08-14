#include "stdafx.h"
#include "FreeCamera.h"

#include "EventCamTool.h"

CFreeCamera::CFreeCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev)
	, m_vAngle(0.f, 0.f, 0.f)
{
}

CFreeCamera::~CFreeCamera(void)
{

}


HRESULT CFreeCamera::Initialize()
{
	m_vEye = { 0.f, 0.f, 0.f };
	m_vAt = { 0.f, 0.f, 1.f };
	m_vUp = { 0.f, 1.f, 0.f };

	m_fFovY = 60.f;
	m_fAspect = (_float)VIEWCX / VIEWCY;
	m_fNear = 1.f;
	m_fFar = 1000.f;

	m_fSpeed = 1.f;

	FAILED_CHECK_RETURN(ENGINE::CCamera::Initialize(), E_FAIL);

	GET_INSTANCE(CCameraMgr)->AddCamera(CAM_FREE, this);
	GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_FREE);

	return S_OK;
}

_int CFreeCamera::Update(const _float& fTimeDelta)
{
	if (g_MFChWnd == ::GetFocus())
	{
		Mouse_Move(fTimeDelta);
		Key_Input(fTimeDelta);
	}
	

	if (m_vAngle.x > 89.9f)
		m_vAngle.x = 89.9f;
	else if (m_vAngle.x < -89.9)
		m_vAngle.x = -89.9f;

	_vec3 vLook = { 0.f, 0.f, 1.f };
	
	_matrix matRot;

	D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.z));

	D3DXVec3TransformCoord(&vLook, &vLook, &matRot);

	m_vAt = m_vEye + vLook;


	_int	iExit = ENGINE::CCamera::Update(fTimeDelta);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFovY), m_fAspect, m_fNear, m_fFar);

	CEventCamTool::Get_Dialog()->Set_FreeCamData(m_vEye, m_vAngle, m_fFovY);

	return iExit;
}


CFreeCamera* CFreeCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFreeCamera*	pInstance = new CFreeCamera(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CFreeCamera::Free(void)
{
	ENGINE::CCamera::Free();
}

void CFreeCamera::Key_Input(const _float& fTimeDelta)
{
	if (ENGINE::KeyDown(DIK_F1))
	{
		ZeroMemory(m_vEye, sizeof(_vec3));
		ZeroMemory(m_vAngle, sizeof(_vec3));
		m_fFovY = 60.f;
	}
	if (ENGINE::KeyDown(DIK_UP))
		m_fFovY += 5.f;
	if (ENGINE::KeyDown(DIK_DOWN))
		m_fFovY -= 5.f;


	// input dir

	_vec3 vInputDir = { 0.f, 0.f, 0.f };

	if (ENGINE::KeyPressing(DIK_W))
		vInputDir.z += 1.f;
	if (ENGINE::KeyPressing(DIK_A))
		vInputDir.x -= 1.f;
	if (ENGINE::KeyPressing(DIK_S))
		vInputDir.z -= 1.f;
	if (ENGINE::KeyPressing(DIK_D))
		vInputDir.x += 1.f;
	if (ENGINE::KeyPressing(DIK_SPACE))
		vInputDir.y += 1.f;
	if (ENGINE::KeyPressing(DIK_LSHIFT))
		vInputDir.y -= 1.f;
	

	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_vAngle.y));
	D3DXVec3TransformCoord(&vInputDir, &vInputDir, &matRotY);

	m_vEye += vInputDir * m_fSpeed;

	

}

void CFreeCamera::Mouse_Move(const _float& fTimeDelta)
{
	_long	dwMouseMove = 0;

	if (ENGINE::MousePressing(ENGINE::DIM_MB))
	{
		if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_Y))
			m_vAngle.x += float(dwMouseMove) * 0.1f;

		if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_X))
			m_vAngle.y += float(dwMouseMove) * 0.1f;
	}
}
