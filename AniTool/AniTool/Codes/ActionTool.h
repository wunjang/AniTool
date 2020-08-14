#pragma once


// CActionTool 대화 상자입니다.

class CActionTool : public CDialogEx
{
	DECLARE_DYNAMIC(CActionTool)

public:
	CActionTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CActionTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACTIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
