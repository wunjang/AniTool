
// AniToolView.cpp : CAniToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAniToolView 생성/소멸

CAniToolView::CAniToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CAniToolView 그리기

void CAniToolView::OnDraw(CDC* /*pDC*/)
{
	CAniToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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

// CAniToolView 인쇄

BOOL CAniToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CAniToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAniToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CAniToolView 진단

#ifdef _DEBUG
void CAniToolView::AssertValid() const
{
	CView::AssertValid();
}

void CAniToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAniToolDoc* CAniToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAniToolDoc)));
	return (CAniToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CAniToolView 메시지 처리기


void CAniToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	ENGINE::Ready_Font(m_pGraphicDev, L"Font_Default", L"맑은 고딕", 15, 20, FW_NORMAL);

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
		D3DFMT_A16B16G16R16F, // 실수 값에 해당하는 픽셀 값을 저장하겠다는 의미, 
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
	//	D3DFMT_A16B16G16R16F, // 실수 값에 해당하는 픽셀 값을 저장하겠다는 의미, 
	//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f)),
	//	E_FAIL);
	//FAILED_CHECK_RETURN(ENGINE::Ready_DebugBuffer(ENGINE::RenderTargetTag_Normal, 0.f, 300.f, 150.f, 150.f), E_FAIL);
	//
	////Depth
	//FAILED_CHECK_RETURN(ENGINE::Ready_RenderTarget(pGraphicDev,
	//	ENGINE::RenderTargetTag_Depth,
	//	ViewPort.Width,
	//	ViewPort.Height,
	//	D3DFMT_A32B32G32R32F, // 실수 값에 해당하는 픽셀 값을 저장하겠다는 의미, 
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
	//	D3DFMT_A16B16G16R16F, // 실수 값에 해당하는 픽셀 값을 저장하겠다는 의미, 
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
