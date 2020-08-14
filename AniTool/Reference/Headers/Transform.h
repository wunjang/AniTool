#ifndef Transform_h__
#define Transform_h__

#include "Component.h"
#include "Cell.h"

BEGIN(ENGINE)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);

public:
	const _vec3*				Get_Scale();
	void						Get_Scale(_vec3* pVector);

	const _vec3*				Get_Info(INFO eType);
	void						Get_Info(INFO eType, _vec3* pVector);

	const _vec3*				Get_Angle();
	void						Get_Angle(_vec3* pVector);

	void						Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void						Set_Scale(const _vec3* fAngle);

	void						Set_Angle(ROTATION eType, const _float& fAngle);
	void						Set_Angle(_vec3& vAngle);

	void						Set_OnType(on_Type eOnType) { m_eOnType = eOnType; }
	on_Type						Get_OnType() { return m_eOnType; }


	void						Set_Info(INFO eType, _vec3* pVector);
	void						Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void						Set_Pos(const _vec3* pPos);


	void						Move_Pos(const _vec3* pDir, const _float& fTimeDelta, const _float& fSpeed);
	void						Rotation(ROTATION eType, const _float& fAngle);
	const _matrix*				Compute_LookAtTarget(const _vec3* pTargetPos);
	void						Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);

	void						Set_FallTrigger(_bool bIsFalling);
	_bool						Get_FallTrigger() { return m_bFallTrigger; }
	void						Set_FallState(_bool bIsFalling);
	_bool						Get_FallState() { return m_bIsFalling; }

	void						Set_FallingAttr(_float fAccFallSpd, _float fFallSpdLimit, _float fInitFallSpeed);
	void						Reset_FallingAttr();

	void						Get_WorldMatrix(_matrix* pWorld)const;
	const  _matrix*				Get_WorldMatrix(void) const;
	const _matrix*				Get_NRotWorldMatrix(_matrix* pWorld) const;
	void						Set_ParentMatrix(const _matrix* pParent);
	void						Set_Matrix(const _matrix* pMatrix);

	void						Set_NavNormal(_vec3 vNormal) { m_vNavNormal = vNormal; }
	_vec3						Get_NavNormal() { return m_vNavNormal; }

	void						Set_StateByWall(ENGINE::StateByWall eState) { m_eStateByWall = eState; }
	ENGINE::StateByWall			Get_StateByWall() { return m_eStateByWall; }

public:
	HRESULT			Ready_Transform(void);
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);
public:
	_vec3						m_vInfo[INFO_END];
	_vec3						m_vScale;
	_vec3						m_vAngle;
	_matrix						m_matWorld;


	_bool						m_bIsFalling;
	_bool						m_bFallTrigger;
	ENGINE::StateByWall			m_eStateByWall;

	_float						m_fFallSpeed;
	_float						m_fAccFallSpd;
	_float						m_fFallSpdLimit;
	_vec3						m_vNavNormal;
	on_Type						m_eOnType;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone(void* pArg) override;

private:
	virtual void			Free(void);



};

END
#endif // Transform_h__
