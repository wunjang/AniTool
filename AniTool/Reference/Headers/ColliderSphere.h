#ifndef ColliderShpere_h__
#define ColliderShpere_h__

#include "Collider.h"


BEGIN(ENGINE)

class ENGINE_DLL CColliderSphere : public CCollider
{
private:
	explicit CColliderSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CColliderSphere(const CColliderSphere& rhs);
	virtual ~CColliderSphere(void);

public:
	_float*				Get_Radius(void) { return &m_fRadius; }
	void				Set_Radius(_float fRadius);
	_vec3*				Get_Pos(void) { return &m_vPos; }
	void				Set_Pos(_vec3 vPos) { m_vPos = vPos; }
	void				Change_SphereRadius(_float fRadius);
	
public:
	HRESULT				 Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, _float fRadius, _vec3 vPos);
	virtual void	Render();
private:
	_float		m_fRadius;
	_vec3		m_vPos;

public:
	static CColliderSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _matrix * m_pTargetBoneMatrix,
		const _matrix * m_pTargetWorldMatrix,
		_float fRadius,
		_vec3 vPos);




	virtual	CComponent*				Clone(void* pArg);


	virtual void					Free(void);
};

END
#endif // Collider_h__
