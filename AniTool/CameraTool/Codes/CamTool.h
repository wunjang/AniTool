
// CamTool.h : CamTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CCamToolApp:
// �� Ŭ������ ������ ���ؼ��� CamTool.cpp�� �����Ͻʽÿ�.
//
class CCamToolView;
class CCamToolApp : public CWinApp
{
public:
	CCamToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

	CCamToolView* pView = nullptr;
};

extern CCamToolApp theApp;
