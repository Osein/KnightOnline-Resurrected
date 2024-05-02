#if !defined(AFX_PROPERTYVIEW_H__5DBF88F9_D535_4292_A230_CB192C574AD5__INCLUDED_)
#define AFX_PROPERTYVIEW_H__5DBF88F9_D535_4292_A230_CB192C574AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "..\Common Control\PropertyList.h"

class CUIEDoc;
class CN3UIBase;
class CN3UIImage;
class CN3UIString;
class CN3UIButton;
class CN3UIProgress;
class CN3UITrackBar;
class CN3UIScrollBar;
class CN3UIArea;
class CPropertyView : public CFormView
{
protected:
	CUIEDoc* GetDocument();
	CPropertyView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPropertyView)

// Form Data
public:
	//{{AFX_DATA(CPropertyView)
	enum { IDD = IDD_PROPERTY };
	CPropertyList	m_UIStatic;
	CPropertyList	m_UIIconSlot;
	CPropertyList	m_UIArea;
	CPropertyList	m_UIEdit;
	CPropertyList	m_UITrackBar;
	CPropertyList	m_UIScrollBar;
	CPropertyList	m_UIProgress;
	CPropertyList	m_UIString;
	CPropertyList	m_UIImage;
	CPropertyList	m_UIButton;
	CPropertyList	m_UIBase;
	CPropertyList	m_UIList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void UpdateInfo();		// ��� ���� ����
	void UpdateUIBaseInfo();			// Base���� ����
	void UpdateUIImageInfo();			// Image���� ����
	void UpdateUIStringInfo();			// String���� ����
	void UpdateUIListInfo();			// List���� ����
	void UpdateUIButtonInfo();			// Button���� ����
	void UpdateUIEditInfo();			// Edit���� ����
	void UpdateUIProgressInfo();		// Progress���� ����
	void UpdateUITrackBarInfo();		// Trackbar���� ����
	void UpdateUIScrollBarInfo();		// ScrollBar���� ����
	void UpdateUIAreaInfo();			// Area���� ����
	void UpdateUIIconSlotInfo();		// IconSlot ���� ����(����Ʈ ����)
	void UpdateUIStaticInfo();			// Static ���� ����
protected:
	void Resize();			// ȭ�� ������� ���ġ
	void SetPropertyListPos(CPropertyList* pList, CPoint& ptWndPos);	// ������� Ư����ġ�� ��ġ�ϴ� �Լ�
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPropertyView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG  // debug version in UIEView.cpp
inline CUIEDoc* CPropertyView::GetDocument()
   { return (CUIEDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYVIEW_H__5DBF88F9_D535_4292_A230_CB192C574AD5__INCLUDED_)
