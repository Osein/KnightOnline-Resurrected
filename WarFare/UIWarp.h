// UIWarp.h: interface for the UIWarp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIWarp_H__27F6610C_5D61_4A22_97F2_93211B77BF9C__INCLUDED_)
#define AFX_UIWarp_H__27F6610C_5D61_4A22_97F2_93211B77BF9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>
#include "../N3Base/N3UIBase.h"

struct __WarpInfo
{
	int iID;
	std::string szName;
	std::string szAgreement;
	int iZone;
	int iMaxUser;
	int iGold;
	__Vector3 vPos;
};

typedef std::list<__WarpInfo>::iterator it_WI;

class CUIWarp : public CN3UIBase  
{
protected:
	class CN3UIButton*		m_pBtn_Ok;
	class CN3UIButton*		m_pBtn_Cancel;
	
	class CN3UIList*		m_pList_Infos;
	class CN3UIString*		m_pText_Agreement;

	std::list<__WarpInfo> m_ListInfos;

public:
	bool OnKeyPress(int iKey);
	void SetVisible(bool bVisible);
	void Reset();
	CUIWarp();
	virtual ~CUIWarp();

	void	InfoAdd(const __WarpInfo& ZW);
	int		InfoGetCur(__WarpInfo& ZW);
	void	UpdateList();
	void	UpdateAgreement();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
};

#endif // !defined(AFX_UIWarp_H__27F6610C_5D61_4A22_97F2_93211B77BF9C__INCLUDED_)
