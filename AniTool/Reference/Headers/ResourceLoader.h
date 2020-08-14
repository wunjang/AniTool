#ifndef __RESOURCELOADER_H__
#define __RESOURCELOADER_H__

#include "Engine_Defines.h"
#include "Base.h"

#include "Loading.h"

BEGIN(ENGINE)

class ENGINE_DLL CResourceLoader : public CBase
{
	DECLARE_SINGLETON(CResourceLoader)
private:
	explicit CResourceLoader(void);
	virtual ~CResourceLoader(void);

public:
	HRESULT		Ready_Loading(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Add_Loading(function<_long(void)> fnLoading, const wstring& wstrLoadingMsg, const wstring& wstrFinishMsg);
	_ulong		Check_Loading();


private:
	LPDIRECT3DDEVICE9										m_pGraphicDev;
	CLoading*												m_pLoading;
	list<tuple<function<_long(void)>, wstring, wstring>>	m_listLoadingQueue;

private:
	virtual void			Free(void);

};
END

#endif // __RESOURCELOADER_H__
