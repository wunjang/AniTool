#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Font.h"

BEGIN(ENGINE)

class ENGINE_DLL CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr(void);
	virtual ~CFontMgr(void);

public:
	HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring pFontTag,
		const wstring pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void	Render_Font(const wstring pFontTag,const wstring pString,const _vec2* pPos,D3DXCOLOR Color, _matrix Scale, _matrix Rot, _matrix Trans, _bool NoRc = true, _matrix view = {}, _matrix proj = {});
	int		Render_Narration(const wstring pFontTag, const wstring narration, const _vec2* pPos, D3DXCOLOR Color, _matrix Scale, _matrix Rot, _matrix Trans, const _float& fTimeDelta);
	void	Set_Count(const wstring pFontTag,_float count);

private:
	CFont*		Find_Font(const wstring pFontTag);

private:
	map<const wstring, CFont*>		m_mapFont;

public:
	virtual void Free(void);
};

END
#endif // FontMgr_h__
