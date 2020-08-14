#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer_Cube : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual ~CVIBuffer_Cube(void) = default;

public:
	virtual HRESULT	Ready_Component_Prototype(void);
	virtual HRESULT	Ready_Component(void* pArg);

public:
	static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void);
};

END
#endif // CubeTex_h__
