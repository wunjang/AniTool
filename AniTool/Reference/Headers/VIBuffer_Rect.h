#pragma once

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer_Rect : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_Rect(void) = default;
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	static CVIBuffer_Rect*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void);
};

END

