// UIManager.h: interface for the CUIManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIMANAGER_H__1EED48B7_90A3_4242_99BD_ABC12040CDDE__INCLUDED_)
#define AFX_UIMANAGER_H__1EED48B7_90A3_4242_99BD_ABC12040CDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\N3BASE\N3UIBase.h"

struct __IconItemSkill;

class CUIManager : public CN3UIBase  
{
protected:
	struct __RenderStateForUI
	{
		DWORD dwZEnable, dwAlphaBlend, dwSrcBlend, dwDestBlend, dwFog;
		DWORD dwMagFilter, dwMinFilter, dwMipFilter;
#ifdef _DEBUG
		BOOL	bSet;	// 이미 값이 3D device의 값으로 세팅이 되었는지 판단.
#endif
	};
	
	static __RenderStateForUI s_sRSFU;	// RenderStateForUI
	DWORD		m_dwMouseFlagsCur;
	bool		m_bEnableOperation;		// UI 조작이 가능한 상태인가?
	CN3UIBase*	m_pUIFocused;

public:
	bool		m_bDoneSomething;		// UI 에서 조작을 했다...

public:
	void UserMoveHideUIs();
	CN3UIBase* GetEnableFocusTopUI(bool bVisible);
	static void	RenderStateRestore();
	static void	RenderStateSet();

	void	EnableOperationSet(bool bEnable) { m_bEnableOperation = bEnable; } // UI 조작 가능/불가능..
	bool	EnableOperation() { return m_bEnableOperation; } // UI 조작이 가능한 상태인가?

	void		Release();
	void		Tick();
	DWORD		MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
	DWORD		GetCurMouseFlags() { return m_dwMouseFlagsCur; }
	CN3UIBase*	GetFocusedUI() { return m_pUIFocused; } // 찍은 UI 가져오기..
	void		SetFocusedUI(CN3UIBase* pUI); // 찍은 UI 가져오기..
	CN3UIBase*	GetTopUI(bool bVisible); // 최상위에 있는 UI 가져오기.
	void		SetVisibleFocusedUI(CN3UIBase* pUI);
	CN3UIBase*	ReFocusUI();
	void		Render();
	void		ReorderChildList();

	bool		BroadcastIconDropMsg(__IconItemSkill* spItem);

	CUIManager();
	virtual ~CUIManager();
};

#endif // !defined(AFX_UIMANAGER_H__1EED48B7_90A3_4242_99BD_ABC12040CDDE__INCLUDED_)
