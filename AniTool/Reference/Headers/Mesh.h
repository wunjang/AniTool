#pragma once
#include "Component.h"
BEGIN(ENGINE)
class ENGINE_DLL CMesh :public CComponent
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	virtual void			Free(void);
	
public:
	void					Set_SubsetState(_uint dwIdx,_bool isRender);
	_bool					Get_SubsetState(_uint dwIdx);
protected:
	_bool					m_bClone;
	vector<_bool>			m_vecSubSetRender;
};

END