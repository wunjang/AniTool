#pragma once

#ifndef __COLLISION_MOUSE_H__
#define __COLLISION_MOUSE_H__

#include "Component.h"

BEGIN(ENGINE)

class CGameObject;

class ENGINE_DLL CCollisionMouse : public CComponent
{
protected:
	explicit CCollisionMouse();

public:
	virtual ~CCollisionMouse();

public:
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);


protected:
	void TranslateLocalSpace(
		HWND hWnd,
		DWORD dwWinCX,
		DWORD dwWinCY,
		const D3DXMATRIX* pProjMatrix,
		const D3DXMATRIX* pViewMatrix,
		const D3DXMATRIX* pWorldMatrix);

protected:
	D3DXVECTOR3 m_vRayPivot;
	D3DXVECTOR3 m_vRayDir;

	CGameObject* m_pPickTarget;
};

END

#endif
