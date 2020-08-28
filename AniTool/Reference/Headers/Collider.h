#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(ENGINE)
class CShader;
class CTexture;
class ENGINE_DLL CCollider : public CComponent
{

protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);

	virtual ~CCollider(void);

public:
	const _vec3*				Get_LocalPosx(void) { return &m_vPos; }
	const _matrix*				Get_WorldMatrix(void) { return &m_matWorld; }

	const LPD3DXMESH			Get_Mesh(void)const { return m_pMesh; }

	_vec3						Get_LocalPos() { return m_vPos; }
	void						Set_LocalPos(_vec3 vPos) { m_vPos = vPos; }

	_float*						Get_Radius(void) { return &m_fRadius; }
	void						Set_Radius(_float fRadius);

	_vec3						Get_Scale() { return m_vScale; }
	void						Set_Scale(_vec3 vScale) { m_vScale = vScale; }

	const void					Set_Target(ENGINE::D3DXFRAME_DERIVED* pTargetFrame, _matrix * m_pParentWorldMatrix);
	const void					Set_TargetWorld(_matrix * m_pTargetWorldMatrix);
	const _matrix*				Get_TargetWorldMatrix(void) { return m_pTargetWorldMatrix; }

	const void					Set_BoneWolrd(_matrix * pTargetBoneMatrix);
	void						Set_Collision() { m_bIsCollision = true; }

	ENGINE::COLLIDER::TYPE		Get_ColType() { return m_eColType; }
	void						Set_ColRender(_bool bState) { m_bColliderRender = bState; }
	_bool						Get_ColRender() { return m_bColliderRender; }

	void						UpdateWolrd();//입장시 콜라이더의 행렬을 셋해주기위한것
public:
	HRESULT Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix);
	virtual void	Render(LPD3DXEFFECT & pEffect, _matrix matView, _matrix matProj);

protected:
	const	_matrix*			m_pTargetWorldMatrix;//부모의 행렬
	const	_matrix*			m_pTargetBoneMatrix;//뼈의 행렬

	_matrix						m_matWorld;//충돌체가 부모의 행렬, 부모의 뼈의 행렬, 고유의 행렬들의 계산을 마친 실제 월드상의 행렬
	_vec3						m_vPos;
	_vec3						m_vScale;
	_float						m_fRadius;


	_bool						m_bIsCollision = false;
	ENGINE::COLLIDER::TYPE		m_eColType;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;


	CTexture*					m_pTexture[ENGINE::COLLIDER::COL_END];//충돌했을때와 안했을때 색을 입혀주기위한 변수

	//LPDIRECT3DTEXTURE9			m_pTexture[ENGINE::COLLIDER::COL_END];//충돌했을때와 안했을때 색을 입혀주기위한 변수

	////경계를 그리기위한 메쉬
	LPD3DXMESH  m_pMesh;
	static _bool				m_bColliderRender;

	virtual	CComponent*	Clone(void* pArg);
	virtual void		Free(void);
};



END
#endif // Collider_h__
