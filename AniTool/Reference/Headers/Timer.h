#ifndef Timer_h__
#define Timer_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);

public:
	HRESULT		Ready_Timer(void);
	void		Set_TimeDelta(void);
	_float		Get_TimeDelta(void);
	void		Set_TimeLimit(_float fTimeLimit) { m_fTimeLimit = fTimeLimit; }
	void		Set_TimerState(_bool bState) { m_bTimeOn = bState; }
private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_CpuTick;


	_float					m_fTimeDelta = 0.f;
	_float					m_fTimeLimit = 0.f;
	_bool					m_bTimeOn;
public:
	static CTimer*	Create(void);
	virtual void	Free(void);

};
END
#endif // Timer_h__
