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
	void						Set_Angle(const _vec3& vAngle);

	void						Set_RevolutionAngle(ROTATION eType, const _float& fAngle);
	void						Set_RevolutionAngle(_vec3& vAngle);


	void						Set_OnType(on_Type eOnType) { m_eOnType = eOnType; }
	on_Type						Get_OnType() { return m_eOnType; }
	_float						Get_FallSpd() { return m_fFallSpeed; }

	void						Set_Info(INFO eType, _vec3* pVector);
	void						Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void						Set_Pos(const _vec3* pPos);
	void						Set_Pos(AXIS eAxis, const _float& fVal);

	void						Add_Scale(const _vec3& vScale);

	void						Move_Pos(const _vec3* pDir, const _float& fTimeDelta, const _float& fSpeed);
	void						Rotation(ROTATION eType, const _float& fAngle);
	void						Rotation( const _vec3& vAngle);

	void						Revolution(ROTATION eType, const _float& fAngle);
	void						Revolution(const _vec3& vAngle);

	on_Type						Check_OnType();


	void						Set_FallingAttr(_float fAccFallSpd, _float fFallSpdLimit, _float fInitFallSpeed);

	void						Reset_FallingAttr();

	void						Get_WorldMatrix(_matrix* pWorld)const;
	const  _matrix*				Get_WorldMatrix(void) const;

	void						Set_ParentMatrix(const _matrix* pParent);
	void						Set_Matrix(const _matrix* pMatrix);

	void						Set_NavNormal(_vec3 vNormal) { m_vNavNormal = vNormal; }
	_vec3						Get_NavNormal() { return m_vNavNormal; }

	void						Set_StateByWall(ENGINE::StateByWall eState) { m_eStateByWall = eState; }
	ENGINE::StateByWall			Get_StateByWall() { return m_eStateByWall; }

	void						Set_ParentWorld(const _matrix* pParentWorld,_bool bCheckParentPosOnly = false)
	{
		m_pParentWorld = pParentWorld; 
		m_bCheckParentPosOnly = bCheckParentPosOnly;
	}

	_vec3*						Get_Pos_Pointer() { return &m_vInfo[INFO_POS]; }
	_matrix*					Get_World_Pointer() { return &m_matWorld; };
	_matrix						Get_Old_World() { return m_matOldWorld; };

	void						Set_BillBoard_Matrix(_matrix matView, _float fCorrectAngle = 0.f, _bool bUseBill_Y = false);
public:
	HRESULT						Ready_Transform(void);
	virtual _int				Update_Component(const _float& fTimeDelta);
	virtual _int				LateUpdate_Component(const _float& fTimeDelta);
public:
	_vec3						m_vInfo[INFO_END];
	_vec3						m_vScale;
	_vec3						m_vAngle;
	_vec3						m_vRevolutionAngle;
	_matrix						m_matWorld;

	////////////////////////////////
	_matrix						m_matBillBoard;
	////////////////////////////////
	//MotionBlur
	//bool 값으로 모션블러 시작 시 포스 고정 시키고 끝날때 초기화시키는것도 좋은 방법
	_matrix						m_matOldWorld;
	_float						m_fOldWorldUpdateTimer;
	_float						m_fOldWorldUpdateFre;

	////////////////////////////////
	_bool						m_bCheckParentPosOnly;
	const _matrix*				m_pParentWorld;

	ENGINE::StateByWall			m_eStateByWall;
	on_Type						m_eOnType;
	on_Type						m_eKeepOnType;

	_float						m_fFallSpeed;
	_float						m_fAccFallSpd;
	_float						m_fFallSpdLimit;
	_vec3						m_vNavNormal;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone(void* pArg) override;

private:
	virtual void			Free(void);



};

END
#endif // Transform_h__
