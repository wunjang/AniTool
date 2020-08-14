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
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;			// 기존 정점 정보를 복제하기 위한 VB
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;			// Element를 보관하고 관리하는 컴객체(버텍스 쉐이더로 입력되는 정점의 데어트 정보를 보관)

public:
	static CParticleTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone(void);
	virtual void	Free(void);
};


END
#endif // PtTex_h__
