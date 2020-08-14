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
	_float*				Get_Radius(void) { return &m_fRadius; }

public:
	HRESULT Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, _vec3	vMin, _vec3 vMax);
	virtual void	Render();

private:
	_vec3		m_vMin, m_vMax;
	_float		m_fRadius;
public:
	static CColliderBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix * m_pTargetBoneMatrix, const _matrix * m_pTargetWorldMatrix, _vec3 vMin, _vec3 vMax);


	virtual	CComponent*		Clone(void);

	virtual void		Free(void);
};

END
#endif // Collider_h__
