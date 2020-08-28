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

	void				Change_SphereRadius(_float fRadius);
	void				Set_Radius(_float fRadius);
public:
	HRESULT				Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix, _float fRadius, _vec3 vPos,_vec3* vScale);
	virtual void		Render(LPD3DXEFFECT & pEffect, _matrix matView, _matrix matProj);



public:
	static CColliderSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _matrix * m_pTargetBoneMatrix,
		const _matrix * m_pTargetWorldMatrix,
		_float fRadius,
		_vec3 vPos,
		_vec3* vScale = nullptr);




	virtual	CComponent*				Clone(void* pArg);


	virtual void					Free(void);
};

END
#endif // Collider_h__
