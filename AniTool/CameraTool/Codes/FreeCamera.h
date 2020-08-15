#ifndef Free_Camera_h__
#define Free_Camera_h__

#include "Camera.h"

class CFreeCamera : public ENGINE::CCamera
{
private:
	explicit CFreeCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFreeCamera(void);

public:
	// Getter
	const _vec3& Get_Pos(void) { return m_vEye; }
	const _vec3& Get_Angle(void) { return m_vAngle; }

	// Setter
	void		Set_Pos(const _vec3& vPos) { m_vEye = vPos; }
	void		Set_Angle(const _vec3& vAngle) { m_vAngle = vAngle; }

public:
	HRESULT Initialize();

	virtual _int Update(const _float& fTimeDelta) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);

private:
	_vec3		m_vAngle;
	_float		m_fSpeed;

public:
	static CFreeCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};


#endif // __ORBITAL_CAMERA_H__
