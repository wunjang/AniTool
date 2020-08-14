#pragma once

#define			MSG_BOX(_message)			AfxMessageBox(_message);
#define			TAGMSG_BOX(_tag,_message)	AfxMessageBox(_tag);
#define			TAGMSG_BOXA(_tag,_message)	AfxMessageBox(_tag);

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){AfxMessageBox(L"System Message");__debugbreak();}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){AfxMessageBox(L"System Message");__debugbreak();return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ AfxMessageBox(L"Failed"); __debugbreak(); return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ AfxMessageBox(L"Failed"); __debugbreak(); return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ AfxMessageBox(_message); __debugbreak();return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ AfxMessageBox(_message); __debugbreak();return _return;}