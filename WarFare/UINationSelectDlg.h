// UINationSelectDlg.h: interface for the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UINATIONSELECTDLG_H__AC80EE20_C90E_4B08_807A_5431F8F23BF7__INCLUDED_)
#define AFX_UINATIONSELECTDLG_H__AC80EE20_C90E_4B08_807A_5431F8F23BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../N3Base/N3UIBase.h"

class CUINationSelectDlg : public CN3UIBase  
{
public:
	CN3UIString* m_pTextNotice;
	CN3UIString* m_pTextNoticeName;
	CN3UIBase* m_pBtnKarus;
	CN3UIBase* m_pBtnKarusNext;
	CN3UIBase* m_pBtnKarusExit;
	CN3UIBase* m_pBtnElmorad;
	CN3UIBase* m_pBtnElmoradNext;
	CN3UIBase* m_pBtnElmoradExit;
	CN3UIString* m_pTextKarusEvent;
	CN3UIString* m_pTextElmoEvent;

	class CGameProcNationSelect* m_pProcNationSelectRef;

public:
	bool	Load(HANDLE hFile);
	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void	SetNation(e_Nation);

	CUINationSelectDlg();
	virtual ~CUINationSelectDlg();
};

#endif // !defined(AFX_UINATIONSELECTDLG_H__AC80EE20_C90E_4B08_807A_5431F8F23BF7__INCLUDED_)
