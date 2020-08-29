// ../Codes/SampleTargetAnimationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CamTool.h"
#include "../Codes/SampleTargetAnimationTool.h"
#include "afxdialogex.h"

// CSampleTargetAnimationTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSampleTargetAnimationTool, CDialogEx)

CSampleTargetAnimationTool::CSampleTargetAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SAMPLETARGETANIMATIONTOOL, pParent)
	, m_fAniSpeed(0)
{

}

CSampleTargetAnimationTool::~CSampleTargetAnimationTool()
{
}

void CSampleTargetAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAMPLETARGETANIMATIONLIST, m_lboxSampleTargetAnimation);
	DDX_Control(pDX, IDC_SAMPLETARGETANIMATIONPLAY, m_btnSampleTargetAniPlay);
	DDX_Text(pDX, IDC_ANISPEED, m_fAniSpeed);
}


BEGIN_MESSAGE_MAP(CSampleTargetAnimationTool, CDialogEx)
	ON_BN_CLICKED(IDC_SAMPLETARGETANIMATIONPLAY, &CSampleTargetAnimationTool::OnBnClickedSampletargetanimationplay)
	ON_LBN_SELCHANGE(IDC_SAMPLETARGETANIMATIONLIST, &CSampleTargetAnimationTool::OnLbnSelchangeSampletargetanimationlist)
	ON_EN_CHANGE(IDC_ANISPEED, &CSampleTargetAnimationTool::OnEnChangeAnispeed)
END_MESSAGE_MAP()


// CSampleTargetAnimationTool 메시지 처리기입니다.


void CSampleTargetAnimationTool::OnBnClickedSampletargetanimationplay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bIsTextPlay)
	{
		m_pSampleObj->Set_AnimationSpeed(m_fAniSpeed);
		m_btnSampleTargetAniPlay.SetWindowTextW(L"Stop");
	}
	else
	{
		m_pSampleObj->Set_AnimationSpeed(0.f);
		m_btnSampleTargetAniPlay.SetWindowTextW(L"Play");
	}

	m_bIsTextPlay = !m_bIsTextPlay;
}

void CSampleTargetAnimationTool::Set_SampleObj(CSampleObject * pSampleObj)
{
	LPD3DXANIMATIONCONTROLLER pAniCtrl = pSampleObj->Get_Mesh(CSampleObject::PARTS_BODY)->Get_AniCtrl()->Get_AniCtrl();

	for (_uint i = 0; i < pAniCtrl->GetNumAnimationSets(); ++i)
	{
		LPD3DXANIMATIONSET pAniSet = nullptr;
		pAniCtrl->GetAnimationSet(i, &pAniSet);

		LPCSTR pAniName = pAniSet->GetName();
		wstring wstrConv(&pAniName[0], &pAniName[strlen(pAniName)]);

		m_lboxSampleTargetAnimation.AddString(wstrConv.c_str());
	}
}


void CSampleTargetAnimationTool::OnLbnSelchangeSampletargetanimationlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_lboxSampleTargetAnimation.GetCurSel() < 0 && m_lboxSampleTargetAnimation.GetCurSel() >= m_lboxSampleTargetAnimation.GetCount())
		return;

	m_pSampleObj->Set_Animation(ANITAG::PLAYER_BODY(m_lboxSampleTargetAnimation.GetCurSel()));
}


BOOL CSampleTargetAnimationTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_lboxSampleTargetAnimation.SetCurSel(0);
	m_fAniSpeed = 1.8f;

	m_pSampleObj = dynamic_cast<CSampleObject*>(ENGINE::Get_GameObject(ENGINE::LAYER_GAMEOBJECT, ENGINE::PLAYER));

	LPD3DXANIMATIONCONTROLLER pAniCtrl = m_pSampleObj->Get_Mesh(CSampleObject::PARTS_BODY)->Get_AniCtrl()->Get_AniCtrl();

	for (_uint i = 0; i < pAniCtrl->GetNumAnimationSets(); ++i)
	{
		LPD3DXANIMATIONSET pAniSet = nullptr;
		pAniCtrl->GetAnimationSet(i, &pAniSet);

		LPCSTR pAniName = pAniSet->GetName();
		wstring wstrConv(&pAniName[0], &pAniName[strlen(pAniName)]);

		m_lboxSampleTargetAnimation.AddString(wstrConv.c_str());
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSampleTargetAnimationTool::OnEnChangeAnispeed()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_btnSampleTargetAniPlay.SetWindowTextW(L"Play");
	m_bIsTextPlay = true;
}
