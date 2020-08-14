#ifndef Frame_h__
#define Frame_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);

public:
	_bool		IsPermit_Call(const _float& fTimeDelta);
	void		Render_FPS(const _vec2& vPos, const _tchar* pFontTag, const D3DXCOLOR& color);

public:
	HRESULT		Ready_Frame(const _float& fCallLimit);

private:
	_float		m_fCallLimit;		// 제한된 시간 값
	_float		m_fAccTimeDelta;	// 누적된 시간 저장하기 위한 변수

	_float		m_fAccTimeSecond;
	_uint		m_iFramePerSec;		// 초당 프레임
	_uint		m_iFrameCount;

public:
	static CFrame*		Create(const _float& fCallLimit);
	virtual void		Free(void);
};

END
#endif // Frame_h__
