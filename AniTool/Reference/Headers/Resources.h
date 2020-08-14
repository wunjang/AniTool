#ifndef Resources_h__
#define Resources_h__

#include "Base.h"
#include "Engine_Defines.h"
#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CResources : public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResources(const CResources& rhs);
	virtual ~CResources(void);

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	virtual CResources*		Clone(void) { return nullptr; }
	virtual void Free(void);
};

END
#endif // Resources_h__
