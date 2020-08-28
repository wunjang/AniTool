#ifndef CameraPath_h__
#define CameraPath_h__

#include "GameObject.h"

class CCameraPath : public ENGINE::CGameObject
{
private:
	explicit CCameraPath(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraPath(void);

public:
	// Getter
	// Setter
	void Set_Action(vector<CAMERAACTION>* pvecAction) { m_pvecAction = pvecAction; }

public:
	HRESULT Initialize();

	virtual _int		Update(const _float& fTimeDelta) override;
	virtual void		Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel) override;

private:
	const vector<CAMERAACTION>*	m_pvecAction = nullptr;
	vector<_vec3>*		m_pvecLine = nullptr;
	ENGINE::CRenderer*	m_pRendererCom = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture2 = nullptr;

	LPD3DXMESH			m_pMesh;


public:
	static CCameraPath*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};


#endif // __ORBITAL_CAMERA_H__
