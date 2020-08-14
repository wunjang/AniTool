#pragma once
#include "VIBuffer.h"
BEGIN(ENGINE)
class ENGINE_DLL CParticleBuffer :
	public CVIBuffer
{
private:
	explicit CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticleBuffer(const CParticleBuffer& rhs);
	virtual ~CParticleBuffer(void);

public:
	virtual HRESULT		Ready_Component_Prototype(void);

	virtual	void		Render_Instance_Buffer(DWORD dwRenderCount);
	void UpdateInstance(void * pData, _uint iSize);
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;			// 기존 정점 정보를 복제하기 위한 VB
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;			// Element를 보관하고 관리하는 컴객체(버텍스 쉐이더로 입력되는 정점의 데어트 정보를 보관)

public:
	static CParticleBuffer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void * pArg);
	virtual void	Free(void);

};

END