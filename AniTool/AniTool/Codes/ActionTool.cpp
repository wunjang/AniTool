// ../Codes/ActionTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AniTool.h"
#include "../Codes/ActionTool.h"
#include "afxdialogex.h"


// CActionTool ��ȭ �����Դϴ�.

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


// CActionTool �޽��� ó�����Դϴ�.
