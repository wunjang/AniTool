#pragma once

#include "Camera.h"
class CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	CCameraMgr();
	~CCameraMgr();

public:
	void AddCamera(CAMERATYPE eCamType, ENGINE::CCamera* pCamera) { m_mapCamera.emplace(eCamType, pCamera); }
	void Set_CurCamera(CAMERATYPE eCamType) { m_pCurCamera = m_mapCamera.find(eCamType)->second; m_eCurCamera = eCamType; }
	void Set_FieldHeight(_float fHeight) { m_fHeightFieldHeight = fHeight; }

	// Getter
	ENGINE::CCamera*	Get_Camera(CAMERATYPE eCamType) { return m_mapCamera.find(eCamType)->second; }
	const CAMERATYPE&	Get_CurCameraType(void) { return m_eCurCamera; }
	size_t				Get_EventCameraActionLeft(void);
	_matrix				Get_View(void) { return m_pCurCamera->Get_matView(); }
	_matrix				Get_Proj(void) { return m_pCurCamera->Get_matProjection(); }
	vector<_vec3>*		Get_Line(void) { return &m_vecLine; }
	_float				Get_FieldHeight(void) { return m_fHeightFieldHeight; }

	_bool				EscapeStop(void);

	_bool Picking(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOut);

private:
	map<CAMERATYPE, ENGINE::CCamera*>	m_mapCamera;
	ENGINE::CCamera*					m_pCurCamera;
	CAMERATYPE							m_eCurCamera;

	vector<_vec3>						m_vecLine;

	_float								m_fHeightFieldHeight;
};

