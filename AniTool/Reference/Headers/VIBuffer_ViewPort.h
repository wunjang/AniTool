#pragma once

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer_ViewPort : public CVIBuffer
{
private:
	explicit CVIBuffer_ViewPort(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_ViewPort(const CVIBuffer_ViewPort& rhs);
	virtual ~CVIBuffer_ViewPort(void) = default;
public:
	virtual HRESULT Ready_Component_Prototype(_float fX, _float fY, _float fSizeX, _float fSizeY);
	virtual HRESULT Ready_Component(void* pArg);
public:
	static CVIBuffer_ViewPort*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fX, _float fY, _float fSizeX, _float fSizeY);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void);
};

END

