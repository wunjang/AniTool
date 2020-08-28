#pragma once
#include "afxwin.h"


// CMapSelectionTool 대화 상자입니다.

class CMapSelectionTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapSelectionTool)

public:
	CMapSelectionTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapSelectionTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPSELECTIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lboxMap;
	virtual BOOL OnInitDialog();

	map<CString, CString> m_mapListMapData;
	afx_msg void OnBnClickedLoadmapdata();
};
