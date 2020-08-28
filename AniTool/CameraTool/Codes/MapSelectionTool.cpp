// ../Codes/MapSelectionTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CamTool.h"
#include "../Codes/MapSelectionTool.h"
#include "afxdialogex.h"

#include "Function.h"
#include "MainScene.h"


// CMapSelectionTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapSelectionTool, CDialogEx)

CMapSelectionTool::CMapSelectionTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPSELECTIONTOOL, pParent)
{

}

CMapSelectionTool::~CMapSelectionTool()
{
}

void CMapSelectionTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lboxMap);
}


BEGIN_MESSAGE_MAP(CMapSelectionTool, CDialogEx)
	ON_BN_CLICKED(IDC_LOADMAPDATA, &CMapSelectionTool::OnBnClickedLoadmapdata)
END_MESSAGE_MAP()


// CMapSelectionTool �޽��� ó�����Դϴ�.


BOOL CMapSelectionTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CFunction::FindFileByExtention(L".dat", L"..\\Data\\Map", &m_mapListMapData);

	for (auto iter = m_mapListMapData.begin(); iter != m_mapListMapData.end(); )
	{
		if (iter->second.Find(L"Nav\\") > 0)
		{
			iter = m_mapListMapData.erase(iter);
		}
		else
			++iter;
	}

	for (auto& rMap : m_mapListMapData)
	{
		m_lboxMap.AddString(rMap.first);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMapSelectionTool::OnBnClickedLoadmapdata()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_lboxMap.GetCurSel() < 0 || m_lboxMap.GetCurSel() >= m_lboxMap.GetCount())
		return;
	_int cnt = ENGINE::Get_GameObjectCnt(ENGINE::LAYER_GAMEOBJECT, ENGINE::TERRAIN);
	for (_int i = 0; i < cnt; ++i)
	{
		ENGINE::Get_GameObject(ENGINE::LAYER_GAMEOBJECT, ENGINE::TERRAIN, i)->Set_Dead();
	}

	CString strCurSel;
	m_lboxMap.GetText(m_lboxMap.GetCurSel(), strCurSel);
	FAILED_CHECK_RETURN(CMainScene::Load_MapData(GET_INSTANCE(ENGINE::CGraphicDev)->GetDevice(), ENGINE::LAYER_GAMEOBJECT, m_mapListMapData[strCurSel].GetString()));


}
