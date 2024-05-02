// EventMgr.h: interface for the CEventMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTMGR_H__73272CD9_F059_4001_A116_871CEC8B25AE__INCLUDED_)
#define AFX_EVENTMGR_H__73272CD9_F059_4001_A116_871CEC8B25AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\N3BASE\N3Base.h"
#include <list>

class CMapMng;
class CEventCell;
class CDlgEditEvent;

class CEventMgr : public CN3Base
{
public:
	std::list<CEventCell*>	m_pEvents;
	CEventCell*				m_pCurrEvent;
	int						m_MapSize;
	short**					m_ppEvent;
	CMapMng*				m_pRefMapMng;
	bool					m_bActive;					// �̱���� Ȱ��ȭ �Ǿ� �ִ���...1:Ȱ��ȭ, 0:��Ȱ��ȭ..
	CDlgEditEvent*			m_pDlgEventList;

public:
	bool MakeGameFile(HANDLE hFile, int iSize);
	bool MakeGameFile(char* szEventName, int iSize);
	void LoadFromFile(const char* RealFileName);
	void SaveToFile(const char* RealFileName);
	void SaveInfoTextFile(char* szEvent);
	void MakeEventArray();
	void DelEvent(CEventCell* pEvent);
	void SetCurrEvent(CEventCell* pEvent);
	void UpdateEvent();
	BOOL MouseMsgFilter(LPMSG pMsg);
	void SetActive(bool active);
	void Render();

	virtual bool	Load(HANDLE hFile);
	virtual bool	Save(HANDLE hFile);

	CEventMgr();
	virtual ~CEventMgr();
};

#endif // !defined(AFX_EVENTMGR_H__73272CD9_F059_4001_A116_871CEC8B25AE__INCLUDED_)
