#pragma once
#include "afxwin.h"

#include "MainFrm.h"
#include "MyForm.h"

// CEventCamTool 대화 상자입니다.

class CEventCamTool : public CDialogEx
{
	enum StaticText {TEXT_BEGINPOS, TEXT_BEGINROT, TEXT_FOLLOW, TEXT_ACTIONLENGTH, TEXT_ACCTIME, TEXT_END};
	DECLARE_DYNAMIC(CEventCamTool)

public:
	CEventCamTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventCamTool();

	static CEventCamTool* Get_Dialog()
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMyForm* pView = dynamic_cast<CMyForm*>(pMain->m_MainSplitWnd.GetPane(0, 0));

		return dynamic_cast<CEventCamTool*>(pView->m_pTab[CMyForm::TOOL_EVENTCAM]);
	}

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENTCAMTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// data
	vector<CAMERAACTION> m_vecCameraAction;

	void Set_FreeCamData(const _vec3& vPos, const _vec3& vAngle, const _float& fViewAngle);

	const vector<CAMERAACTION>* Get_Actions(void) { return &m_vecCameraAction; }
	const CAMERAACTION*	Get_CurAction(void);
	int					Get_CurActionIdx(void);
	void				Set_CurAction(_vec3 vPick);

	void SerchFile(void);

	afx_msg void OnLbnSelchangeCameraactionList();
	afx_msg void OnBnClickedButtonSaveactionedit();
	CStatic m_textTargetAction[TEXT_END];
	CStatic m_textCameraAction[TEXT_END];
	afx_msg void OnBnClickedCameraactionAdd();
	afx_msg void OnBnClickedCameracationRemove();
	float m_vMoveTo[3];
	float m_vRotateTo[3];
	CEdit m_editActionName;
	int m_iFollowOBJ_ID;
	int m_iFollowOBJ_IDX;
	float m_fDistance;
	float m_fActionLength;
	virtual BOOL OnInitDialog();
	CListBox m_lboxCameraAction;
	float m_fViewAngleTo;
	CButton m_cbIsFollowTarget;
	afx_msg void OnBnClickedCheckIsFollowTarget();
	afx_msg void OnBnClickedButtonPlay();
	CStatic m_textCurveText;
	afx_msg void OnBnClickedFilesave();
	afx_msg void OnBnClickedFileload();
	CEdit m_editFileName;
	CListBox m_lboxFiles;
	afx_msg void OnLbnSelchangeFilelist();
	afx_msg void OnBnClickedButtonPlayevent();
	CComboBox m_cboxEffect;
	CStatic m_textFreeCamPos;
	CStatic m_textFreeCamAngle;
	afx_msg void OnBnClickedFreecamCopy();
	CStatic m_textFreeCamViewAngle;
	afx_msg void OnBnClickedTargetmove();
	afx_msg void OnBnClickedTargetCopycam();
	float m_fTargetPos[3];
	float m_fTargetAngle[3];
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedFreecamMove();
};
