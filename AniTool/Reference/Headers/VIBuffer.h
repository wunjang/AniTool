#pragma once

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void) = default;

public:
	_ulong		Get_TriCnt(void) { return m_dwTriCnt; }
public:
	virtual HRESULT Ready_Component_Prototype();
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Render_Buffer(void);

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
protected:
	_ulong		m_dwVtxSize = 0;
	_ulong 		m_dwVtxCnt = 0;
	_ulong 		m_dwTriCnt = 0;
	_ulong 		m_dwVtxFVF = 0;
	_ulong 		m_dwIdxSize = 0;
	D3DFORMAT	m_IdxFmt = D3DFMT_UNKNOWN;
public:	
	virtual CComponent*	Clone(void* pArg) = 0;
	virtual void Free(void);

};

END
