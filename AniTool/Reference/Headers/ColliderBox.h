#ifndef ColliderBox_h__
#define ColliderBox_h__

#include "Collider.h"


BEGIN(ENGINE)

class ENGINE_DLL CColliderBox : public CCollider
{
private:
	explicit CColliderBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CColliderBox(void);

public:
	const _vec3*		Get_Min(void) { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }

	const _matrix		Get_RenderWorld(void) { return m_matRenderWorld; }
public:
	HRESULT Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, _vec3	vMin, _vec3 vMax);

	HRESULT Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, const _vec3* pMeshPos, const _ulong& dwNumVtx, const _ulong& dwStride);

	virtual void	Render(LPD3DXEFFECT & pEffect, _matrix matView, _matrix matProj);

private:
	_vec3		m_vMin, m_vMax;
	_matrix		m_matRenderWorld;
public:
	static CColliderBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, _vec3 vMin, _vec3 vMax);

	static CColliderBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, const _vec3* pMeshPos, const _ulong& dwNumVtx, const _ulong& dwStride);


	virtual	CComponent*		Clone(void);

	virtual void		Free(void);
};

END
#endif // Collider_h__
