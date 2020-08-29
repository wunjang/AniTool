#pragma once

#include "GameObject.h"

BEGIN(ENGINE)


END

class CMoveSample : public ENGINE::CGameObject
{
public:
	enum {BODY, FACE, EYE, HANDL, HANDR, HEAD, PARTS_END};
private:
	explicit CMoveSample(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveSample();

public:
	// Getter
	ENGINE::CDynamicMesh* Get_Mesh(_uint iParts) { return m_pMeshCom[iParts]; }
	// Setter
	// General

public:
	virtual HRESULT		Initialize(void);
	virtual _int		Update(const _float& fTimeDelta);
	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float & fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel);

private:
	HRESULT				Add_Component();
	void				Input_Key(const _float& fTimeDelta);

private:
	ENGINE::CDynamicMesh*	m_pMeshCom[PARTS_END];
	_matrix					m_matTransform[PARTS_END];
	ENGINE::CTransform*		m_pTransformCom = nullptr;
	ENGINE::CRenderer*		m_pRendererCom = nullptr;

public:
	static CMoveSample* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

