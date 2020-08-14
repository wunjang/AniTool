// ../Codes/ActionTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AniTool.h"
#include "../Codes/ActionTool.h"
#include "afxdialogex.h"


// CActionTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CActionTool, CDialogEx)

CActionTool::CActionTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ACTIONTOOL, pParent)
{

}

CActionTool::~CActionTool()
{
}

void CActionTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CActionTool, CDialogEx)
END_MESSAGE_MAP()


// CActionTool 메시지 처리기입니다.
