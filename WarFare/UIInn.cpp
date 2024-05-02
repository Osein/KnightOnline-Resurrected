// UINPCEvent.cpp: implementation of the UINPCEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "UIInn.h"
#include "UICreateClanName.h"

#include "GameProcMain.h"
#include "GameProcedure.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "PlayerMySelf.h"
#include "UITradeBBSSelector.h"
#include "UIManager.h"

CUIInn::CUIInn()
{
}

CUIInn::~CUIInn()
{
}

bool CUIInn::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)					
	{
		if(pSender->m_szID == "btn_warehouse")	//창고이용..
		{
			MsgSend_OpenWareHouse();
			SetVisible(false);
			return true;
		}

		if(pSender->m_szID == "btn_makeclan")	//clan 생성..
		{
			__InfoPlayerBase* pInfo = &(CGameProcedure::s_pPlayer->m_InfoBase);
			__InfoPlayerMySelf* pInfoExt = &(CGameProcedure::s_pPlayer->m_InfoExt);

			if(pInfo->iLevel < CLAN_LEVEL_LIMIT)
			{
				Message(IDS_CLAN_DENY_LOWLEVEL);
				SetVisible(false);
				return true;
			}
			if(pInfoExt->iGold < CLAN_COST)
			{
				Message(IDS_CLAN_DENY_LOWGOLD);
				SetVisible(false);
				return true;
			}
			if(pInfoExt->iKnightsID!=0)
			{
				Message(IDS_CLAN_DENY_ALREADYJOINED);
				SetVisible(false);
				return true;
			}

			CGameProcedure::s_pProcMain->m_pUICreateClanName->Open(IDS_CLAN_INPUT_NAME);
			SetVisible(false);
			return true;
		}
		else if(pSender->m_szID == "btn_sale")
		{
			CGameProcedure::s_pProcMain->m_pUITradeBBSSelector->MsgSend_OpenTradeSellBBS();
			SetVisible(false);
			return true;
		}

	}
	return true;
}

void CUIInn::Message(int iMessageID)
{
	std::string szMsg;
	::_LoadStringFromResource(iMessageID, szMsg);

	if(iMessageID == IDS_CLAN_DENY_LOWGOLD)
	{
		char szTmp[256];
		sprintf(szTmp, szMsg.c_str(), CLAN_COST);
		szMsg = szTmp;
	}

	CGameProcedure::s_pProcMain->MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_NOTHING);	
}

void CUIInn::MsgSend_OpenWareHouse()
{
	BYTE byBuff[2];		// 패킷 버퍼..
	int iOffset=0;		// 패킷 오프셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WAREHOUSE);		
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_WARE_OPEN);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIInn::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

bool CUIInn::OnKeyPress(int iChar)
{
	switch(iChar)
	{
	case DIK_ESCAPE:
		SetVisible(false);
		return true;
	}

	return CN3UIBase::OnKeyPress(iChar);
}
