// SoundMgr.h: interface for the CEventMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDMGR_H__73272CD9_F059_4001_A116_871CEC8B25AE__INCLUDED_)
#define AFX_SOUNDMGR_H__73272CD9_F059_4001_A116_871CEC8B25AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\N3BASE\N3BaseFileAccess.h"
#include <list>

class CMapMng;
class CSoundCell;
class CDlgSetSound;

typedef struct __SoundInfo
{
	char	szName[256];
	DWORD	dwID;
	char	szBGM[4][256];
	float	fBGMRegenTime[4];
	char	szBGE[4][256];
	float	fBGERegenTime[4];

	__SoundInfo()
	{
		ZeroMemory(szName, 256);
		dwID = 0;

		for(int i=0;i<4;i++)
		{
			ZeroMemory(szBGM[i], 256);
			fBGMRegenTime[i] = 0.0f;

			ZeroMemory(szBGE[i], 256);
			fBGERegenTime[i] = 0.0f;
		}
	}
} SOUNDINFO, *LPSOUNDINFO;

class CSoundMgr : public CN3BaseFileAccess
{
public:
	int						m_iVersion;
	std::list<CSoundCell*>	m_pSound;
	CSoundCell*				m_pCurrSound;

	int						m_MapSize;
		
	CMapMng*				m_pRefMapMng;
	bool					m_bActive;		// �̱���� Ȱ��ȭ �Ǿ� �ִ���...1:Ȱ��ȭ, 0:��Ȱ��ȭ..
	CDlgSetSound*			m_pDlgSound;

public:
	void DelSound(CSoundCell* pEvent);
	void SetCurrSound(CSoundCell* pEvent);
	void UpdateSound();
	BOOL MouseMsgFilter(LPMSG pMsg);

	void SetActive(bool active);
	void Render();

	//////////////////////////////////////////////
	//m_pSound�� ��Ʈ�ϱ������̾�..
	void	SCSort();
	void	SCMerge(std::list<CSoundCell*>& l1, std::list<CSoundCell*>& l2);
	bool	SCComp(CSoundCell* pP1, CSoundCell* pP2);
	//
	//////////////////////////////////////////////

	virtual bool	Load(HANDLE hFile);
	virtual bool	Save(HANDLE hFile);
	void SaveGameData(HANDLE hFile);

	CSoundMgr();
	virtual ~CSoundMgr();
};

#endif // !defined(AFX_SOUNDMGR_H__73272CD9_F059_4001_A116_871CEC8B25AE__INCLUDED_)
