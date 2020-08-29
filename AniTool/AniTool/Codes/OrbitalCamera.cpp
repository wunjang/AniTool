#include "stdafx.h"
#include "OrbitalCamera.h"

COrbitalCamera::COrbitalCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev),
	m_vCentre(0.f, 0.f, 0.f),
	m_vAngle(0.f, 180.f, 0.f),
	m_fDist(64.f)
{
	ZeroMemory(m_matWorld, sizeof(m_matWorld));
}

COrbitalCamera::~COrbitalCamera(void)
{

}


HRESULT COrbitalCamera::Initialize(const _float& fFovY)
{
	m_vEye = { 0.f, 0.f, -1.f };
	m_vAt = { 0.f, 0.f, 0.f };
	m_vUp = { 0.f, 1.f, 0.f };

	m_fFovY = D3DXToRadian(fFovY);
	m_fAspect = (_float)VIEWCX / VIEWCY;
	m_fNear = 1.f;
	m_fFar = 1000.f;

	FAILED_CHECK_RETURN(ENGINE::CCamera::Initialize(), E_FAIL);

	return S_OK;
}

_int COrbitalCamera::Update(const _float& fTimeDelta)
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
	_vec3 vLook = { 0.f, 0.f, m_fDist };
	
	_matrix matRot[ENGINE::ROT_END], matTrans;

	D3DXMatrixRotationX(&matRot[ENGINE::ROT_X], D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationY(&matRot[ENGINE::ROT_Y], D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationZ(&matRot[ENGINE::ROT_Z], D3DXToRadian(m_vAngle.z));
	m_matWorld = matRot[ENGINE::ROT_X] * matRot[ENGINE::ROT_Y] * matRot[ENGINE::ROT_Z];

	D3DXVec3TransformCoord(&vLook, &vLook, &m_matWorld);

	m_vAt = m_vCentre;
	m_vEye = m_vAt - vLook;


	_int	iExit = ENGINE::CCamera::Update(fTimeDelta);

	return iExit;
}

void COrbitalCamera::Render(const _float & fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel)
{
}


COrbitalCamera* COrbitalCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fFovY)
{
	COrbitalCamera*	pInstance = new COrbitalCamera(pGraphicDev);

	if (FAILED(pInstance->Initialize(fFovY)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void COrbitalCamera::Free(void)
{
	ENGINE::CCamera::Free();
}

void COrbitalCamera::Key_Input(const _float& fTimeDelta)
{
	if (ENGINE::KeyDown(DIK_SPACE))
	{
		m_vCentre = { 0.f, 0.f, 0.f };
		m_fDist = 64.f;
	}
}

void COrbitalCamera::Mouse_Move(const _float& fTimeDelta)
{
	_long	dwMouseMove = 0;
	// 마우스 우클릭 회전
	if(ENGINE::MousePressing(ENGINE::DIM_RB))
	{
		if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_Y))
			m_vAngle.x += float(dwMouseMove) * 0.1f;


		if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_X))
			m_vAngle.y += float(dwMouseMove) * 0.1f;
	}

	// 마우스 휠 이동
	if(ENGINE::MousePressing(ENGINE::DIM_MB))
	{
		if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_Y))
		{
			_vec3 vMove = {};
			memcpy(&vMove, m_matWorld.m[ENGINE::INFO_UP], sizeof(_vec3));
			m_vCentre += dwMouseMove * 0.00055f * vMove * m_fDist;
		}

		if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_X))
		{
			_vec3 vMove = {};
			memcpy(&vMove, m_matWorld.m[ENGINE::INFO_RIGHT], sizeof(_vec3));
			m_vCentre -= dwMouseMove * 0.00055f * vMove * m_fDist;
		}
	}

	// 마우스 휠 거리조절
	dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::DIMS_Z);
	if (dwMouseMove > 0)
		m_fDist *= 0.8f;
	else if (dwMouseMove < 0)
		m_fDist *= 1.25;
	
}
