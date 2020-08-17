
// CamToolView.h : CCamToolView Ŭ������ �������̽�
//

#pragma once

class CCamToolDoc;

BEGIN(ENGINE)

class CGraphicDev;
class CManagement;

END

class CCamToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CCamToolView();
	DECLARE_DYNCREATE(CCamToolView)

// Ư���Դϴ�.
public:
	CCamToolDoc* GetDocument() const;

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
	virtual ~CCamToolView();
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

	void Get_KeyInput();
};

#ifndef _DEBUG  // CamToolView.cpp�� ����� ����
inline CCamToolDoc* CCamToolView::GetDocument() const
   { return reinterpret_cast<CCamToolDoc*>(m_pDocument); }
#endif

