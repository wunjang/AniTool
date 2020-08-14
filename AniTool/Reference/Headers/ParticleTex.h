#ifndef PtTex_h__
#define PtTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CParticleTex : public CVIBuffer
{
private:
	explicit CParticleTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticleTex(const CParticleTex& rhs);
	virtual ~CParticleTex(void);

public:
	virtual HRESULT		Ready_Buffer(void);
	virtual	void		Render_Buffer(void);

private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;			// ���� ���� ������ �����ϱ� ���� VB
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;			// Element�� �����ϰ� �����ϴ� �İ�ü(���ؽ� ���̴��� �ԷµǴ� ������ ����Ʈ ������ ����)

public:
	static CParticleTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone(void);
	virtual void	Free(void);
};


END
#endif // PtTex_h__
