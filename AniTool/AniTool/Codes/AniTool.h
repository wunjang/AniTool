
// AniTool.h : AniTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CAniToolApp:
// �� Ŭ������ ������ ���ؼ��� AniTool.cpp�� �����Ͻʽÿ�.
//
class CAniToolView;
class CAniToolApp : public CWinApp
{
public:
	CAniToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

	CAniToolView* pView = nullptr;
};

extern CAniToolApp theApp;
