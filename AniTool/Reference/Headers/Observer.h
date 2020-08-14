#pragma once

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CObserver : public CBase
{
public:
	explicit CObserver(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObserver();

public:
	virtual void Update(int iMessage, void* pData) PURE;

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

public:

protected:
	virtual void			Free(void);
};

END

#endif

