#ifndef __LINE_H__
#define __LINE_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CLine : public CBase
{
	DECLARE_SINGLETON(CLine)

private:
	explicit CLine(void);
	virtual ~CLine(void);

public:
	HRESULT		Ready_Line(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Render_Line(const _vec3& v1, const _vec3& v2, const D3DXMATRIX* pTransform, const D3DCOLOR& color, _float fWidth = 1.f);
	void		Render_Polygon(const _vec3* pPoint, _ulong dwPointCount, const D3DXMATRIX* pTransform, const D3DCOLOR& color, _float fWidth = 1.f);

public:

private:
	LPD3DXLINE				m_pLine = nullptr;

public:
	virtual void Free(void);
};

END

#endif // !__LINE_H__
