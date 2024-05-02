// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__1CC8CB68_CF95_4849_8E89_134826B1FAC2__INCLUDED_)
#define AFX_USER_H__1CC8CB68_CF95_4849_8E89_134826B1FAC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "iocport.h"
#include "MagicProcess.h"
class CServerDlg;

#include "extern.h"
#include "STLMap.h"
#include <list>

typedef std::list <_USERLOG*>		UserLogList;

class CUser  
{
public:
	CServerDlg* m_pMain;

	CIOCPort* m_pIocport;	
	CMagicProcess m_MagicProcess;

	UserLogList	m_UserLogList;

	// �ʿ��� ���� ������ ����,,,
	// ĳ���� �⺻ �Ӽ�
	char m_strUserID[MAX_ID_SIZE+1];	// ĳ������ �̸�
	int		m_iUserId;					// User�� ��ȣ
	BYTE	m_bLive;					// �׾���? ��Ҵ�?

	float			m_curx;				// ���� X ��ǥ
	float			m_cury;				// ���� Y ��ǥ
	float			m_curz;				// ���� Z ��ǥ
	float			m_fWill_x;			// ���� X ��ǥ
	float			m_fWill_y;			// ���� Y ��ǥ
	float			m_fWill_z;			// ���� Z ��ǥ
	short			m_sSpeed;			// ������ ���ǵ�	
	BYTE 			m_curZone;			// ���� ��
	short			m_sZoneIndex;		// ���� ���� index ��ȣ..

	BYTE	m_bNation;						// �Ҽӱ���
	short	m_sLevel;						// ����

	short	m_sHP;							// HP
	short	m_sMP;							// MP
	short	m_sSP;							// SP
	short	m_sMaxHP;						// HP
	short	m_sMaxMP;						// MP
	short	m_sMaxSP;						// SP

	BYTE	m_state;						// User�� ����

	short	m_sRegionX;						// ���� ���� X ��ǥ
	short	m_sRegionZ;						// ���� ���� Z ��ǥ
	short	m_sOldRegionX;					// ���� ���� X ��ǥ
	short	m_sOldRegionZ;					// ���� ���� Z ��ǥ

	BYTE	m_bResHp;						// ȸ����
	BYTE	m_bResMp;
	BYTE	m_bResSta;

	BYTE    m_byNowParty;				// ��Ƽ���̸� 1, �δ����̸� 2, �Ѵ� �ƴϸ� 0
	BYTE	m_byPartyTotalMan;			// ��Ƽ ���� �� ���� �ο��� 
	short   m_sPartyTotalLevel;			// ��Ƽ ���� ����� �� ����
	short	m_sPartyNumber;				// ��Ƽ ��ȣ

	short	m_sHitDamage;				// Hit
	float	m_fHitrate;					// ���� ��ø��
	float	m_fAvoidrate;				// ��� ��ø��
	short	m_sAC;						// �����
	short   m_sItemAC;                  // ������ ����
	

	short  m_sSurroundNpcNumber[8];		// Npc �ٱ�~

	BYTE   m_byIsOP;					// ��������� �Ǵ�..
	long   m_lUsed;						// ������ �������.. (1:�����.. ���� ���ġ ����. 0:����ص� ����)

	BOOL		m_bLogOut;				// Logout ���ΰ�?

	// 2002.7.10 - Yookozuna
	BYTE    m_bMagicTypeLeftHand;			// The type of magic item in user's left hand  
	BYTE    m_bMagicTypeRightHand;			// The type of magic item in user's right hand
	short   m_sMagicAmountLeftHand;         // The amount of magic item in user's left hand
	short	m_sMagicAmountRightHand;        // The amount of magic item in user's left hand

public:
	void InitUserLog();
	void WriteUserLog();
	short GetMagicDamage(int damage, short tid);
	void Initialize();
	void InitNpcAttack();
	void Attack(int sid, int tid);	// ATTACK
	void SetDamage(int damage, int tid);				// user damage
	void Dead(int tid, int nDamage);					// user dead
	void SetExp(int iNpcExp, int iLoyalty, int iLevel);		// user exp
	void SetPartyExp(int iNpcExp, int iLoyalty, int iPartyLevel, int iMan);		// user exp
	short GetDamage(int tid, int magicid=0);
	BYTE GetHitRate(float rate);
	int IsSurroundCheck(float fX, float fY, float fZ, int NpcID);
	void HealMagic();
	void HealAreaCheck(int rx, int rz);

	void SendAttackSuccess(int tuid, BYTE result, short sDamage, int nHP=0, short sAttack_type=1);  // ���� ����
	void SendMagicAttackResult(int tuid, BYTE result, short sDamage, short sHP=0);  // ���� ����
	void SendHP();												// user�� HP
	void SendExp(int iExp, int iLoyalty, int tType = 1);
	void SendSystemMsg(TCHAR *pMsg, BYTE type, int nWho);
	void SendAll(TCHAR *pBuf, int nLength);						// game server�� ��Ŷ ����...
	BOOL IsOpIDCheck(char* szName);

	CUser();
	virtual ~CUser();
};

#endif // !defined(AFX_USER_H__1CC8CB68_CF95_4849_8E89_134826B1FAC2__INCLUDED_)
