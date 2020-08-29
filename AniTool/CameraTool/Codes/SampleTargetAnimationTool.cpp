// ../Codes/SampleTargetAnimationTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CamTool.h"
#include "../Codes/SampleTargetAnimationTool.h"
#include "afxdialogex.h"

// CSampleTargetAnimationTool ��ȭ �����Դϴ�.

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


// CSampleTargetAnimationTool �޽��� ó�����Դϴ�.


void CSampleTargetAnimationTool::OnBnClickedSampletargetanimationplay()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_lboxSampleTargetAnimation.GetCurSel() < 0 && m_lboxSampleTargetAnimation.GetCurSel() >= m_lboxSampleTargetAnimation.GetCount())
		return;

	m_pSampleObj->Set_Animation(ANITAG::PLAYER_BODY(m_lboxSampleTargetAnimation.GetCurSel()));
}


BOOL CSampleTargetAnimationTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSampleTargetAnimationTool::OnEnChangeAnispeed()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_btnSampleTargetAniPlay.SetWindowTextW(L"Play");
	m_bIsTextPlay = true;
}
