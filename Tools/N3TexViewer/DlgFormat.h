#if !defined(AFX_DLGFORMAT_H__94A51D98_DD7E_4FF1_ABD5_940462C2355F__INCLUDED_)
#define AFX_DLGFORMAT_H__94A51D98_DD7E_4FF1_ABD5_940462C2355F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat dialog
#include <D3D8Types.h>

class CDlgFormat : public CDialog
{
// Construction
public:
	D3DFORMAT	m_Fmt;
	int			m_nWidth;
	int			m_nHeight;
	BOOL		m_bMipMap;

	CDlgFormat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFormat)
	enum { IDD = IDD_FORMAT };
	CComboBox	m_CBFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormat)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFORMAT_H__94A51D98_DD7E_4FF1_ABD5_940462C2355F__INCLUDED_)
