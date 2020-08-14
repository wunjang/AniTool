
// AniToolView.h : CAniToolView 클래스의 인터페이스
//

#pragma once

class CAniToolDoc;

BEGIN(ENGINE)

class CGraphicDev;
class CManagement;

END

class CAniToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CAniToolView();
	DECLARE_DYNCREATE(CAniToolView)

// 특성입니다.
public:
	CAniToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CAniToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	ENGINE::CGraphicDev*	m_pDeviceClass;
	ENGINE::CManagement*	m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	HRESULT SetUp_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev);
};

#ifndef _DEBUG  // AniToolView.cpp의 디버그 버전
inline CAniToolDoc* CAniToolView::GetDocument() const
   { return reinterpret_cast<CAniToolDoc*>(m_pDocument); }
#endif

