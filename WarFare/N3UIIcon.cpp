// N3UIIcon.cpp: implementation of the CN3UIIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "N3UIIcon.h"
#include "N3UIWndBase.h"
#include "GameProcedure.h"
#include "UIImageTooltipDlg.h"

#ifdef _N3GAME
#include "..\Warfare\N3UIWndBase.h"
#endif

CN3UIIcon::CN3UIIcon()
{
	CN3UIImage::CN3UIImage();
	m_eType = UI_TYPE_ICON;
}

CN3UIIcon::~CN3UIIcon()
{
}

DWORD CN3UIIcon::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;

	if (!m_bVisible) return dwRet;
	if ( (m_pParent->GetState() == UI_STATE_COMMON_NONE) || (m_pParent->GetState() == UI_STATE_ICON_MOVING) ) 
		SetStyle(GetStyle() & (~UISTYLE_ICON_HIGHLIGHT));	

#ifdef _N3GAME
	if ( CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer ) return dwRet;
#endif

	RECT rect = GetRegion();
	if ( ::PtInRect(&rect, ptCur) && (m_pParent->GetState() == UI_STATE_COMMON_NONE) )
		SetStyle(GetStyle() | UISTYLE_ICON_HIGHLIGHT);	

	rect = GetMoveRect();

	if(!::PtInRect(&rect, ptCur))		// 영역 밖이면
	{
		dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
		return dwRet;
	}

	if((dwFlags & UI_MOUSE_LBCLICK) && !(dwFlags & UI_MOUSE_RBDOWN))  // 왼쪽버튼 눌르는 순간
	{
		m_pParent->SetState(UI_STATE_ICON_MOVING);
		m_pParent->ReceiveMessage(this, UIMSG_ICON_DOWN_FIRST); // 부모에게 버튼 클릭 통지..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if((dwFlags & UI_MOUSE_LBCLICKED) && !(dwFlags & UI_MOUSE_RBDOWN)) // 왼쪽버튼을 떼는 순간
	{
		if(m_pParent && m_pParent->GetState() == UI_STATE_ICON_MOVING) // 이전 상태가 버튼을 Down 상태이면
		{
			m_pParent->SetState(UI_STATE_COMMON_NONE);
			m_pParent->ReceiveMessage(this, UIMSG_ICON_UP); // 부모에게 버튼 클릭 통지..
			dwRet |= UI_MOUSEPROC_DONESOMETHING;
			return dwRet;
		}
	}

	if ((dwFlags & UI_MOUSE_RBCLICK)  && !(dwFlags & UI_MOUSE_LBDOWN))	// 오른쪽
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_RDOWN_FIRST); // 부모에게 버튼 클릭 통지..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if((dwFlags & UI_MOUSE_RBCLICKED)  && !(dwFlags & UI_MOUSE_LBDOWN))// 오른쪽 버튼을 떼는 순간
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_RUP); // 부모에게 버튼 클릭 통지..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if (dwFlags & UI_MOUSE_LBDOWN)	// 왼쪽
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_DOWN); // 부모에게 버튼 클릭 통지..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}
/*
	else
	{
		if(m_pParent && m_pParent->GetState() == UI_STATE_ICON_MOVING) // 이전 상태가 버튼을 Down 상태이면
		{
			m_pParent->ReceiveMessage(this, UIMSG_ICON_UP); // 부모에게 버튼 클릭 통지..
			dwRet |= UI_MOUSEPROC_DONESOMETHING;
			return dwRet;
		}
	}
*/

	if (dwFlags & UI_MOUSE_LBDBLCLK)	// 왼쪽 더블 클릭
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_DBLCLK); // 부모에게 버튼 클릭 통지..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if (dwFlags & UI_MOUSE_RBDBLCLK)	// 오른쪽 더블 클릭
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_RDBLCLK); // 부모에게 버튼 클릭 통지..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

void CN3UIIcon::Render()
{
	if (!IsVisible()) return;

	CN3UIWndBase::m_pSelectionImage->SetVisible(false);

/*	if ( m_dwStyle & UISTYLE_ICON_HIGHLIGHT )
	{		
		CN3UIWndBase::m_pSelectionImage->SetVisible(true);
		CN3UIWndBase::m_pSelectionImage->SetRegion(GetRegion());
		CN3UIWndBase::m_pSelectionImage->Render();
		CN3UIWndBase::m_pSelectionImage->SetVisible(false);
	}*/

	CN3UIImage::Render();

	if (m_dwStyle & UISTYLE_DISABLE_SKILL)
	{
		CN3UIWndBase::m_pSelectionImage->SetVisible(true);
		CN3UIWndBase::m_pSelectionImage->SetRegion(GetRegion());
		m_dc = CN3UIWndBase::m_pSelectionImage->GetColor();
		CN3UIWndBase::m_pSelectionImage->SetColor(D3DCOLOR_RGBA(40,40, 40, 160));
		CN3UIWndBase::m_pSelectionImage->RenderIconWrapper();
		CN3UIWndBase::m_pSelectionImage->SetColor(m_dc);
		CN3UIWndBase::m_pSelectionImage->SetVisible(false);
	}

	if( m_dwStyle & UISTYLE_DURABILITY_EXHAUST )
	{
		CN3UIWndBase::m_pSelectionImage->SetVisible(true);
		CN3UIWndBase::m_pSelectionImage->SetRegion(GetRegion());
		m_dc = CN3UIWndBase::m_pSelectionImage->GetColor();
		CN3UIWndBase::m_pSelectionImage->SetColor(D3DCOLOR_RGBA(200, 20, 20, 100));
		CN3UIWndBase::m_pSelectionImage->RenderIconWrapper();
		CN3UIWndBase::m_pSelectionImage->SetColor(m_dc);
		CN3UIWndBase::m_pSelectionImage->SetVisible(false);
	}
}





