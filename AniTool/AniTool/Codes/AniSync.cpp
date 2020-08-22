// ../Codes/AniSync.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AniTool.h"
#include "../Codes/AniSync.h"
#include "afxdialogex.h"


// CAniSync 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAniSync, CDialogEx)

CAniSync::CAniSync(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANISYNC, pParent)
	, m_iFullTime(0)
	, m_iCurTime(0)
{

}

CAniSync::~CAniSync()
{
}

void CAniSync::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_lboxMainPart);
	DDX_Control(pDX, IDC_COMBO1, m_cboxSubParts[0]);
	DDX_Control(pDX, IDC_COMBO2, m_cboxSubParts[1]);
	DDX_Control(pDX, IDC_COMBO3, m_cboxSubParts[2]);
	DDX_Control(pDX, IDC_COMBO4, m_cboxSubParts[3]);
	DDX_Control(pDX, IDC_COMBO5, m_cboxSubParts[4]);
	DDX_Control(pDX, IDC_CHECK3, m_bIsFindSameNameIfExist);
	DDX_Control(pDX, IDC_SLIDER1, m_sldPlayBar);
	DDX_Control(pDX, IDC_CHECK2, m_bIsAnimationLoop);
	DDX_Control(pDX, IDC_BUTTON3, m_btnPlayStop);
	DDX_Text(pDX, IDC_FULLTIME, m_iFullTime);
	DDX_Text(pDX, IDC_CURTIME, m_iCurTime);
	DDX_Control(pDX, IDC_EDIT1, m_editFileName);
}


BEGIN_MESSAGE_MAP(CAniSync, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_LBN_SELCHANGE(IDC_LIST2, &CAniSync::OnLbnSelchangeMainPartsAni)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAniSync::OnCbnSelchangeSubParts)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAniSync::OnCbnSelchangeSubParts)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CAniSync::OnCbnSelchangeSubParts)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CAniSync::OnCbnSelchangeSubParts)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CAniSync::OnCbnSelchangeSubParts)
	ON_BN_CLICKED(IDC_BUTTON1, &CAniSync::OnBnClickedSaveAniComboSet)
	ON_BN_CLICKED(IDC_BUTTON2, &CAniSync::OnBnClickedLoadAniComboSet)
	ON_BN_CLICKED(IDC_BUTTON3, &CAniSync::OnBnClickedPlayStop)
END_MESSAGE_MAP()


// CAniSync 메시지 처리기입니다.


BOOL CAniSync::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pSampleObject = dynamic_cast<CSampleObject*>(ENGINE::Get_GameObject(ENGINE::LAYER_GAMEOBJECT, ENGINE::OBJ_ID::PLAYER));
	NULL_CHECK_RETURN_MSG(m_pSampleObject, TRUE, L"Failed To Get SampleObject !!!");

	for (int i = 0; i < CSampleObject::PARTS_END; ++i)
		m_pPartMesh[i] = m_pSampleObject->Get_Mesh(i);

	LPD3DXANIMATIONCONTROLLER pAniCtrl = m_pPartMesh[0]->Get_AniCtrl()->Get_AniCtrl();
	
	for (_uint i = 0; i < pAniCtrl->GetNumAnimationSets(); ++i)
	{
		LPD3DXANIMATIONSET pAniSet = nullptr;
		pAniCtrl->GetAnimationSet(i, &pAniSet);

		LPCSTR pAniName = pAniSet->GetName();
		wstring wstrConv(&pAniName[0], &pAniName[strlen(pAniName)]);

		m_lboxMainPart.AddString(wstrConv.c_str());
	}

	for (int i = 0; i < CSampleObject::PARTS_END - 1; ++i)
	{
		pAniCtrl = m_pPartMesh[i + 1]->Get_AniCtrl()->Get_AniCtrl();

		for (_uint j = 0; j < pAniCtrl->GetNumAnimationSets(); ++j)
		{
			LPD3DXANIMATIONSET pAniSet = nullptr;
			pAniCtrl->GetAnimationSet(j, &pAniSet);

			LPCSTR pAniName = pAniSet->GetName();
			wstring wstrConv(&pAniName[0], &pAniName[strlen(pAniName)]);
			m_cboxSubParts[i].AddString(wstrConv.c_str());
		}
	}
	


	// Defualt Setting
	m_lboxMainPart.SetCurSel(0);
	for (int i = 0; i < CSampleObject::PARTS_END - 1; ++i)
		m_cboxSubParts[i].SetCurSel(0);
	m_bIsFindSameNameIfExist.SetCheck(true);
	m_bIsAnimationLoop.SetCheck(true);

	m_editFileName.SetWindowTextW(L"PlayerAnimationSet");

	OnLbnSelchangeMainPartsAni();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAniSync::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CDialogEx::OnMouseMove(nFlags, point);
}


void CAniSync::OnLbnSelchangeMainPartsAni()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPartMesh[0]->Set_AnimationSet(m_lboxMainPart.GetCurSel());

	LPD3DXANIMATIONSET pAniSet = nullptr;
	m_pPartMesh[0]->Get_AniCtrl()->Get_AniCtrl()->GetAnimationSet(m_lboxMainPart.GetCurSel(), &pAniSet);
	_int iAniPeriod = (_int)(pAniSet->GetPeriod() * 30.0); // 애니메이션은 30프레임으로 만들어져 있음
	m_sldPlayBar.SetRange(0, iAniPeriod, TRUE);
	m_iFullTime = iAniPeriod;
	m_iCurTime = 0;

	CString strCurMain;
	m_lboxMainPart.GetText(m_lboxMainPart.GetCurSel(), strCurMain);

	if (m_mapAnimationCombine.find(strCurMain) == m_mapAnimationCombine.end())
	{
		//저장된 조합이 없을 경우 같은 이름의 애니메이션을 찾는다
		for (int i = 0; i < CSampleObject::PARTS_END - 1; ++i)
		{
			if (m_bIsFindSameNameIfExist.GetCheck())
			{
				_int iFindSame = m_cboxSubParts[i].FindString(0, strCurMain);
				if (iFindSame >= 0)
					m_cboxSubParts[i].SetCurSel(iFindSame);
			}
			// 같은 이름도 없을 경우 기존의 값으로 놔둔다
			m_pPartMesh[i + 1]->Set_AnimationSet(m_cboxSubParts[i].GetCurSel());
		}
	}
	else
	{
		// 저장된 조합이 있을 경우 그 조합을 불러온다
		LoadAniCombo();
	}


	UpdateData(FALSE);

	SaveAniCombo();
}


