#pragma once

#include "Mesh.h"
#include "Cell.h"
#include "Transform.h"

BEGIN(ENGINE)

typedef struct tagNavTreePart
{
	float fLeftX;
	float fRightX;
	float fFrontZ;
	float fBackZ;
}NavPart;

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void					Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; } // ¾È¾¸
	_ulong					Get_Index() { return m_dwIndex; } // ¾È¾¸

public:
	HRESULT					Initialize();
	void					Render();

	_vec3					Move_OnNaviMesh(ENGINE::OBJ_ID eID, int iStateMsg, ENGINE::CTransform* pTransform, const _vec3* pTargetDir, _vec2* pOutNavIndex);
	float					Get_NavMeshY(ENGINE::CTransform* pTransform, _vec2* pOutNavIndex);

private:
	HRESULT					Load_NaviData(const wstring wstrFilePath);
	HRESULT					Division_NavMesh();

private:
	_vec3					IsOnNavMeshCheck(vector<CCell*> vecOverlapCell, _vec3* pEndPos, ENGINE::CTransform* pTransform, int iStateMsg = 0);
	_vec3					IsOnNavMeshCheckY(vector<CCell*> vecOverlapCell, _vec3* pEndPos, ENGINE::CTransform* pTransform);

	_vec3					IntersectTri(CCell* pCell, D3DXVECTOR3* pEndPos, D3DXVECTOR3 vRayPos, D3DXVECTOR3 vRayDir, int iStateMsg, ENGINE::CTransform* pTransform);
	bool					IntersectTriBool(CCell* pCell, D3DXVECTOR3 vRayPos, D3DXVECTOR3 vRayDir, float* pDist = nullptr);
	_vec3					IntersectTriY(CCell* pCell, D3DXVECTOR3* pEndPos, D3DXVECTOR3 vRayPos, D3DXVECTOR3 vRayDir);


private:
	static const int m_iCleaveNum = 32;

	vector<CCell*>			m_vecCell;
	vector<CCell*>			m_vecPartCell[m_iCleaveNum][m_iCleaveNum];
	NavPart					m_tPart[m_iCleaveNum][m_iCleaveNum];
	_ulong					m_dwIndex; // ¾È¾¸

	POINT					m_iIndex;
public:
	static  CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent *	Clone(void * pArg) override;
	virtual void			Free(void);
};
END

