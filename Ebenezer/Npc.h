// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__1DE71CDD_4040_4479_828D_E8EE07BD7A67__INCLUDED_)
#define AFX_NPC_H__1DE71CDD_4040_4479_828D_E8EE07BD7A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"

class CEbenezerDlg;

class CNpc  
{
public:
	CEbenezerDlg* m_pMain;

	short	m_sNid;				// NPC (��������)�Ϸù�ȣ
	short	m_sSid;				// NPC ���̺� ������ȣ
	short	m_sCurZone;			// Current Zone;
	short	m_sZoneIndex;		// NPC �� �����ϰ� �ִ� ���� �ε���
	float	m_fCurX;			// Current X Pos;
	float	m_fCurY;			// Current Y Pos;
	float	m_fCurZ;			// Current Z Pos;
	float	m_fDir;				// NPC�� ���� �ִ� ����
	short	m_sPid;				// MONSTER(NPC) Picture ID
	short	m_sSize;			// MONSTER(NPC) Size
	int		m_iWeapon_1;
	int		m_iWeapon_2;
	TCHAR	m_strName[MAX_ID_SIZE];		// MONSTER(NPC) Name
	int		m_iMaxHP;			// �ִ� HP
	int		m_iHP;				// ���� HP
	BYTE	m_byState;			// ���� (NPC) ����
	BYTE	m_byGroup;			// �Ҽ� ����
	BYTE	m_byLevel;			// ����
	BYTE	m_tNpcType;			// NPC Type
								// 0 : Normal Monster
								// 1 : NPC
								// 2 : �� �Ա�,�ⱸ NPC
								// 3 : ���
	int   m_iSellingGroup;		// ItemGroup
//	DWORD	m_dwStepDelay;		

	short m_sRegion_X;			// region x position
	short m_sRegion_Z;			// region z position
	BYTE	m_NpcState;			// NPC�� ���� - ��Ҵ�, �׾���, ���ִ� ���...
	BYTE	m_byGateOpen;		// Gate Npc Status -> 1 : open 0 : close
	short   m_sHitRate;			// ���� ������
	BYTE    m_byObjectType;     // ������ 0, objectŸ��(����, ����)�� 1

	short   m_byEvent;		    // This is for the quest. 

public:
	CNpc();
	virtual ~CNpc();

	void Initialize();
	void MoveResult(float xpos, float ypos, float zpos, float speed);
	void NpcInOut(BYTE Type, float fx, float fz, float fy);
	void RegisterRegion();
	void RemoveRegion(int del_x, int del_z);
	void InsertRegion(int del_x, int del_z);
	int GetRegionNpcList(int region_x, int region_z, char *buff, int &t_count);
	BYTE GetState()		{ return m_byState; };

};

#endif // !defined(AFX_NPC_H__1DE71CDD_4040_4479_828D_E8EE07BD7A67__INCLUDED_)
