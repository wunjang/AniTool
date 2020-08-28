#pragma once
#include "GameObject.h"

class CHeightField : public ENGINE::CGameObject
{
public:
	explicit CHeightField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHeightField();

public:////기본함수
	virtual HRESULT		Initialize(void);
	virtual _int		Update(const _float& fTimeDelta) override;
	virtual _int		LateUpdate(const _float& fTimeDelta) override;
	virtual void		Render(const _float& fTimeDelta, ENGINE::SUBSET::RENDER eRenderSel) override;
	virtual HRESULT		Add_Component();
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT & pEffect);

private:
	void				Key_Input(void);

private:
	ENGINE::CVIBuffer_Terrain*		m_pBuffer;
	ENGINE::CRenderer*				m_pRendererCom;

	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;

	_float m_fHeight = 0.f;

public:
	static CHeightField* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void) override;
};

