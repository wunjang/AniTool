
// AniToolView.h : CAniToolView Ŭ������ �������̽�
//

#pragma once

class CAniToolDoc;

BEGIN(ENGINE)

class CGraphicDev;
class CManagement;

END

class CAniToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CAniToolView();
	DECLARE_DYNCREATE(CAniToolView)

// Ư���Դϴ�.
public:
	CAniToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
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

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	HRESULT SetUp_RenderTarget(LPDIRECT3DDEVICE9 & pGraphicDev);
};

#ifndef _DEBUG  // AniToolView.cpp�� ����� ����
inline CAniToolDoc* CAniToolView::GetDocument() const
   { return reinterpret_cast<CAniToolDoc*>(m_pDocument); }
#endif

