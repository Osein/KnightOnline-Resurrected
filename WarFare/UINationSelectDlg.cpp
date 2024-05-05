// UINationSelectDlg.cpp: implementation of the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDef.h"
#include "UINationSelectDlg.h"
#include "GameProcNationSelect.h"
#include "../N3BASE/N3UIString.h"
#include "resource.h"

CUINationSelectDlg::CUINationSelectDlg()
{
	m_pProcNationSelectRef = NULL;
}

CUINationSelectDlg::~CUINationSelectDlg()
{
	m_pBtnKarus = nullptr;
	m_pBtnKarusNext = nullptr;
	m_pBtnKarusExit = nullptr;
	m_pBtnElmorad = nullptr;
	m_pBtnElmoradNext = nullptr;
	m_pBtnElmoradExit = nullptr;
	m_pTextNotice = nullptr;
	m_pTextNoticeName = nullptr;
}

bool CUINationSelectDlg::Load(HANDLE hFile)
{
	bool bSuccess = CN3UIBase::Load(hFile);

	m_pBtnKarus = GetChildByID("nation_karus")->GetChildByID("btn_karus_selection"); __ASSERT(m_pBtnKarus, "NULL UI Component!!");
	m_pBtnKarusNext = GetChildByID("nation_karus")->GetChildByID("btn_karus_next"); __ASSERT(m_pBtnKarusNext, "NULL UI Component!!");
	m_pBtnKarusExit = GetChildByID("nation_karus")->GetChildByID("btn_karus_close"); __ASSERT(m_pBtnKarusExit, "NULL UI Component!!");
	m_pBtnElmorad = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_selection"); __ASSERT(m_pBtnElmorad, "NULL UI Component!!");
	m_pBtnElmoradNext = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_next"); __ASSERT(m_pBtnElmoradNext, "NULL UI Component!!");
	m_pBtnElmoradExit = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_close"); __ASSERT(m_pBtnElmoradExit, "NULL UI Component!!");

	m_pTextNotice = (CN3UIString*)GetChildByID("text_notice"); __ASSERT(m_pTextNotice, "NULL UI Component!!");
	m_pTextNoticeName = (CN3UIString*)GetChildByID("text_notice_name"); __ASSERT(m_pTextNoticeName, "NULL UI Component!!");
	
	m_pTextKarusEvent = (CN3UIString*)GetChildByID("nation_karus")->GetChildByID("text_karus_notice"); __ASSERT(m_pTextNotice, "NULL UI Component!!");
	m_pTextElmoEvent = (CN3UIString*)GetChildByID("nation_elmo")->GetChildByID("text_elmo_notice"); __ASSERT(m_pTextNotice, "NULL UI Component!!");

	SetNation(e_Nation::NATION_ELMORAD);

	RECT rc = this->GetRegion();
	int iX = ((int)s_CameraData.vp.Width - (rc.right - rc.left))/2;
	int iY = ((int)s_CameraData.vp.Height - (rc.bottom - rc.top))/2;
	this->SetPos(iX, iY);

	return bSuccess;
}

void CUINationSelectDlg::SetNation(e_Nation nation)
{
	GetChildByID("nation_karus")->SetVisible(nation == e_Nation::NATION_KARUS);
	GetChildByID("nation_elmo")->SetVisible(nation == e_Nation::NATION_ELMORAD);

	std::string szNoticeName;
	::_LoadStringFromResource(
		nation == e_Nation::NATION_ELMORAD ? IDS_NATION_SELECT_ELMO_TITLE : IDS_NATION_SELECT_KARUS_TITLE, szNoticeName);
	m_pTextNoticeName->SetString(szNoticeName);

	std::string szNoticeKarus;
	::_LoadStringFromResource(IDS_NATION_SELECT_KARUS_DESC, szNoticeKarus);
	std::string szNoticeElmo;
	::_LoadStringFromResource(IDS_NATION_SELECT_ELMO_DESC, szNoticeElmo);

	m_pTextNotice->SetString(nation == e_Nation::NATION_ELMORAD ? szNoticeElmo : szNoticeKarus);
	m_pTextKarusEvent->SetString(szNoticeKarus);
	m_pTextElmoEvent->SetString(szNoticeElmo);
}

bool CUINationSelectDlg::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if(NULL == pSender) return false;

	if( dwMsg == UIMSG_BUTTON_CLICK )
	{
		if ( pSender == m_pBtnKarus )	// Karus
		{
			if(m_pProcNationSelectRef) m_pProcNationSelectRef->MsgSendNationSelect(NATION_KARUS);
		}
		else if ( pSender == m_pBtnElmorad )	// Elmorad
		{
			if(m_pProcNationSelectRef) m_pProcNationSelectRef->MsgSendNationSelect(NATION_ELMORAD);
		}
		else if (pSender == m_pBtnKarusNext)
		{
			SetNation(e_Nation::NATION_ELMORAD);
		}
		else if (pSender == m_pBtnElmoradNext)
		{
			SetNation(e_Nation::NATION_KARUS);
		}
		if (pSender == m_pBtnKarusExit || pSender == m_pBtnElmoradExit)
		{
			std::string szMsg;
			::_LoadStringFromResource(IDS_CONFIRM_EXIT_GAME, szMsg);
			e_Behavior eBehavior = ((true) ? BEHAVIOR_EXIT : BEHAVIOR_NOTHING);
			CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, eBehavior);
		}
	}

	return true;
}