void CAniSync::OnCbnSelchangeSubParts()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < CSampleObject::PARTS_END - 1; ++i)
		m_pPartMesh[i + 1]->Set_AnimationSet(m_cboxSubParts[i].GetCurSel());

	// 서브 파트 애니메이션을 바꾸면 그 조합을 저장한다
	SaveAniCombo();
}

void CAniSync::SaveAniCombo()
{
	// 메인 애니에 따라 서브 애니를 저장하는 기능
	CString strCurMain;
	m_lboxMainPart.GetText(m_lboxMainPart.GetCurSel(), strCurMain);

	auto iter_find = m_mapAnimationCombine.find(strCurMain);

	if (iter_find == m_mapAnimationCombine.end())
	{
		vector<_uint> vecAniIdx;
		vecAniIdx.resize(CSampleObject::PARTS_END);
		

		m_mapAnimationCombine.emplace(strCurMain, vecAniIdx);

		iter_find = m_mapAnimationCombine.find(strCurMain);
	}

	iter_find->second[0] = m_lboxMainPart.GetCurSel();
	for (int i = 0; i < CSampleObject::PARTS_END - 1; ++i)
		iter_find->second[i + 1] = m_cboxSubParts[i].GetCurSel();
}

void CAniSync::LoadAniCombo()
{
	// 메인 애니를 선택했을 때 서브 애니를 저장된 대로 선택해주는 기능
	CString strCurMain;
	m_lboxMainPart.GetText(m_lboxMainPart.GetCurSel(), strCurMain);

	auto iter_find = m_mapAnimationCombine.find(strCurMain);

	if (iter_find == m_mapAnimationCombine.end())
		return;

	for (int i = 0; i < CSampleObject::PARTS_END - 1; ++i)
	{
		m_cboxSubParts[i].SetCurSel(iter_find->second[i +1]);
		m_pPartMesh[i + 1]->Set_AnimationSet(m_cboxSubParts[i].GetCurSel());
	}
}

void CAniSync::OnBnClickedSaveAniComboSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString strFullPath;
	m_editFileName.GetWindowTextW(strFullPath);
	strFullPath = L"../Data/" + strFullPath +L".dat";
	HANDLE hFile = CreateFile(strFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX(L"Failed to Create Save File .dat !!!");
		return;
	}

	// Buffer
	TCHAR szAnimationName[64] = L"";

	DWORD dwByte = 0;
	for (auto& rAniCombo : m_mapAnimationCombine)
	{
		StrCpyW(szAnimationName, rAniCombo.first);

		WriteFile(hFile, &szAnimationName, sizeof(szAnimationName), &dwByte, nullptr);
		for (int i = 0; i < CSampleObject::PARTS_END; ++i)
			WriteFile(hFile, &rAniCombo.second[i], sizeof(_uint), &dwByte, nullptr);
	}
	CloseHandle(hFile);
	return;
}


void CAniSync::OnBnClickedLoadAniComboSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (auto& rAniSet : m_mapAnimationCombine)
	{
		rAniSet.second.clear();
		rAniSet.second.shrink_to_fit();
	}
	m_mapAnimationCombine.clear();

	UpdateData(TRUE);
	CString strFullPath;
	m_editFileName.GetWindowTextW(strFullPath);
	strFullPath = L"../Data/" + strFullPath + L".dat";
	
	HANDLE hFile = CreateFile(strFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX(L"Failed to Create Load File .dat !!!");
		return;
	}

	// Buffer
	TCHAR szAnimationName[64] = L"";
	vector<_uint> vecAniSet;
	vecAniSet.resize(CSampleObject::PARTS_END);

	DWORD dwByte = 0;
	while (true)
	{
		ReadFile(hFile, &szAnimationName, sizeof(szAnimationName), &dwByte, nullptr);
		for (int i = 0; i < CSampleObject::PARTS_END; ++i)
			ReadFile(hFile, &vecAniSet[i], sizeof(_uint), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		m_mapAnimationCombine.emplace(szAnimationName, vecAniSet);
	}

	CloseHandle(hFile);
	return;
}


void CAniSync::Set_UpdateAniTime(_uint iTime)
{
	while (iTime > m_iFullTime)
	{
		iTime -= m_iFullTime;
	}
	m_sldPlayBar.SetPos(iTime);
	m_iCurTime = iTime; 
	
	UpdateData(FALSE);
}

void CAniSync::OnBnClickedPlayStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	for (int i = 0; i < CSampleObject::PARTS_END; ++i)
	{
		m_pPartMesh[i]->Get_AniCtrl()->Get_AniCtrl()->SetTrackPosition(0, 0.0);
		m_pPartMesh[i]->Get_AniCtrl()->Get_AniCtrl()->SetTrackPosition(1, 0.0);

		m_pPartMesh[i]->Get_AniCtrl()->Get_AniCtrl()->ResetTime();
	}
}
