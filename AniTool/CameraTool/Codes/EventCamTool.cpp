// ../Codes/EventCamTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CamTool.h"
#include "../Codes/EventCamTool.h"
#include "afxdialogex.h"

#include "EventCamera.h"
#include "FreeCamera.h"
#include "SmapleObject.h"


// CEventCamTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventCamTool, CDialogEx)

CEventCamTool::CEventCamTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EVENTCAMTOOL, pParent)
	, m_iFollowOBJ_IDX(0)
	, m_fDistance(0)
	, m_fActionLength(0)
	, m_fViewAngleTo(0)
{
	ZeroMemory(m_vMoveTo, sizeof(float) * 3);
	ZeroMemory(m_vRotateTo, sizeof(float) * 3);
	ZeroMemory(m_fTargetPos, sizeof(float) * 3);
	ZeroMemory(m_fTargetAngle, sizeof(float) * 3);
}

CEventCamTool::~CEventCamTool()
{
	m_vecCameraAction.clear();
	m_vecCameraAction.shrink_to_fit();
}

void CEventCamTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERAACTION_BEGINPOS, m_textCameraAction[TEXT_BEGINPOS]);
	DDX_Control(pDX, IDC_CAMERAACTION_BEGINROT, m_textCameraAction[TEXT_BEGINROT]);
	DDX_Control(pDX, IDC_CAMERAACTION_FOLLOW, m_textCameraAction[TEXT_FOLLOW]);
	DDX_Control(pDX, IDC_CAMERAACTION_LENGTH, m_textCameraAction[TEXT_ACTIONLENGTH]);
	DDX_Control(pDX, IDC_CAMERAACTION_ACCTIME, m_textCameraAction[TEXT_ACCTIME]);


	DDX_Text(pDX, IDC_MOVE_X, m_vMoveTo[ENGINE::ROT_X]);
	DDX_Text(pDX, IDC_MOVE_Y, m_vMoveTo[ENGINE::ROT_Y]);
	DDX_Text(pDX, IDC_MOVE_Z, m_vMoveTo[ENGINE::ROT_Z]);

	DDX_Text(pDX, IDC_ROT_X, m_vRotateTo[ENGINE::ROT_X]);
	DDX_Text(pDX, IDC_ROT_Y, m_vRotateTo[ENGINE::ROT_Y]);
	DDX_Text(pDX, IDC_ROT_Z, m_vRotateTo[ENGINE::ROT_Z]);
	DDX_Control(pDX, IDC_ACTIONNAME, m_editActionName);
	DDX_Text(pDX, IDC_FOLLOWOBJ_ID, m_iFollowOBJ_ID);
	DDX_Text(pDX, IDC_FOLLOWOBJ_IDX, m_iFollowOBJ_IDX);
	DDX_Text(pDX, IDC_MOVE_CURVE, m_fDistance);
	DDX_Text(pDX, IDC_ACTION_LENGTH, m_fActionLength);
	DDX_Control(pDX, IDC_CAMERAACTION_LIST, m_lboxCameraAction);
	DDX_Text(pDX, IDC_VIEWANGLE, m_fViewAngleTo);
	DDV_MinMaxFloat(pDX, m_fViewAngleTo, 0, 180);
	DDX_Control(pDX, IDC_CHECK_ISFOLLOW, m_cbIsFollowTarget);
	DDX_Control(pDX, IDC_CURVE_DISTANCE_TEXT, m_textCurveText);
	DDX_Control(pDX, IDC_FILENAME, m_editFileName);
	DDX_Control(pDX, IDC_FILELIST, m_lboxFiles);
	DDX_Control(pDX, IDC_EFFECT, m_cboxEffect);
	DDX_Control(pDX, IDC_FREECAM_POS, m_textFreeCamPos);
	DDX_Control(pDX, IDC_FREECAM_ANGLE, m_textFreeCamAngle);
	DDX_Control(pDX, IDC_FREECAM_VIEWANGLE, m_textFreeCamViewAngle);
	DDX_Text(pDX, IDC_TARGET_X, m_fTargetPos[ENGINE::ROT_X]);
	DDX_Text(pDX, IDC_TARGET_Y, m_fTargetPos[ENGINE::ROT_Y]);
	DDX_Text(pDX, IDC_TARGET_Z, m_fTargetPos[ENGINE::ROT_Z]);
	DDX_Text(pDX, IDC_TARGETROT_X, m_fTargetAngle[ENGINE::ROT_X]);
	DDX_Text(pDX, IDC_TARGETROT_Y, m_fTargetAngle[ENGINE::ROT_Y]);
	DDX_Text(pDX, IDC_TARGETROT_Z, m_fTargetAngle[ENGINE::ROT_Z]);
}


