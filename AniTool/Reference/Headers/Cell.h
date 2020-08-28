#pragma once

#include "Line.h"
#include "StaticMesh.h"

BEGIN(ENGINE)
class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };

	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _ulong*	Get_Index(void)				 { return &m_dwIndex; }
	_vec3*			Get_Point(POINT eType)  { return &m_vPoint[eType]; }
	CLine*			Get_Line(int Index) { return m_pLine[Index]; }
	_vec3			Get_CellNormal() { return m_vNormal; }
	bool			Get_IsCollision() { return m_bIsCollision; }
	_vec3			Get_CenterPos() { return m_vCenterPos; }
		
	float			Get_NormalAngle() { return m_fUp_Normal_Angle; }
	float			Get_CellLength() { return m_fCellCycleLength; }
	_IntVec3		Get_intPoint(POINT eType) { return m_ivPoint[eType]; }


	CCell*			Get_Neighbor(NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	CCell*			Get_Neighbor(_uint eType) const { return m_pNeighbor[eType]; }
	void			Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }

	int				Get_NeighborIndex(NEIGHBOR eType) { return m_iNeighborIndex[eType]; }
	void			Set_NeighborIndex(NEIGHBOR eType, int iIndex) { m_iNeighborIndex[eType] = iIndex; }

public:
	void			Set_IsCollision(bool bIsCollision) { m_bIsCollision = bIsCollision; }
	void			Set_Point(_vec3 vPos, POINT eType) { m_vPoint[eType] = vPos; }
	void			Set_intPoint(_IntVec3 ivPos, POINT eType) { m_ivPoint[eType] = ivPos; }

	void			Move_Cell(_vec3 vDir);
	void			Set_Move(vector<_vec3> vecPos, float vTime, float fSpeed = 1.f);
	void			Move_Angle(ENGINE::CStaticMesh* pMesh, _matrix matrix);

	void			Set_MoveType(int iType) { m_iMoveType = iType; }
	int				Get_MoveType() { return m_iMoveType; }

public:
	HRESULT			Ready_Cell(const _ulong& dwIndex,
								const _vec3* pPointA,
								const _vec3* pPointB,
								const _vec3* pPointC,
								ENGINE::CGameObject* pObj);

	bool			Compare(const _vec3* pEndPos);
	bool			ComparePart(const _vec2* pPartPos);

	void			Render(LPD3DXEFFECT pEffect, _matrix matView, _matrix matProj);

	_bool			Compare_Point(_IntVec3* pPointFirst, _IntVec3* pPointSecond, CCell* pCell);

	ENGINE::CGameObject* Get_CellObj() { return m_pObj; }

	D3DXPLANE		Get_Plane() { return m_tPlane; }
	void			Set_Plane(D3DXPLANE tPlane) { m_tPlane = tPlane; }


private:
	_vec3					m_vPoint[POINT_END];
	CLine*					m_pLine[LINE_END];

	LPD3DXLINE				m_pD3DXLine;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_ulong					m_dwIndex;

	_vec3					m_vNormal;
	bool					m_bIsCollision;

	D3DXVECTOR3				m_vCenterPos;
	float					m_fCellCycleLength;

	float					m_fUp_Normal_Angle;
	int						m_iCellType;

	CCell*					m_pNeighbor[NEIGHBOR_END];
	int						m_iNeighborIndex[NEIGHBOR_END];

	_IntVec3				m_ivPoint[POINT_END];

	int						m_iMoveType;

	ENGINE::CGameObject*	m_pObj;

	D3DXPLANE				m_tPlane;
public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, ENGINE::CGameObject* pObj = nullptr);
	virtual void		Free(void);
};

END

