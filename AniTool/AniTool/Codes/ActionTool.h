#pragma once


// CActionTool ��ȭ �����Դϴ�.

class CActionTool : public CDialogEx
{
	DECLARE_DYNAMIC(CActionTool)

public:
	CActionTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CActionTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACTIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