BEGIN_MESSAGE_MAP(CEventCamTool, CDialogEx)
	ON_LBN_SELCHANGE(IDC_CAMERAACTION_LIST, &CEventCamTool::OnLbnSelchangeCameraactionList)
	ON_BN_CLICKED(IDC_CAMERAACTION_ADD, &CEventCamTool::OnBnClickedCameraactionAdd)
	ON_BN_CLICKED(IDC_CAMERACATION_REMOVE, &CEventCamTool::OnBnClickedCameracationRemove)
	ON_BN_CLICKED(IDC_BUTTON_SAVEACTIONEDIT, &CEventCamTool::OnBnClickedButtonSaveactionedit)
	ON_BN_CLICKED(IDC_CHECK_ISFOLLOW, &CEventCamTool::OnBnClickedCheckIsFollowTarget)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CEventCamTool::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_FILESAVE, &CEventCamTool::OnBnClickedFilesave)
	ON_BN_CLICKED(IDC_FILELOAD, &CEventCamTool::OnBnClickedFileload)
	ON_LBN_SELCHANGE(IDC_FILELIST, &CEventCamTool::OnLbnSelchangeFilelist)
	ON_BN_CLICKED(IDC_BUTTON_PLAYEVENT, &CEventCamTool::OnBnClickedButtonPlayevent)
	ON_BN_CLICKED(IDC_FREECAM_COPY, &CEventCamTool::OnBnClickedFreecamCopy)
	ON_BN_CLICKED(IDC_TARGETMOVE, &CEventCamTool::OnBnClickedTargetmove)
	ON_BN_CLICKED(IDC_TARGET_COPYCAM, &CEventCamTool::OnBnClickedTargetCopycam)
	ON_BN_CLICKED(IDC_STOP, &CEventCamTool::OnBnClickedStop)
	ON_BN_CLICKED(IDC_FREECAM_MOVE, &CEventCamTool::OnBnClickedFreecamMove)
END_MESSAGE_MAP()


// CEventCamTool 메시지 처리기입니다.

void CEventCamTool::Set_FreeCamData(const _vec3& vPos, const _vec3& vAngle, const _float& fViewAngle)
{
	CString strPos, strAngle, strViewAngle;
	strPos.Format(L"[%.2f, %.2f, %.2f]", vPos.x, vPos.y, vPos.z);
	strAngle.Format(L"[%.2f, %.2f, %.2f]", vAngle.x, vAngle.y, vAngle.z);
	strViewAngle.Format(L"%.2f", fViewAngle);

	m_textFreeCamPos.SetWindowTextW(strPos);
	m_textFreeCamAngle.SetWindowTextW(strAngle);
	m_textFreeCamViewAngle.SetWindowTextW(strViewAngle);
}

const CAMERAACTION * CEventCamTool::Get_CurAction(void)
{
	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return nullptr;

	return &m_vecCameraAction[m_lboxCameraAction.GetCurSel()];
}

int CEventCamTool::Get_CurActionIdx(void)
{
	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return -1;

	return m_lboxCameraAction.GetCurSel();
}

void CEventCamTool::Set_CurAction(_vec3 vPick)
{
	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return;

	m_vecCameraAction[m_lboxCameraAction.GetCurSel()].vMoveTo = vPick;
}

void CEventCamTool::SerchFile(void)
{
	m_lboxFiles.ResetContent();

	CFileFind find;
	BOOL bContinue = FALSE;

	bContinue = find.FindFile(L"..\\Data\\EventCamera\\*.dat");
	while (bContinue)
	{
		bContinue = find.FindNextFile();
		if (find.IsDots() || find.IsSystem() || find.IsDirectory())
		{
			continue;
		}
		else
		{
			m_lboxFiles.AddString(find.GetFileTitle());
		}
	}
}

