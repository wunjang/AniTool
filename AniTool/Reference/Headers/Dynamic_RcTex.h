#ifndef Dynamic_RcTex_h__
#define Dynamic_RcTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CDynamicRcTex : public CVIBuffer
{
private:
	explicit CDynamicRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicRcTex(const CDynamicRcTex& rhs);
	virtual ~CDynamicRcTex(void);

public:
	virtual HRESULT		Ready_Buffer(void);
	void				Render_Buffer(void);

public:
	static CDynamicRcTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone(void);
	virtual void	Free(void);
};


END
#endif // RcTex_h__
