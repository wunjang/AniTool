#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"

BEGIN(ENGINE)

// GraphicDev
// Get
// Set
// General;
inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, ENGINE::CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);


// TimerMgr
// Get
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
// Set
inline void			Set_TimeDelta(const _tchar* pTimerTag);
// General;
inline HRESULT		Ready_Timer(const _tchar* pTimerTag);

// FrameMgr
// Get
inline _bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
// Set
// General
inline 	HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
inline void			Render_FPS(const _tchar* pFrameTag, const _vec2& vPos = _vec2(10.f, 15.f), const _tchar* pFontTag = L"Font_Default", const D3DXCOLOR& color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

// FontMgr
// Get
// Set
// General
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring pFontTag,
	const wstring pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void	Render_Font(const wstring pFontTag,
	const wstring pString,
	const _vec2* pPos,
	D3DXCOLOR Color);

// InputDev
// Get
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

inline _bool		KeyPressing(_ubyte byKeyID);
inline _bool		KeyDown(_ubyte byKeyID);
inline _bool		KeyUp(_ubyte byKeyID);

inline _bool		MousePressing(MOUSEKEYSTATE eMouse);
inline _bool		MouseDown(MOUSEKEYSTATE eMouse);
inline _bool		MouseUp(MOUSEKEYSTATE eMouse);
// Set
// General
inline HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void	Set_InputDev(void);

inline void Release_System(void);

#include "Export_System.inl"

END


#endif // Export_System_h__
