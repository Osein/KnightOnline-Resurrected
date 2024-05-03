// VersionManagerDlg.h : header file
//

#if !defined(AFX_VERSIONMANAGERDLG_H__1563BFF5_5A54_47E5_A62C_7C123D067588__INCLUDED_)
#define AFX_VERSIONMANAGERDLG_H__1563BFF5_5A54_47E5_A62C_7C123D067588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "Iocport.h"
#include "DBProcess.h"
#include "STLMap.h"
#include <vector>
#include <string>
/////////////////////////////////////////////////////////////////////////////
// CVersionManagerDlg dialog

typedef CSTLMap <string, _VERSION_INFO>	VersionInfoList;
typedef std::vector <_SERVER_INFO*>	ServerInfoList;

class CVersionManagerDlg : public CDialog
{
// Construction
public:
	BOOL GetInfoFromIni();
	
	CVersionManagerDlg(CWnd* pParent = NULL);	// standard constructor

	static CIOCPort	m_Iocport;

	char	m_strFtpUrl[256];
	char	m_strFilePath[256];

	int		m_nLastVersion;

	char	m_ODBCName[32];
	char	m_ODBCLogin[32];
	char	m_ODBCPwd[32];
	char	m_TableName[32];

	VersionInfoList		m_VersionList;
	ServerInfoList		m_ServerList;
	int		m_nServerCount;

	CDBProcess	m_DBProcess;

// Dialog Data
	//{{AFX_DATA(CVersionManagerDlg)
	enum { IDD = IDD_VERSIONMANAGER_DIALOG };
	CListBox	m_OutputList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVersionManagerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVersionManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnVersionSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERSIONMANAGERDLG_H__1563BFF5_5A54_47E5_A62C_7C123D067588__INCLUDED_)
