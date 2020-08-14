#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(ENGINE)

class CTerrainTex;
class CTransform;

class ENGINE_DLL CCalculator : public CComponent
{
private:
	typedef	struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];	// 객체 당 세 개의 면을 향해 뻗어나가는 벡터
		_vec3		vAxis[3];		// 박스와 평행한 임의의 축

	}OBB;

private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator(void);


public:
	HRESULT		Ready_Calculator(void);
	_float		Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX, const _ulong& dwCntZ);


	_bool		Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
		const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
		const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_Sphere(const _vec3 * pDistance, const _float * pRadiusSum);

private:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);
	
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*				Clone(void);
private:
	virtual void		Free(void);

};

END
#endif // Calculator_h__
