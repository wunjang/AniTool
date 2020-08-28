#ifndef Event_Camera_h__
#define Event_Camera_h__

#include "Camera.h"

struct SHAKESTAT
{
	SHAKESTAT() : vDest(0.f, 0.f, 0.f), vPos(0.f, 0.f, 0.f), fSpeed(1.f)
	{
	}
	_vec3	vDest;
	_vec3	vPos;
	_float	fSpeed;
};

class CEventCamera : public ENGINE::CCamera
{
private:
	explicit CEventCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEventCamera(void);

public:
	// Getter
	size_t		Get_ActionLeft(void) { return m_listActionQueue.size(); }
	// Setter
	void		Set_Action(vector<CAMERAACTION>& vecAction, pair<_float, _float> fSmoothInOut = { 0.f, 0.f });

	// General
	void		Stop(void);
	_bool		EscapeStop(void);

public:
	HRESULT				Initialize();
	virtual _int		Update(const _float& fTimeDelta) override;
	virtual void		Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel) override;

private:
	_bool ReadActionData(const _float& fTimeDelta);

	void	ActionTarget(const _float& fTimeDelta, const CAMERAACTION& CurAction);
	void	ActionFree(const _float& fTimeDelta, const CAMERAACTION& CurAction);
	void	Shake(const _float& fTimeDelta, CAMERAACTION& CurAction);

private:
	list<CAMERAACTION>	m_listActionQueue;
	CAMERAACTION		m_LastAction;
	CAMERAACTION		m_LastLastAction;
	_float				m_fActionCounter;

	_float				m_fLineCounter = 0.f;
	vector<_vec3>*		m_pvecLine = nullptr;


	ENGINE::CRenderer*	m_pRendererCom = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture2 = nullptr;

	LPD3DXMESH			m_pMesh;

	_vec3				m_vAngle;

	const _vec3*		m_pTargetPos = nullptr;

	SHAKESTAT			m_Shake_Eye;
	SHAKESTAT			m_Shake_At;

public:
	static CEventCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};


#endif // __ORBITAL_CAMERA_H__
