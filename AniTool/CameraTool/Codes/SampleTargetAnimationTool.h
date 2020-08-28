#pragma once
#include "afxwin.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "EventCamTool.h"

#include "SmapleObject.h"

// CSampleTargetAnimationTool ��ȭ �����Դϴ�.

class CSampleTargetAnimationTool : public CDialogEx
{
	DECLARE_DYNAMIC(CSampleTargetAnimationTool)

public:
	CSampleTargetAnimationTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSampleTargetAnimationTool();

	static CSampleTargetAnimationTool* Get_Dialog()
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMyForm* pView = dynamic_cast<CMyForm*>(pMain->m_MainSplitWnd.GetPane(0, 0));

		return dynamic_cast<CSampleTargetAnimationTool*>(dynamic_cast<CEventCamTool*>(pView->m_pTab[CMyForm::TOOL_EVENTCAM])->Get_SampleAniTool());
	}

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLETARGETANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lboxSampleTargetAnimation;
	afx_msg void OnBnClickedSampletargetanimationplay();
	CButton m_btnSampleTargetAniPlay;

	void		Set_SampleObj(CSampleObject* pSampleObj);
	CSampleObject*	m_pSampleObj = nullptr;
	afx_msg void OnLbnSelchangeSampletargetanimationlist();
	float m_fAniSpeed;
	virtual BOOL OnInitDialog();
	bool m_bIsTextPlay = true;
	afx_msg void OnEnChangeAnispeed();
};
