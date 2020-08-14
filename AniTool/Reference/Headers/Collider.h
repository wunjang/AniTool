#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(ENGINE)
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

	void						Set_LocalPos(_vec3 vPos) { m_vPos = vPos; }
	const void					Set_Target(ENGINE::D3DXFRAME_DERIVED* pTargetFrame, _matrix * m_pParentWorldMatrix);
	const void					Set_TargetWorld(_matrix * m_pTargetWorldMatrix);
	const _matrix*				Get_TargetWorldMatrix(void) { return m_pTargetWorldMatrix; }

	const void					Set_BoneWolrd(_matrix * pTargetBoneMatrix);
	void						Set_Collision() { m_bIsCollision = true; }

	ENGINE::COLLIDER::TYPE		Get_ColType() { return m_eColType; }
	void						Set_ColRender(_bool bState) { m_bColliderRender = bState; }
	_bool						Get_ColRender() { return m_bColliderRender; }
public:
	HRESULT Ready_Collider(const _matrix * pTargetBoneMatrix, const _matrix * pTargetWorldMatrix);
	virtual void	Render();

protected:
	const	_matrix*			m_pTargetWorldMatrix;//�θ��� ���
	const	_matrix*			m_pTargetBoneMatrix;//���� ���

	_matrix						m_matWorld;//�浹ü�� �θ��� ���, �θ��� ���� ���, ������ ��ĵ��� ����� ��ģ ���� ������� ���
	_vec3						m_vPos;

	_bool						m_bIsCollision = false;
	ENGINE::COLLIDER::TYPE		m_eColType;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[ENGINE::COLLIDER::COL_END];//�浹�������� �������� ���� �����ֱ����� ����

	////��踦 �׸������� �޽�
	LPD3DXMESH  m_pMesh;
	static _bool				m_bColliderRender;

	virtual	CComponent*	Clone(void* pArg);
	virtual void		Free(void);
};



END
#endif // Collider_h__
