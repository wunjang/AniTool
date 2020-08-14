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
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;			// ���� ���� ������ �����ϱ� ���� VB
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;			// Element�� �����ϰ� �����ϴ� �İ�ü(���ؽ� ���̴��� �ԷµǴ� ������ ����Ʈ ������ ����)

public:
	static CParticleBuffer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void * pArg);
	virtual void	Free(void);

};

END