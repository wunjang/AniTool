#pragma once
#include "GameObject.h"

class CMapObject : public ENGINE::CGameObject
{
protected:
	explicit CMapObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapObject();

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
	ENGINE::CShader*			m_pShaderCom;
	
public:
	static CMapObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);

protected:
	virtual void	Free(void);
};

