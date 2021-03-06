// ../Codes/MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AniTool.h"
#include "MyForm.h"

#include "AniSync.h"
#include "ActionTool.h"


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


// CMyForm 진단입니다.

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


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_tabTools.InsertItem(0, L"Sync");
	m_pTab[TOOL_ANISYNC] = new CAniSync;
	m_pTab[TOOL_ANISYNC]->Create(IDD_ANISYNC, &m_tabTools);

	m_tabTools.InsertItem(1, L"Action");
	m_pTab[TOOL_ACTION] = new CActionTool;
	m_pTab[TOOL_ACTION]->Create(IDD_ACTIONTOOL, &m_tabTools);


	// 공통부
	CRect rc;
	m_tabTools.GetWindowRect(&rc);

	for (int i = 0; i < TOOL_END; ++i)
	{
		m_pTab[i]->MoveWindow(0, 21, rc.Width() - 2, rc.Height() - 21);
		m_pTab[i]->ShowWindow(SW_HIDE);
	}
	m_pTab[0]->ShowWindow(SW_SHOW);


	UpdateData(FALSE);
}


void CMyForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < TOOL_END; ++i)
		m_pTab[i]->ShowWindow(SW_HIDE);
	m_pTab[m_tabTools.GetCurSel()]->ShowWindow(SW_SHOW);
	ENGINE::CScene* pScene = nullptr;


	*pResult = 0;
}
