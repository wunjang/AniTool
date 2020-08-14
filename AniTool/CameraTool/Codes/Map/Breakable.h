#pragma once
#include "MapObject.h"
#include "NavMapObject.h"

class CBreakable : public CMapObject
{
protected:
	explicit CBreakable(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBreakable();

	virtual HRESULT		Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);
public:

	virtual _int		Update(const _float& fTimeDelta);
	virtual _int		LateUpdate(const _float& fTimeDelta);
	virtual void		Render(const _float& fTimeDelta);
	virtual void		Render_PostProcess(const _float& fTimeDelta)override;
	virtual HRESULT		Add_Component(const wstring wstrResourseName);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT & pEffect);

private:
	CNavMapObject*	m_pNavObj;

public:
	static CBreakable* Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);

protected:
	virtual void	Free(void);
};

