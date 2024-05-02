// Knights.h: interface for the CKnights class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNIGHTS_H__741B63A3_F081_45B0_9918_012D2E88A8BC__INCLUDED_)
#define AFX_KNIGHTS_H__741B63A3_F081_45B0_9918_012D2E88A8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "gamedefine.h"

class CUser;
class CEbenezerDlg;
class CKnights  
{
public:
	short	m_sIndex;
	BYTE    m_byFlag;			// 1 : Clan, 2 : ����
	BYTE	m_byNation;			// nation
	BYTE	m_byGrade;			// clan ��� (1 ~ 5���)
	BYTE	m_byRanking;		// clan ��� (1 ~ 5��)
	char	m_strName[MAX_ID_SIZE+1];
	short	m_sMembers;
	char	m_strChief[MAX_ID_SIZE+1];
	char	m_strViceChief_1[MAX_ID_SIZE+1];		// �δ��� 1
	char	m_strViceChief_2[MAX_ID_SIZE+1];		// �δ��� 2 (���ܿ����� �屳)
	char	m_strViceChief_3[MAX_ID_SIZE+1];		// �δ��� 3	(���ܿ����� ������� ����)
	__int64	m_nMoney;
	short	m_sDomination;
	int		m_nPoints;
	BYTE	m_Image[MAX_KNIGHTS_MARK];
	_ITEM_DATA	m_StashItem[MAX_KNIGHTS_BANK];
	_KNIGHTS_USER m_arKnightsUser[MAX_CLAN];		// Ŭ������ ����

public:
	CKnights();
	virtual ~CKnights();

	void InitializeValue();

private:
	
};

#endif // !defined(AFX_KNIGHTS_H__741B63A3_F081_45B0_9918_012D2E88A8BC__INCLUDED_)
