// ../Codes/MapSelectionTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CamTool.h"
#include "../Codes/MapSelectionTool.h"
#include "afxdialogex.h"

#include "Function.h"
#include "MainScene.h"


// CMapSelectionTool 대화 상자입니다.

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


// CMapSelectionTool 메시지 처리기입니다.


BOOL CMapSelectionTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapSelectionTool::OnBnClickedLoadmapdata()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
