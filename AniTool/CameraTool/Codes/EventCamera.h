#ifndef Event_Camera_h__
#define Event_Camera_h__

#include "Camera.h"

class CEventCamera : public ENGINE::CCamera
{
private:
	explicit CEventCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEventCamera(void);

public:
	// Getter
	size_t		Get_ActionLeft(void) { return m_listActionQueue.size(); }
	// Setter
	void		Set_Action(vector<CAMERAACTION>& vecAction);

	// General
	void		Stop(void);

public:
	HRESULT Initialize();

	virtual _int Update(const _float& fTimeDelta) override;
	virtual void		Render(const _float& fTimeDelta) override;

private:
	_bool ReadActionData(const _float& fTimeDelta);

	void	ActionTarget(const _float& fTimeDelta);
	void	ActionFree(const _float& fTimeDelta);

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

public:
	static CEventCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};


#endif // __ORBITAL_CAMERA_H__
