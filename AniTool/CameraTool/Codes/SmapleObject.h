#pragma once

#include "GameObject.h"

BEGIN(ENGINE)


END

class CSampleObject : public ENGINE::CGameObject
{
public:
	enum PARTS { PARTS_BODY, PARTS_EYE, PARTS_FACE, PARTS_HANDL, PARTS_HANDR, PARTS_HEAD, PARTS_END };
private:
	explicit CSampleObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSampleObject();

public:
	// Getter
	ENGINE::CDynamicMesh* Get_Mesh(_uint iParts) { return m_pMeshCom[iParts]; }
	// Setter
	void				Set_Animation(ANITAG::PLAYER_BODY eAni);
	void				Set_AnimationSpeed(const _float& fAniSpeed) { m_fAnimationSpeed = fAniSpeed; }
	// General
	HRESULT				Load_AnimationComboData(const wstring& wstrFileName);

public:
	virtual HRESULT		Initialize(void);
	virtual _int		Update(const _float& fTimeDelta);
	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel);

private:
	HRESULT				Add_Component();

private:
	ENGINE::CDynamicMesh*	m_pMeshCom[PARTS_END];
	_matrix					m_matTransform[PARTS_END];
	ENGINE::CTransform*		m_pTransformCom = nullptr;
	ENGINE::CRenderer*		m_pRendererCom = nullptr;

	map<_uint, vector<_uint>>	m_mapAniSet;

	_float					m_fAnimationSpeed = 1.8f;
public:
	static CSampleObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

