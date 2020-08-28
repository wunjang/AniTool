#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "Timer.h"
#include "Engine_Defines.h"

BEGIN(ENGINE)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)


private:
	explicit CTimerMgr(void);
	virtual ~CTimerMgr(void);

public:

	void		Set_TimeDelta(const TIMER::TYPE eTimerTag);
	_float		Get_TimeDelta(const TIMER::TYPE eTimerTag);
	void		Set_TimerState(const TIMER::TYPE eTimerTag, _bool bTimerState);

	CTimer*		Find_Timer(const TIMER::TYPE eTimerTag);
public:
	HRESULT	Ready_Timer(const TIMER::TYPE eTimerTag, const _float fTimeLimit = 0.03f);





private:
	vector<CTimer*> m_vecTimer;
		
public:
	virtual void Free(void);
};

END
#endif // TimerMgr_h__
