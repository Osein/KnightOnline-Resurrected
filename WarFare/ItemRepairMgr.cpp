// ItemRepairMgr.cpp: implementation of the CItemRepairMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ItemRepairMgr.h"

#include "PacketDef.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "UIInventory.h"
#include "GameProcMain.h"
#include "PlayerMySelf.h"

#include "UIRepairTooltipDlg.h"

#include "../N3Base/N3UIString.h"

CItemRepairMgr::CItemRepairMgr()
{
	m_pspItemBack = NULL;	
}

CItemRepairMgr::~CItemRepairMgr()
{

}

void CItemRepairMgr::Tick()
{
	CUIInventory *pInv	= CGameProcedure::s_pProcMain->m_pUIInventory;
	if (!pInv) return;
	if (!pInv->IsVisible())	return;
	if (pInv->GetInvState() != INV_STATE_REPAIR) return;
	if (CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) return;

	CUIRepairTooltipDlg *pDlg = CGameProcedure::s_pProcMain->m_pUIRepairTooltip;
	if (pDlg)	pDlg->m_bBRender = false;

	POINT ptCur			= CGameProcedure::s_pLocalInput->MouseGetPos();

	// 위치를 구해서 
	int i;	int iArm = 0x00; int iOrder = -1; __IconItemSkill* spItem = NULL;
	for (i = 0; i < ITEM_SLOT_COUNT; i++)
	{
		if (spItem) break;
		if (pInv->m_pMySlot[i])
		{
			if (pInv->m_pMySlot[i]->pUIIcon->IsIn(ptCur.x, ptCur.y))
			{
				iArm = 0x01;
				spItem = pInv->m_pMySlot[i];
				iOrder = i;
			}
		}
	}

	if (!spItem)
	{
		for (i = 0; i < MAX_ITEM_INVENTORY; i++)
		{
			if (spItem) break;
			if (pInv->m_pMyInvWnd[i])
			{
				if (pInv->m_pMyInvWnd[i]->pUIIcon->IsIn(ptCur.x, ptCur.y))
				{
					iArm = 0x02;
					spItem = pInv->m_pMyInvWnd[i];
					iOrder = i;
				}
			}
		}
	}

	// 아이콘 위에 있으면..  
	int iRepairGold = 0;
	if (spItem)
	{
		iRepairGold = CalcRepairGold(spItem);

		// 수리 가격 툴팁 표시..
		if (pDlg)
		{
			pDlg->m_bBRender		= true;
			pDlg->m_iBxpos			= ptCur.x;
			pDlg->m_iBypos			= ptCur.y;
			pDlg->m_pBspItem		= spItem;
			pDlg->m_iBRequiredGold	= iRepairGold;
		}

		// 내가 가진 돈 보다 수리 비용이 비싸면.. 
		if (iRepairGold > s_pPlayer->m_InfoExt.iGold)
		{
			// 빨갛게 표시.. 
			if (pDlg)
				pDlg->m_bBHaveEnough = false;
		}
		else
		{
			//아이면 원래 색깔..
			if (pDlg)
				pDlg->m_bBHaveEnough = true;
		}
	}

	DWORD dwMouseFlags	= CGameProcedure::s_pLocalInput->MouseGetFlag();	// 마우스 버튼 플래그 - LocalInput.h 참조
	if (dwMouseFlags & MOUSE_LBCLICK)		// 왼쪽 버튼을 누르면..
	{
		m_pspItemBack	= spItem;
		m_iArm			= iArm;
		m_iiOrder		= iOrder;
	}
	else if(dwMouseFlags & MOUSE_LBCLICKED)
	{
		if (m_pspItemBack && spItem && (m_pspItemBack == spItem) )
		{
			// Send To Server..
			if (iRepairGold > 0)										// 수리 가격이 있으면..
			{
				// 내가 가진 돈 보다 수리 비용이 비싸면.. 
				if (iRepairGold > s_pPlayer->m_InfoExt.iGold)
				{
					// 서버에게 보내지 않고 메시지 표시.. 
					std::string szMsg; ::_LoadStringFromResource(IDS_REPAIR_LACK_GOLD, szMsg);
					CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff00ff);
				}
				else
				{
					BYTE byBuff[8];															// 패킷 버퍼..
					int iOffset=0;															// 패킷 오프셋..

					CAPISocket::MP_AddByte(byBuff, iOffset,  N3_ITEM_REPAIR_REQUEST);			// 게임 스타트 패킷 커멘드..
					CAPISocket::MP_AddByte(byBuff, iOffset,  iArm);							// 아이디 길이 패킷에 넣기..
					CAPISocket::MP_AddByte(byBuff, iOffset,  iOrder);							// 아이디 길이 패킷에 넣기..
					CAPISocket::MP_AddDword(byBuff, iOffset, spItem->pItemBasic->dwID+spItem->pItemExt->dwID);	// 아이디 문자열 패킷에 넣기..

					CGameProcedure::s_pSocket->Send(byBuff, iOffset);	

					// 응답을 기다림..
					CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;

					// Change To Cursor..
					CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorNowRepair, true);
				}
			}
		}
	}
}

