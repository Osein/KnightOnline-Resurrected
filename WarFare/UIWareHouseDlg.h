// UIWareHouseDlg.h: interface for the UIWareHouseDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIWAREHOUSEDLG_H__47E9608F_598A_49CF_B10E_3D95FA647768__INCLUDED_)
#define AFX_UIWAREHOUSEDLG_H__47E9608F_598A_49CF_B10E_3D95FA647768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\N3BASE\N3UIBase.h"
#include "..\N3BASE\N3UIArea.h"
#include "..\N3Base\N3UIString.h"
#include "..\N3Base\N3UIImage.h"
#include "..\N3Base\N3UIButton.h"
#include "GameProcedure.h"

#include "GameDef.h"
#include "N3UIWndBase.h"

class CUIWareHouseDlg : public CN3UIWndBase  
{
	friend class CUIInventory;

public:
	__IconItemSkill*		m_pMyWare[MAX_ITEM_WARE_PAGE][MAX_ITEM_TRADE];
	__IconItemSkill*		m_pMyWareInv[MAX_ITEM_INVENTORY];
	CN3UIString*			m_pStrMyGold;
	CN3UIString*			m_pStrWareGold;

	CN3UIButton*			m_pBtnGold;
	CN3UIButton*			m_pBtnGoldWareHouse;
	CN3UIButton*			m_pBtnClose;
	CN3UIButton*			m_pBtnPageUp;
	CN3UIButton*			m_pBtnPageDown;

	bool					m_bSendedItemGold;
	int						m_iGoldOffsetBackup;

	int						m_iCurPage;
	CUIImageTooltipDlg*		m_pUITooltipDlg;

protected:
	int					GetItemiOrder(__IconItemSkill* spItem, e_UIWND_DISTRICT eWndDist);
	RECT				GetSampleRect();

	e_UIWND_DISTRICT	GetWndDistrict(__IconItemSkill* spItem);

public:
	CUIWareHouseDlg();
	virtual ~CUIWareHouseDlg();
	void				Release();

	void				SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void				SetVisible(bool bVisible);
	bool				Load(HANDLE hFile);
	bool				OnKeyPress(int iKey);

	virtual DWORD		MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
	virtual bool		ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void				Render();
	void				LeaveWareHouseState();
	void				EnterWareHouseStateStart(int iWareGold);
	void				AddItemInWare(int iItem, int iDurability, int iCount, int iIndex);
	void				EnterWareHouseStateEnd();

	void				InitIconWnd(e_UIWND eWnd);
	void				InitIconUpdate();

	__IconItemSkill*	GetHighlightIconItem(CN3UIIcon* pUIIcon);

	void				IconRestore();
	
	bool				ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur);

	void				CancelIconDrop(__IconItemSkill* spItem);
	void				AcceptIconDrop(__IconItemSkill* spItem);

	void				SendToServerToWareMsg(int iItemID, BYTE page, BYTE startpos, BYTE pos, int iCount);
	void				SendToServerFromWareMsg(int iItemID, BYTE page, BYTE startpos, BYTE pos, int iCount);
	void				SendToServerWareToWareMsg(int iItemID, BYTE page, BYTE startpos, BYTE destpos);
	void				SendToServerInvToInvMsg(int iItemID, BYTE page, BYTE startpos, BYTE destpos);
	void				ReceiveResultToWareMsg(BYTE bResult);
	void				ReceiveResultFromWareMsg(BYTE bResult);
	void				ReceiveResultWareToWareMsg(BYTE bResult);
	void				ReceiveResultInvToInvMsg(BYTE bResult);
	void				ReceiveResultGoldToWareFail();
	void				ReceiveResultGoldFromWareFail();

	void				ItemCountOK();
	void				ItemCountCancel();

	void				GoldCountToWareOK();
	void				GoldCountFromWareOK();
	void				GoldCountToWareCancel();
	void				GoldCountFromWareCancel();

	void				ItemMoveFromInvToThis();
	void				ItemMoveFromThisToInv();
};

#endif // !defined(AFX_UIWAREHOUSEDLG_H__47E9608F_598A_49CF_B10E_3D95FA647768__INCLUDED_)
