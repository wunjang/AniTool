
// AniToolView.cpp : CAniToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "AniTool.h"
#endif

#include "AniToolDoc.h"
#include "AniToolView.h"

#include "MainScene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_MFChWnd;

// CAniToolView

IMPLEMENT_DYNCREATE(CAniToolView, CView)

BEGIN_MESSAGE_MAP(CAniToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAniToolView ����/�Ҹ�

CAniToolView::CAniToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CAniToolView::~CAniToolView()
{
	ENGINE::Safe_Release(m_pGraphicDev);
	ENGINE::Safe_Release(m_pManagementClass);

	ENGINE::Release_Utility();
	ENGINE::Release_System();
}

BOOL CAniToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CAniToolView �׸���

void CAniToolView::OnDraw(CDC* /*pDC*/)
{
	CAniToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	_float fDeltaTime = ENGINE::Get_TimeDelta(ENGINE::TIMER::IMMEDIATE);
	if (!ENGINE::IsPermit_Call(L"Frame", fDeltaTime))
		return;
	ENGINE::Set_TimeDelta(ENGINE::TIMER::IMMEDIATE);

	ENGINE::Set_InputDev();

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
}

// CAniToolView �μ�

BOOL CAniToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CAniToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CAniToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CAniToolView ����

#ifdef _DEBUG
void CAniToolView::AssertValid() const
{
	CView::AssertValid();
}

void CAniToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAniToolDoc* CAniToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAniToolDoc)));
	return (CAniToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CAniToolView �޽��� ó����


void CAniToolView::OnInitialUpdate()
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
	ENGINE::Ready_Timer(ENGINE::TIMER::IMMEDIATE);
	ENGINE::Set_TimeDelta(ENGINE::TIMER::IMMEDIATE);
	// Manager
	ENGINE::InItManager(m_pGraphicDev);
	
	

	// Shader
	SetUp_RenderTarget(m_pGraphicDev);
}

HRESULT CAniToolView::SetUp_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev)
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
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Albedo, 0.f, 0.f, 150.f, 150.f), E_FAIL);
	//
	////Alpha
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Alpha,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A16B16G16R16F,
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Alpha, 0.f, 150.f, 150.f, 150.f), E_FAIL);
	//
	////Normal
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Normal,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A16B16G16R16F, // �Ǽ� ���� �ش��ϴ� �ȼ� ���� �����ϰڴٴ� �ǹ�, 
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Normal, 0.f, 300.f, 150.f, 150.f), E_FAIL);
	//
	////Depth
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Depth,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A32B32G32R32F, // �Ǽ� ���� �ش��ϴ� �ȼ� ���� �����ϰڴٴ� �ǹ�, 
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Depth, 0.f, 450.f, 150.f, 150.f), E_FAIL);
	//
	//
	////Shade
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Shade,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A16B16G16R16F, // �Ǽ� ���� �ش��ϴ� �ȼ� ���� �����ϰڴٴ� �ǹ�, 
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Shade, 150.f, 0.f, 150.f, 150.f), E_FAIL);
	//
	////Specular
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Specular,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A16B16G16R16F,
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Specular, 150.f, 150.f, 150.f, 150.f), E_FAIL);
	//
	////Blend
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Blend,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A16B16G16R16F,
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Blend, 150.f, 300.f, 150.f, 150.f), E_FAIL);
	//
	//
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Defferd, ENGINE::RenderTargetTag_Albedo), E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Defferd, ENGINE::RenderTargetTag_Normal), E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Defferd, ENGINE::RenderTargetTag_Depth), E_FAIL);
	//
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Blend, ENGINE::RenderTargetTag_Blend), E_FAIL);
	//
	//
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Light, ENGINE::RenderTargetTag_Shade), E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Light, ENGINE::RenderTargetTag_Specular), E_FAIL);
	//
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Alpha, ENGINE::RenderTargetTag_Alpha), E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Alpha, ENGINE::RenderTargetTag_Normal), E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_MRT(ENGINE::MRTTag_Alpha, ENGINE::RenderTargetTag_Depth), E_FAIL);

	return S_OK;
}
