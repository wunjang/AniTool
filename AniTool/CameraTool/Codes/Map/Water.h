#pragma once
#include "GameObject.h"
class CWater :
	public ENGINE::CGameObject
{
protected:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWater();

	virtual HRESULT		Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);
public:
	virtual _int		Update(const _float& fTimeDelta);
	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta);
	virtual void		Render_PostProcess(const _float& fTimeDelta)override;
	virtual HRESULT		Add_Component(const wstring wstrResourseName);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT & pEffect);


protected:
	ENGINE::CStaticMesh*		m_pMesh;
	ENGINE::CTransform*			m_pTransformCom;
	ENGINE::CRenderer*			m_pRendererCom;


	_float m_fTime;

	ENGINE::CTexture*			m_pDistortionTex;
	ENGINE::CTexture*			m_pNormalTex;

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);
protected:
	virtual void	Free(void);
};

