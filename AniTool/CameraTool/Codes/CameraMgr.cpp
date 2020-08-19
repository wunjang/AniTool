#include "stdafx.h"
#include "CameraMgr.h"

#include "EventCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr)
CCameraMgr::CCameraMgr()
{
}


CCameraMgr::~CCameraMgr()
{
	//for (auto rCam : m_mapCamera)
	//	ENGINE::Safe_Release(rCam);
	m_mapCamera.clear();
}

size_t CCameraMgr::Get_EventCameraActionLeft(void)
{
	return dynamic_cast<CEventCamera*>(m_mapCamera[CAMERATYPE::CAM_EVENT])->Get_ActionLeft();
}

_bool CCameraMgr::EscapeStop(void)
{
	return dynamic_cast<CEventCamera*>(m_mapCamera[CAMERATYPE::CAM_EVENT])->EscapeStop();
}

_bool CCameraMgr::Picking(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pOut)
{
	if (::GetFocus() != g_MFChWnd)
		return false;

	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_MFChWnd, &ptMouse);

	// ������ ������ ���콺 ��ǥ�� �������� ��ȯ

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
	vMousePos.z = 0.f;

	// �������� �佺���̽�
	_matrix		matProj;
	matProj = Get_Proj();
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3	vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// �� �����̽����� ���� �����̽��� ��ȯ

	_matrix		matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// ���忡�� ���÷� ��ȯ

	_matrix		matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_float u, v, dist;

	_vec3 vertex[3];
	vertex[0] = { -5000.f, m_fHeightFieldHeight, -5000.f };
	vertex[1] = { -5000.f, m_fHeightFieldHeight, 15000.f };
	vertex[2] = { 15000.f, m_fHeightFieldHeight, -5000.f };

	if (D3DXIntersectTri(&vertex[0], &vertex[1], &vertex[2], &vRayPos, &vRayDir, &u, &v, &dist))
	{
		if (pOut != nullptr)
		{
			*pOut = vertex[0] + (vertex[1] - vertex[0]) * u + (vertex[2] - vertex[0]) * v;
		}

		return true;
	}

	return false;
}
