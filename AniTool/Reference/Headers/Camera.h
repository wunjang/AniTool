#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "GameObject.h"

BEGIN(ENGINE)

class ENGINE_DLL CCamera : public CGameObject
{

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	//Getter
	const _matrix& Get_matOrthoView() { return m_matOrthoView; }
	const _matrix& Get_matView() { return m_matView; }
	const _matrix& Get_matProjection() { return m_matProj; }
	const _matrix& Get_matCameraWorld(_matrix* CameraWorld)
	{
		D3DXMatrixInverse(&m_matCamWolrd, nullptr, &m_matView);
		if (nullptr != CameraWorld)
		{
			*CameraWorld = m_matCamWolrd;
		}
		return m_matCamWolrd;
	}
	

public:
	virtual HRESULT	Initialize(void) override;
	virtual _int	Update(const _float& fTimeDelta) override;
	virtual _int	LateUpdate(const _float& fTimeDelta) override;

protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFovY, m_fAspect, m_fNear, m_fFar;
	_matrix			m_matView, m_matProj, m_matOrthoView, m_matCamWolrd;


public:
	virtual void	Free(void);
};

END

#endif