void CEventCamTool::OnLbnSelchangeCameraactionList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return;
	UpdateData(TRUE);

	const CAMERAACTION& CurAction = m_vecCameraAction[m_lboxCameraAction.GetCurSel()];

	m_editActionName.SetWindowTextW(CurAction.strName);

	m_cbIsFollowTarget.SetCheck(CurAction.bIsFollow);
	OnBnClickedCheckIsFollowTarget();
	
	memcpy(m_vMoveTo, CurAction.vMoveTo, sizeof(float) * 3);
	memcpy(m_vRotateTo, CurAction.vRotateTo, sizeof(float) * 3);
	//m_vMoveTo = CurAction.vMoveTo;
	//m_vRotateTo = CurAction.vRotateTo;

	m_fViewAngleTo = CurAction.fViewAngleTo;

	m_fActionLength = CurAction.fLength;

	UpdateData(FALSE);
}


void CEventCamTool::OnBnClickedCameraactionAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_lboxCameraAction.AddString(L"NewAction");
	m_vecCameraAction.push_back(CAMERAACTION());

	UpdateData(TRUE);

}


void CEventCamTool::OnBnClickedCameracationRemove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	auto iter = m_vecCameraAction.begin();
	m_vecCameraAction.erase(iter + m_lboxCameraAction.GetCurSel());
	m_lboxCameraAction.DeleteString(m_lboxCameraAction.GetCurSel());
}


void CEventCamTool::OnBnClickedButtonSaveactionedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return;

	int iListCurSel = m_lboxCameraAction.GetCurSel();
	CAMERAACTION& CurAction = m_vecCameraAction[iListCurSel];

	m_editActionName.GetWindowTextW(CurAction.strName);
	m_lboxCameraAction.DeleteString(iListCurSel);
	m_lboxCameraAction.InsertString(iListCurSel, CurAction.strName);
	m_lboxCameraAction.SetCurSel(iListCurSel);

	CurAction.bIsFollow = m_cbIsFollowTarget.GetCheck() != 0;

	if (CurAction.bIsFollow)
	{
		CurAction.eOBJ_ID = m_iFollowOBJ_ID;
		CurAction.eObj_Idx = m_iFollowOBJ_IDX;
	}
	
	memcpy(CurAction.vMoveTo, m_vMoveTo, sizeof(float) * 3);
	memcpy(CurAction.vRotateTo, m_vRotateTo, sizeof(float) * 3);
	//CurAction.vMoveTo = m_vMoveTo;
	//CurAction.vRotateTo = m_vRotateTo;
	CurAction.fViewAngleTo = m_fViewAngleTo;
	CurAction.fLength = m_fActionLength;
	CurAction.fDistance = m_fDistance;

	UpdateData(FALSE);
}


BOOL CEventCamTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// cboxOBJ_ID
	// 일단 보류

	SerchFile();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEventCamTool::OnBnClickedCheckIsFollowTarget()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return;

	const CAMERAACTION& CurAction = m_vecCameraAction[m_lboxCameraAction.GetCurSel()];

	if (m_cbIsFollowTarget.GetCheck())
	{
		// 추적 카메라의 경우
		GetDlgItem(IDC_FOLLOWOBJ_ID)->EnableWindow(TRUE);
		GetDlgItem(IDC_FOLLOWOBJ_IDX)->EnableWindow(TRUE);
		GetDlgItem(IDC_MOVE_CURVE)->EnableWindow(TRUE);

		m_iFollowOBJ_ID = CurAction.eOBJ_ID;
		m_iFollowOBJ_IDX = CurAction.eObj_Idx;
		m_fDistance = CurAction.fDistance;
	}
	else
	{
		// 자유 카메라의 경우
		m_iFollowOBJ_ID = 0;
		m_iFollowOBJ_IDX = 0;

		GetDlgItem(IDC_FOLLOWOBJ_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_FOLLOWOBJ_IDX)->EnableWindow(FALSE);
		GetDlgItem(IDC_MOVE_CURVE)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}


void CEventCamTool::OnBnClickedButtonPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_FREE);
	dynamic_cast<CEventCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_EVENT))->Set_Action(m_vecCameraAction);
}


void CEventCamTool::OnBnClickedFilesave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_vecCameraAction.empty())
		return;

	CString strFullPath;
	m_editFileName.GetWindowTextW(strFullPath);

	if (strFullPath == L"")
	{
		MSG_BOX(L"이름을 넣어야 합니다 !!!");
		return;
	}

	strFullPath = L"../Data/EventCamera/" + strFullPath + L".dat";
	HANDLE hFile = CreateFile(strFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX(L"Failed to Create Save File .dat !!!");
		return;
	}

	// Buffer
	TCHAR szActionName[64] = L"";

	DWORD dwByte = 0;
	for (auto& rAction : m_vecCameraAction)
	{
		StrCpyW(szActionName, rAction.strName);
		WriteFile(hFile, &szActionName, sizeof(szActionName), &dwByte, nullptr);
		WriteFile(hFile, &rAction.bIsFollow, sizeof(CAMERAACTION) - sizeof(CString), &dwByte, nullptr);
	}
	CloseHandle(hFile);

	SerchFile();

	return;
}


