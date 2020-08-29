#ifndef __ORBITAL_CAMERA_H__
#define __ORBITAL_CAMERA_H__

#include "Camera.h"

class COrbitalCamera : public ENGINE::CCamera
{
private:
	explicit COrbitalCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrbitalCamera(void);

public:
	const _float*	Get_Dist() { return &m_fDist; }

public:
	HRESULT Initialize(const _float& fFovY);

	virtual _int Update(const _float& fTimeDelta) override;
	virtual void Render(const _float & fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);

private:
	_vec3		m_vCentre;
	_vec3		m_vAngle;
	_float		m_fDist;

	_matrix		m_matWorld;

public:
	static COrbitalCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fFovY);

private:
	virtual void Free(void);
};


#endif // __ORBITAL_CAMERA_H__
