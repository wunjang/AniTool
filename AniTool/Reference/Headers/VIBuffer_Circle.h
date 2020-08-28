#pragma once
#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer_Circle :
	public CVIBuffer
{
private:
	explicit CVIBuffer_Circle(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Circle(const CVIBuffer_Circle& rhs);
	virtual ~CVIBuffer_Circle();

public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	static CVIBuffer_Circle*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void);
};

END