void CItemRepairMgr::ReceiveResultFromServer(int iResult, int iUserGold)
{
	CUIRepairTooltipDlg *pDlg = CGameProcedure::s_pProcMain->m_pUIRepairTooltip;
	CUIInventory *pInv	= CGameProcedure::s_pProcMain->m_pUIInventory;
	if (!pInv) return;
	if (!m_pspItemBack) return;

	// 성공이면 npc영역의 Durability를 최대값으로..
	if(iResult == 0x01)
	{
		m_pspItemBack->iDurability = m_pspItemBack->pItemBasic->siMaxDurability+m_pspItemBack->pItemExt->siMaxDurability;

		switch (m_iArm)
		{
			case 0x01: // 장착하고 있는 아이템
				pInv->m_pMySlot[m_iiOrder] = m_pspItemBack;
				s_pPlayer->DurabilitySet((e_ItemSlot)m_iiOrder, m_pspItemBack->iDurability); // 내구력을 복구 해준다..
				break;

			case 0x02: // 인벤토리에 있는 아이템..
				pInv->m_pMyInvWnd[m_iiOrder] = m_pspItemBack;
				break;
		}

		// 아이콘 상태가 UISTYLE_DURABILITY_EXHAUST 이면..
		m_pspItemBack->pUIIcon->SetStyle(m_pspItemBack->pUIIcon->GetStyle() & (~UISTYLE_DURABILITY_EXHAUST));
		
		if (pDlg)	pDlg->m_iBRequiredGold	= 0;
		pInv->PlayRepairSound();
	}

	// 돈 업데이트..
	UpdateUserTotalGold(iUserGold);

	// 응답 기다림 해제..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;

	// Change To Cursor..
	CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorPreRepair, true);
}

void CItemRepairMgr::UpdateUserTotalGold(int iGold)
{
	char szGold[32];
	CN3UIString* pStatic = NULL;

	// 돈 업데이트..
	s_pPlayer->m_InfoExt.iGold = iGold;
	sprintf(szGold, "%d", iGold);
	pStatic = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStatic, "NULL UI Component!!");
	if(pStatic)
	{
		pStatic->SetString(szGold);
	}
}

int CItemRepairMgr::CalcRepairGold(__IconItemSkill* spItem)
{
	int iMaxDurability = spItem->pItemBasic->siMaxDurability + spItem->pItemExt->siMaxDurability;
	if (iMaxDurability <= 0) return 0;
	float fAllPrice = (float)spItem->pItemBasic->iPrice*(float)spItem->pItemExt->siPriceMultiply;
	float fTemp = ((fAllPrice-10.0f)/(10000.0f))+pow(fAllPrice, 0.75f);
	float fValue = fTemp*( (float)(iMaxDurability - spItem->iDurability) / (float)(iMaxDurability) );

	return (int)fValue;
}





