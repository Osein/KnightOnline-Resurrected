// NpcInfoTable.h: interface for the CNpcInfoTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCINFOTABLE_H__5F61A01D_DBD7_40CE_8728_03E662E44182__INCLUDED_)
#define AFX_NPCINFOTABLE_H__5F61A01D_DBD7_40CE_8728_03E662E44182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNpcInfoTable  
{
public:
	short	m_sSid;				// MONSTER(NPC) Serial ID
	TCHAR	m_strName[20];		// MONSTER(NPC) Name

	short	m_sMaxHP;			// �ִ� HP
	short	m_sMaxMP;			// �ִ� PP
	
	BYTE	m_byClass;			// ����迭

	int		m_iExp;				// ����ġ

	short	m_sDefense;		// ��
	BYTE	m_byAttackRange;			// �����Ÿ�

	int		m_sAI;				// �ΰ����� �ε���
	int		m_sAttackDelay;		// ���ݵ�����

	BYTE	m_bySearchRange;	// �� Ž�� ����
	int		m_sSpeed;			// �̵��ӵ�	

	int		m_sStandTime;		// ���ִ� �ð�
	BYTE	m_tNpcType;			// NPC Type
								// 0 : Monster
								// 1 : Normal NPC

	int		m_sFamilyType;		// ������̿��� �������踦 �����Ѵ�.
	BYTE	m_tItemPer;			// �������� ������ Ȯ��
	BYTE	m_tDnPer;			// ���� ������Ȯ��

public:
	void Initialize();
	CNpcInfoTable();
	virtual ~CNpcInfoTable();

};

#endif // !defined(AFX_NPCINFOTABLE_H__5F61A01D_DBD7_40CE_8728_03E662E44182__INCLUDED_)
