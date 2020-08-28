// ../Codes/MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CamTool.h"
#include "MyForm.h"

#include "EventCamTool.h"
#include "MapSelectionTool.h"


HWND g_FormHWND;

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
	for (int i = 0; i < TOOL_END; ++i)
		ENGINE::Safe_Delete(m_pTab[i]);
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabTools);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	g_FormHWND = m_hWnd;

	m_tabTools.InsertItem(0, L"EventCam");
	m_pTab[TOOL_EVENTCAM] = new CEventCamTool;
	m_pTab[TOOL_EVENTCAM]->Create(IDD_EVENTCAMTOOL, &m_tabTools);
	
	m_tabTools.InsertItem(1, L"MapSelect");
	m_pTab[TOOL_MAPSELECT] = new CMapSelectionTool;
	m_pTab[TOOL_MAPSELECT]->Create(IDD_MAPSELECTIONTOOL, &m_tabTools);


	// �����
	CRect rc;
	m_tabTools.GetWindowRect(&rc);
	
	for (int i = 0; i < TOOL_END; ++i)
	{
		m_pTab[i]->MoveWindow(0, 21, rc.Width() - 2, rc.Height() - 21);
		m_pTab[i]->ShowWindow(SW_HIDE);
	}
	m_pTab[0]->ShowWindow(SW_SHOW);
}


void CMyForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < TOOL_END; ++i)
		m_pTab[i]->ShowWindow(SW_HIDE);
	m_pTab[m_tabTools.GetCurSel()]->ShowWindow(SW_SHOW);


	*pResult = 0;
}
