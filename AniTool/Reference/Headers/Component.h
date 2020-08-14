#ifndef Component_h__
#define Component_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(ENGINE)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	explicit CComponent(void);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);

	
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

protected:
	_bool					m_bClone;

	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;


public:
	virtual	CComponent*		Clone(void* pArg)PURE;

protected:
	virtual void Free();
};

END
#endif // Component_h__
