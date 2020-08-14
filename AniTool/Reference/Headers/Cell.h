#pragma once

#include "Line.h"

BEGIN(ENGINE)

class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE	{ COMPARE_MOVE, COMPARE_STOP };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _ulong*	Get_Index(void)				 { return &m_dwIndex; }
	_vec3*			Get_Point(POINT eType)  { return &m_vPoint[eType]; }
	CLine*			GetLine(int Index) { return m_pLine[Index]; }
	_vec3			Get_CellNormal() { return m_vNormal; }
	bool			Get_IsCollision() { return m_bIsCollision; }

public:
	void			Set_IsCollision(bool bIsCollision) { m_bIsCollision = bIsCollision; }
	void			Set_Point(_vec3 vPos, POINT eType) { m_vPoint[eType] = vPos; }
public:
	HRESULT			Ready_Cell(const _ulong& dwIndex,
								const _vec3* pPointA,
								const _vec3* pPointB,
								const _vec3* pPointC);

	bool			Compare(const _vec3* pEndPos);
	void			Render(void);

private:
	_vec3					m_vPoint[POINT_END];
	CLine*					m_pLine[LINE_END];

	LPD3DXLINE				m_pD3DXLine;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_ulong					m_dwIndex;

	_vec3					m_vNormal;
	bool					m_bIsCollision;

public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual void		Free(void);
};

END

