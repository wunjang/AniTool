
// CamToolView.cpp : CCamToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "CamTool.h"
#endif

#include "CamToolDoc.h"
#include "CamToolView.h"
#include "EventCamTool.h"

#include "MainScene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_MFChWnd;

// CCamToolView

IMPLEMENT_DYNCREATE(CCamToolView, CView)

BEGIN_MESSAGE_MAP(CCamToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CCamToolView ����/�Ҹ�

CCamToolView::CCamToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CCamToolView::~CCamToolView()
{
	ENGINE::Safe_Release(m_pGraphicDev);
	ENGINE::Safe_Release(m_pManagementClass);

	GET_INSTANCE(CCameraMgr)->DestroyInstance();

	ENGINE::Release_Utility();
	ENGINE::Release_System();

	int i = 0;
}

BOOL CCamToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CCamToolView �׸���

void CCamToolView::OnDraw(CDC* /*pDC*/)
{
	CCamToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	_float fDeltaTime = ENGINE::Get_TimeDelta(L"Timer_FPS");
	if (!ENGINE::IsPermit_Call(L"Frame", fDeltaTime))
		return;
	ENGINE::Set_TimeDelta(L"Timer_FPS");

	ENGINE::Set_InputDev();

	Get_KeyInput();

	// Update
	m_pManagementClass->Update_Scene(fDeltaTime);

	// LateUpdate
	m_pManagementClass->LateUpdate_Scene(fDeltaTime);

	// Render
	ENGINE::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	//m_pManagementClass->Render_Scene(m_pGraphicDev, fDeltaTime);
	ENGINE::Get_Renderer()->Render_RenderTarget(m_pGraphicDev, fDeltaTime);


	ENGINE::Render_FPS(L"Frame");


	ENGINE::Render_End();

	// KeyInput
	if (ENGINE::KeyDown(DIK_F3))
		GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_FREE);
	if (ENGINE::KeyDown(DIK_F4))
		GET_INSTANCE(CCameraMgr)->Set_CurCamera(CAM_EVENT);
}

// CCamToolView �μ�

BOOL CCamToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CCamToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CCamToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CCamToolView ����

#ifdef _DEBUG
void CCamToolView::AssertValid() const
{
	CView::AssertValid();
}

void CCamToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCamToolDoc* CCamToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCamToolDoc)));
	return (CCamToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CCamToolView �޽��� ó����


void CCamToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_MFChWnd = m_hWnd;
	CRect rt;
	GetWindowRect(&rt);

	// GraphicDev
	ENGINE::Ready_GraphicDev(g_MFChWnd, ENGINE::MODE_WIN, rt.Width(), rt.Height(), &m_pDeviceClass);
	m_pGraphicDev = m_pDeviceClass->GetDevice();
	m_pGraphicDev->AddRef();

	// Component
	ENGINE::Reserve_Component_Manager(1);

	// Management
	ENGINE::CScene*	pScene = CMainScene::Create(m_pGraphicDev);
	ENGINE::Create_Management(m_pGraphicDev, &m_pManagementClass);
	m_pManagementClass->AddRef();
	ENGINE::SetUp_Scene(pScene);

	// Font
	ENGINE::Ready_Font(m_pGraphicDev, L"Font_Default", L"���� ���", 15, 20, FW_NORMAL);

	// Input
	ENGINE::Ready_InputDev(AfxGetInstanceHandle(), g_MFChWnd);

	// Frame
	ENGINE::Ready_Frame(L"Frame", 144.f);

	// Timer
	ENGINE::Ready_Timer(L"Timer_FPS");
	ENGINE::Set_TimeDelta(L"Timer_FPS");
	// Manager
	ENGINE::InItManager(m_pGraphicDev);
	
	

	// Shader
	SetUp_RenderTarget(m_pGraphicDev);
}

HRESULT CCamToolView::SetUp_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	// For.Component_Shader_Rect
	if (FAILED(ENGINE::Add_Prototype(0, L"Component_Shader_Rect", ENGINE::CShader::Create(m_pGraphicDev, L"../ShaderFiles/Shader_Rect.fx"))))
		return E_FAIL;


	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	//Albedo
	FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
		ENGINE::RenderTargetTag_Albedo,
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F, // �Ǽ� ���� �ش��ϴ� �ȼ� ���� �����ϰڴٴ� �ǹ�, 
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
		E_FAIL);
	
	return S_OK;
}

void CCamToolView::Get_KeyInput()
{
	if (ENGINE::KeyDown(DIK_O))
	{
		// ���� Ÿ�� �ű��
		_vec3 vPick;
		if (GET_INSTANCE(CCameraMgr)->Picking(m_pGraphicDev, &vPick))
		{
			ENGINE::CTransform*	pTransform = dynamic_cast<ENGINE::CTransform*>(ENGINE::Get_Component(ENGINE::LAYER_GAMEOBJECT, ENGINE::PLAYER, ENGINE::COMPONENT::TAG_TRANSFORM, ENGINE::COMPONENT::ID_DYNAMIC));
			NULL_CHECK(pTransform);
			pTransform->Set_Pos(&vPick);
			CEventCamTool::Get_Dialog()->Set_TargetPosData(vPick);
		}
	}

	if (ENGINE::KeyDown(DIK_P))
	{
		// ������ �׼� ������ �ű��
		_vec3 vPick;
		if (GET_INSTANCE(CCameraMgr)->Picking(m_pGraphicDev, &vPick))
		{
			CEventCamTool::Get_Dialog()->Set_CurAction(vPick);
		}
	}
}