#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"
#include "Engine_Defines.h"


BEGIN(ENGINE)


class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit	CInputDev(void);
	virtual		~CInputDev(void);

public:
	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byCurKeyState[byKeyID]; }




	_byte		Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tCurMouseState.rgbButtons[eMouse]; }
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tCurMouseState) + eMouseState);
	}

	_bool		KeyPressing(_ubyte byKeyID);
	_bool		KeyDown(_ubyte byKeyID);
	_bool		KeyUp(_ubyte byKeyID);

	_bool		KeyCombine(_ubyte dwFristKey, _ubyte dwSecondKey);

	_bool		MousePressing(MOUSEKEYSTATE eMouse);
	_bool		MouseDown(MOUSEKEYSTATE eMouse);
	_bool		MouseUp(MOUSEKEYSTATE eMouse);


public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);

	void	Set_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byCurKeyState[256];
	_byte					m_byUpKeyState[256];
	_byte					m_byDownKeyState[256];

	DIMOUSESTATE			m_tCurMouseState;
	DIMOUSESTATE			m_tUpMouseState;
	DIMOUSESTATE			m_tDownMouseState;

public:
	virtual void	Free(void);

};


END

#endif
