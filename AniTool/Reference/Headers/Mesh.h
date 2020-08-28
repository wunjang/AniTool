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
	void					Set_SubsetState(_uint dwIdx, SUBSET::RENDER eRender, _bool bAllSet = false);
	SUBSET::RENDER			Get_SubsetState(_uint dwIdx);

	void					Set_SubsetStateContainer(vector<SUBSET::RENDER>* pSubSetStateContainer) { m_vecSubSetRender = *pSubSetStateContainer; }
	vector<SUBSET::RENDER>*	Get_SubsetStateContainer() { return &m_vecSubSetRender; }
protected:
	_bool					m_bClone;
	vector<SUBSET::RENDER>	m_vecSubSetRender;


};

END