void CEventCamTool::OnBnClickedFileload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	UpdateData(TRUE);
	CString strFullPath;
	m_editFileName.GetWindowTextW(strFullPath);
	strFullPath = L"../Data/EventCamera/" + strFullPath + L".dat";

	HANDLE hFile = CreateFile(strFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX(L"Failed to Create Load File .dat !!!");
		return;
	}

	m_vecCameraAction.clear();
	m_vecCameraAction.shrink_to_fit();

	m_lboxCameraAction.ResetContent();

	// Buffer
	TCHAR szActionName[64] = L"";
	CAMERAACTION Action;

	DWORD dwByte = 0;
	while (true)
	{
		ReadFile(hFile, &szActionName, sizeof(szActionName), &dwByte, nullptr);
		ReadFile(hFile, &Action.bIsFollow, sizeof(CAMERAACTION) - sizeof(CString), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		Action.strName = szActionName;
		m_lboxCameraAction.AddString(Action.strName);
		m_vecCameraAction.push_back(Action);
	}

	CloseHandle(hFile);
	return;
}


void CEventCamTool::OnLbnSelchangeFilelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_lboxFiles.GetCount() < m_lboxFiles.GetCurSel() || m_lboxFiles.GetCurSel() < 0)
		return;

	CString strCurFileName;
	m_lboxFiles.GetText(m_lboxFiles.GetCurSel(), strCurFileName);

	m_editFileName.SetWindowTextW(strCurFileName);
}


void CEventCamTool::OnBnClickedButtonPlayevent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_EVENT);
	dynamic_cast<CEventCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_EVENT))->Set_Action(m_vecCameraAction);
}


void CEventCamTool::OnBnClickedFreecamCopy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CFreeCamera* pFreeCam = static_cast<CFreeCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_FREE));
	memcpy(m_vMoveTo, pFreeCam->Get_Pos(), sizeof(_vec3));
	memcpy(m_vRotateTo, pFreeCam->Get_Angle(), sizeof(_vec3));
	UpdateData(FALSE);
}


void CEventCamTool::OnBnClickedTargetmove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 vPos, vAngle;
	memcpy(&vPos, m_fTargetPos, sizeof(_vec3));
	memcpy(&vAngle, m_fTargetAngle, sizeof(_vec3));

	ENGINE::CTransform*	pTransform = dynamic_cast<ENGINE::CTransform*>(ENGINE::Get_Component(ENGINE::LAYER_GAMEOBJECT, ENGINE::PLAYER, ENGINE::COMPONENT::TAG_TRANSFORM, ENGINE::COMPONENT::ID_DYNAMIC));
	NULL_CHECK(pTransform);
	pTransform->Set_Pos(&vPos);
	pTransform->Set_Angle(vAngle);
}


void CEventCamTool::OnBnClickedTargetCopycam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	const _vec3& vPos = dynamic_cast<CFreeCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_FREE))->Get_Pos();
	const _vec3& vAngle = dynamic_cast<CFreeCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_FREE))->Get_Angle();

	memcpy(m_fTargetPos, &vPos, sizeof(float) * 3);
	memcpy(m_fTargetAngle, &vAngle, sizeof(float) * 3);

	UpdateData(FALSE);
}


void CEventCamTool::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEventCamTool::OnBnClickedFreecamMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 현재 액션 위치로 이동합니다
	if (m_lboxCameraAction.GetCount() < m_lboxCameraAction.GetCurSel() || m_lboxCameraAction.GetCurSel() < 0)
		return;

	const CAMERAACTION& rCamAction = m_vecCameraAction[m_lboxCameraAction.GetCurSel()];

	dynamic_cast<CFreeCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_FREE))->Set_Pos(rCamAction.vMoveTo);
	dynamic_cast<CFreeCamera*>(GET_INSTANCE(CCameraMgr)->Get_Camera(CAM_FREE))->Set_Pos(rCamAction.vRotateTo);

}
