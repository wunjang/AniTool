#pragma once
#include "GameObject.h"
BEGIN(ENGINE)
//class CStaticMesh;
END
class CNavMapObject :
	public ENGINE::CGameObject
{
protected:
	explicit CNavMapObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNavMapObject();

	virtual HRESULT		Initialize(ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);
public:

	virtual _int			Update(const _float& fTimeDelta);
	virtual _int			LateUpdate(const _float& fTimeDelta);
	virtual HRESULT			Add_Component(const wstring wstrResourseName);
	vector<ENGINE::CCell*>&	Get_vecCell() { return m_vecCell; }
	ENGINE::CStaticMesh*	Get_Mesh() { return m_pMesh; }

protected:
	void					MakeNavMesh();

protected:
	ENGINE::CStaticMesh*		m_pMesh;
	ENGINE::CTransform*			m_pTransformCom;
	vector<ENGINE::CCell*>		m_vecCell;

public:
	static CNavMapObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::_WorldInfo tInfo, const wstring wstrResourseName);

protected:
	virtual void	Free(void);
};

