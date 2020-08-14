#ifndef Controller_h__
#define Controller_h__

#include "Component.h"


BEGIN(ENGINE)

class ENGINE_DLL CController : public CComponent
{
protected:
	explicit CController(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CController(void);

public:

public:
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

protected:
	virtual void			Free(void);
	virtual CComponent*	Clone(void * pArg) override;

};

END
#endif // Controller_h__
