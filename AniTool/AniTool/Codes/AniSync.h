#pragma once
#include "afxwin.h"

#include "SmapleObject.h"
#include "afxcmn.h"
#include "MainFrm.h"
#include "MyForm.h"

// CAniSync ��ȭ �����Դϴ�.

class CAniSync : public CDialogEx
{
	DECLARE_DYNAMIC(CAniSync)

public:
	CAniSync(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAniSync();

	static CAniSync* Get_Dialog()
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMyForm* pView = dynamic_cast<CMyForm*>(pMain->m_MainSplitWnd.GetPane(0, 0));

		return dynamic_cast<CAniSync*>(pView->m_pTab[CMyForm::TOOL_ANISYNC]);
	}

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANISYNC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lboxMainPart;
	ENGINE::CDynamicMesh* m_pPartMesh[CSampleObject::PARTS_END];

	CFont m_Font;

	CComboBox m_cboxSubParts[CSampleObject::PARTS_END - 1];

	CButton m_bIsFindSameNameIfExist;
	virtual BOOL OnInitDialog();

	CSampleObject* m_pSampleObject;
	map<CString, vector<_uint>> m_mapAnimationCombine;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLbnSelchangeMainPartsAni();
	afx_msg void OnCbnSelchangeSubParts();


	void SaveAniCombo();
	void LoadAniCombo();

	afx_msg void OnBnClickedSaveAniComboSet();
	afx_msg void OnBnClickedLoadAniComboSet();
	CString m_strSaveFileName;


	CSliderCtrl m_sldPlayBar;
	CSliderCtrl* Get_PlaySlider() { return &m_sldPlayBar; }
	void Set_UpdateAniTime(_uint iTime);

	CButton m_bIsAnimationLoop;
	_bool Get_IsAnimationLoop() { return m_bIsAnimationLoop.GetCheck() != 0; }

	CButton m_btnPlayStop;
	_bool m_bIsPlaying;
	afx_msg void OnBnClickedPlayStop();
	CString m_strPlayBarText;
	UINT m_iFullTime;
	UINT m_iCurTime;
};
