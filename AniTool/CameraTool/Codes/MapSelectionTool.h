#pragma once
#include "afxwin.h"


// CMapSelectionTool ��ȭ �����Դϴ�.

class CMapSelectionTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapSelectionTool)

public:
	CMapSelectionTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapSelectionTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPSELECTIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lboxMap;
	virtual BOOL OnInitDialog();

	map<CString, CString> m_mapListMapData;
	afx_msg void OnBnClickedLoadmapdata();
};
