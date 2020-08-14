#ifndef __LOADING_H__
#define __LOADING_H__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(ENGINE)

class ENGINE_DLL CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }
	_bool				Get_FinishOnce(void) 
	{
		if (m_bFinish == m_bFinishOnce) 
			return false; 
		m_bFinishOnce = m_bFinish;
		return true;
	}
	const wstring&		Get_Msg(void);

public:
	static unsigned int CALLBACK	Thread_Main(void* pArg);
	static unsigned int CALLBACK	Thread_Sub(void* pArg);

public:
	HRESULT				Ready_Loading(function<_ulong(void)> fnLoading, const wstring& wstrLoadingMsg, const wstring& wstrFinishMsg);
	_uint				Loading(void);

private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_Crt;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bFinish;
	_bool					m_bFinishOnce;
	function<_uint(void)>	m_fnLoading;
	wstring					m_wstrLoadingMsg;
	wstring					m_wstrFinishMsg;

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, function<_ulong(void)> fnLoading, const wstring& wstrLoadingMsg = L"Loading...", const wstring& wstrFinishMsg = L"Loading Complete");
private:
	virtual void			Free(void);

};

END

#endif // __LOADING_H__
