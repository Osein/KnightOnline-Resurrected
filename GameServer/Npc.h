// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__6077F7AF_6166_463A_AA80_FBF218781BC6__INCLUDED_)
#define AFX_NPC_H__6077F7AF_6166_463A_AA80_FBF218781BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "iocport.h"
#include "./N3Base/My_3DStruct.h"
#include "Map.h"
#include "PathFind.h"
#include "user.h"
#include "NpcMagicProcess.h"

#define MAX_MAP_SIZE		10000
#define MAX_PATH_SIZE		100

#define NPC_MAX_USER_LIST	5

#define NPC_ATTACK_SHOUT	0
#define NPC_SUBTYPE_LONG_MON 1

#define NPC_TRACING_STEP	100

#define NPC_HAVE_USER_LIST	5
#define NPC_HAVE_ITEM_LIST	6
#define NPC_PATTEN_LIST		5
#define NPC_PATH_LIST		50
#define NPC_MAX_PATH_LIST	100
#define NPC_EXP_RANGE		50
#define NPC_EXP_PERSENT		50

#define NPC_SECFORMETER_MOVE	4
#define NPC_SECFORMETER_RUN		4
#define NPC_VIEW_RANGE			100

#define MAX_MAGIC_TYPE3			20
#define MAX_MAGIC_TYPE4			9



struct  _NpcSkillList
{
	short	sSid;
	BYTE	tLevel;
	BYTE	tOnOff;
};

struct  _NpcGiveItem
{
	int 	sSid;			// item serial number
	short	count;			// item ����(���� ����)
};

struct	_ExpUserList
{
	TCHAR	strUserID[MAX_ID_SIZE+1];		// ���̵�(ĳ���� �̸�)
	int		iUid;							// User uid
	int		nDamage;						// Ÿ��ġ ��
	BOOL	bIs;							// �þ߿� �����ϴ����� �Ǵ�(true:����)
	//BOOL	bSameParty;						// ���� ��Ƽ �Ҽ��� �ִٸ� TRUE, �׷��� ������ FALSE
};

struct _Target
{
	int	id;							// ���ݴ�� User uid
	float x;						// User�� x pos
	float y;						// User�� y pos
	float z;						// User�� z pos
	int failCount;
};

struct _PattenPos
{
	short x;
	short z;
};

struct _Patten
{
	int	patten_index;
	_PattenPos pPattenPos[NPC_MAX_PATH_LIST];
};

struct _PathList
{
	_PattenPos pPattenPos[NPC_MAX_PATH_LIST];
};

struct _MagicType3
{
	short		sHPAttackUserID;	// ���� ������ ����� ������ ���̵� ����
	short		sHPAmount;			// ���� damage ( �����Ѿ� / (���ӽð� / 2) )
	BYTE		byHPDuration;		// ���� �ð�
	BYTE		byHPInterval;		// ���� ����
	float		fStartTime;			// ������ ���۵Ǵ� �ð�..
};

struct _MagicType4
{
	BYTE	byAmount;			// ��
	short	sDurationTime;		// ���� �ð�
	float   fStartTime;			// ������ ���۵Ǵ� �ð�..
};

struct _TargetHealer
{
	short	sNID;				// npc nid
	short	sValue;				// ����
};

class CServerDlg;

/*
     ** Repent AI Server �۾��� ���� ���� **
	1. MONSTER DB �ʿ� �ִ� ������ ���� ����..
*/

class CNpc  
{
public:

	CServerDlg* m_pMain;
	CNpcMagicProcess m_MagicProcess;

	_Target	m_Target;				// ������ ���� ����,,
	short		m_ItemUserLevel;		// ������ ���� �̻� �����۸� ���������� �����ؾ��ϴ� �����Ƿ���

	int		m_TotalDamage;	// �� ������ �������
	_ExpUserList m_DamagedUserList[NPC_HAVE_USER_LIST]; // ������ Ÿ��ġ�� �� ���������� ����Ʈ�� �����Ѵ�.(����ġ �й�)
	short   m_sMaxDamageUserid;		// ������ �ְ��� �������� �� ������ ���̵� ����..

	_PathList m_PathList;			// Npc�� �н� ����Ʈ 
	_PattenPos m_pPattenPos;		// Npc�� ����,,

	//int m_iPattenNumber;		// ������ ���Ϲ�ȣ
	short m_iPattenFrame;			// ������ ���� ��ġ..

	BYTE  m_byMoveType;			// NPC�� �ൿŸ��(�̵�����)
	BYTE  m_byInitMoveType;		// NPC�� �ʱ� �ൿŸ��(�̵�����)
	short m_sPathCount;			// NPC�� PathList Count
	short m_sMaxPathCount;		// NPC�� PathList Max Count

	BOOL	m_bFirstLive;		// NPC �� ó�� �����Ǵ��� �׾��� ��Ƴ����� �Ǵ�.
	BYTE	m_NpcState;			// NPC�� ���� - ��Ҵ�, �׾���, ���ִ� ���...
	short	m_ZoneIndex;		// NPC �� �����ϰ� �ִ� ���� �ε���

	short	m_sNid;				// NPC (��������)�Ϸù�ȣ

	CMapInfo	**m_pOrgMap;		// ���� MapInfo �� ���� ������

	float		m_nInitX;			// ó�� ������ ��ġ X
	float		m_nInitY;			// ó�� ������ ��ġ Y
	float		m_nInitZ;			// ó�� ������ ��ġ Z

	short		m_sCurZone;			// Current Zone;
	float		m_fCurX;			// Current X Pos;
	float		m_fCurY;			// Current Y Pos;
	float		m_fCurZ;			// Current Z Pos;

	float		m_fPrevX;			// Prev X Pos;
	float		m_fPrevY;			// Prev Y Pos;
	float		m_fPrevZ;			// Prev Z Pos;

	//
	//	PathFind Info
	//
	short		m_min_x;
	short		m_min_y;
	short		m_max_x;
	short		m_max_y;

	int		m_pMap[MAX_MAP_SIZE];// 2���� -> 1���� �迭�� x * sizey + y

	SIZE	m_vMapSize;

	float m_fStartPoint_X, m_fStartPoint_Y;
	float m_fEndPoint_X, m_fEndPoint_Y;

	short m_sStepCount;

	CPathFind m_vPathFind;
	_PathNode	*m_pPath;

	int		m_nInitMinX;					// �ʱ���ġ
	int		m_nInitMinY;
	int		m_nInitMaxX;
	int		m_nInitMaxY;

	// NPC Class, Skill
	//_NpcSkillList	m_NpcSkill[SKILL_NUM];	// NPC �� ������ �ִ� ��ų
	//BYTE			m_tWeaponClass;			// NPC �� ���� Ŭ���� 

	// ���� ���� ����..
	float		m_fHPChangeTime;			// Hp ȸ����
	float       m_fFaintingTime;			// ������ �ִ� �ð�..
	_MagicType3  m_MagicType3[MAX_MAGIC_TYPE3];			// HP ���õ� ����..
	_MagicType4  m_MagicType4[MAX_MAGIC_TYPE4];			// �ɷ�ġ ���õ� ����..

	//----------------------------------------------------------------
	//	MONSTER DB �ʿ� �ִ� ������
	//----------------------------------------------------------------
	short	m_sSid;				// MONSTER(NPC) Serial ID
	TCHAR	m_strName[MAX_ID_SIZE+1];		// MONSTER(NPC) Name
	short	m_sPid;							// MONSTER(NPC) Picture ID
	short   m_sSize;						// ĳ������ ����(100 �ۼ�Ʈ ����)
	int     m_iWeapon_1;			// ���� ����
	int     m_iWeapon_2;			// ���� ����
	BYTE	m_byGroup;			// �Ҽ�����(���� ����)
	BYTE	m_byActType;		// �ൿ����
	BYTE	m_byRank;			// ����
	BYTE	m_byTitle;			// ����
	int 	m_iSellingGroup;	// ������ �׷�(���ǸŸ� ��� NPC�� ��츸)
	short	m_sLevel;			// level
	int		m_iExp;				// ����ġ
	int		m_iLoyalty;			// loyalty
	int		m_iMaxHP;			// �ִ� HP
	short	m_sMaxMP;			// �ִ� MP
	short	m_sAttack;			// ���ݰ�(���� ������� ����..)
	short	m_sDefense;			// ��
	short	m_sHitRate;			// ���ݹ�ø
	short   m_sEvadeRate;		// ����ø
	short	m_sDamage;			// �⺻ ������ - ���ݰ�
	short	m_sAttackDelay;		// ���ݵ�����
	short	m_sSpeed;			// �̵��ӵ�	
	float   m_fSpeed_1;			// �⺻ �̵� Ÿ��		(1�ʿ� �� �� �ִ� �Ÿ�)
	float   m_fSpeed_2;			// �ٴ� �̵� Ÿ��..		(1�ʿ� �� �� �ִ� �Ÿ�)
	short	m_sStandTime;		// ���ִ� �ð�
	int		m_iMagic1;			// ��븶�� 1 (����)
	int		m_iMagic2;			// ��븶�� 2 (����)	
	int		m_iMagic3;			// ��븶�� 3 (�ɷ�ġ, ����)	
	BYTE	m_byFireR;			// ȭ�� ���׷�
	BYTE	m_byColdR;			// �ñ� ���׷�
	BYTE	m_byLightningR;		// ���� ���׷�
	BYTE	m_byMagicR;			// ���� ���׷�
	BYTE	m_byDiseaseR;		// ���� ���׷�
	BYTE	m_byPoisonR;		// �� ���׷�
	BYTE	m_byLightR;			// �� ���׷�
	float	m_fBulk;			// ������ ũ�� (���� ����)
	BYTE	m_bySearchRange;	// �� Ž�� ����
	BYTE	m_byAttackRange;	// �����Ÿ�
	BYTE	m_byTracingRange;	// �߰� �Ÿ�

	short	m_sAI;				// �ΰ����� �ε���
	
	BYTE	m_tNpcType;			// NPC Type
								// 0 : Normal Monster
								// 1 : NPC

	short	m_byFamilyType;		// ������̿��� �������踦 �����Ѵ�.
	BYTE	m_tItemPer;			// �������� ������ Ȯ��
	BYTE	m_tDnPer;			// ���� ������Ȯ��
	BYTE    m_byMoneyType;		// Event������ ��� ���� ���� �ִ� ��, (0:����, 1:������ ���� �ʰ� �ٷ� �������´�)

	int		m_iMoney;			// �������� ��
	int		m_iItem;			// �������� ������
	//BYTE	m_byMemory;			// ���� Ÿ�� 

	int			m_iHP;				// ���� HP
	short		m_sMP;				// ���� MP
	
	float   m_fSecForMetor;		// �ʴ� �� �� �ִ� �Ÿ�..

	//----------------------------------------------------------------
	//	MONSTER AI�� ���õ� ������
	//----------------------------------------------------------------
	BYTE	m_tNpcLongType;		// ���� �Ÿ� : ���Ÿ�(1), �ٰŸ�(0), ��.����(2)
	BYTE	m_tNpcAttType;		// ���� ���� : ����(1), �İ�(0)
	BYTE	m_tNpcOldAttType;	// ���� ���� : ����(1), �İ�(0) (Ȱ������ ����)
	BYTE	m_tNpcGroupType;	// ������ �����ϳ�(1), ���ϳ�?(0)
	BYTE	m_byNpcEndAttType;	// ���������� �ο��(1), �׷��� ������(0)
//	BYTE	m_tNpcTraceType;	// ������ ���󰣴�(1), �þ߿��� �������� �׸�(0)
	BYTE    m_byAttackPos;		// User�� ��� �κп��� �����ϴ����� �Ǵ�(8����)
	BYTE    m_byBattlePos;		// � ������ ������ �������� �Ǵ�..
	BYTE	m_byWhatAttackType; // ���� Ÿ�� : Normal(0), ��.��Ÿ�����(1), ��(2), ����(3), ����������(4), 1+4�� ����(5)
	BYTE	m_byGateOpen;		// ������ ��쿡.. ���... Gate Npc Status -> 1 : open 0 : close
	BYTE    m_byMaxDamagedNation;	// ���� ���� ������ ������ ����.. (1:ī�罺, 2:������)
	BYTE    m_byObjectType;         // ������ 0, objectŸ��(����, ����)�� 1
	BYTE	m_byDungeonFamily;		// �������� ���� �йи� ���� (���� ��)
	BYTE	m_bySpecialType;		// ������ ���°� ���ϴ����� �Ǵ�(0:������ ����, 1:���ϴ� ����, 
									// 2:�״°�� �����ϴ� ����(������� ������� ������ ����), 
									// 3:��������� ������ ������ �ִ� ����(������Ͱ� ������ ����Ǵ� ���ʹ� ���� �׵���)
									// 4:���ϸ鼭 �״°�� �����ϴ� ���� (m_sControlSid)
									// 5:ó���� �׾��ִٰ� �����ϴ� ����,,
									// 6:�����ð��� ���� �Ŀ� �ൿ�ϴ� ����,,
									// 100:�׾����� �������� ���� ���� ������ ����� �ּ���
	BYTE	m_byTrapNumber;			// �������� Ʈ���� ��ȣ,,
	BYTE	m_byChangeType;			// 0:�������, 1:���ϱ� ���� �غ�, 2:�ٸ����ͷ� ����, 3:������ ����, 100:������ ����
	BYTE	m_byRegenType;			// 0:���������� ������ ��.. , 1:�ѹ� ������ ������ �ȵǴ� Ư�� ��, 2:������ �ȵ�
	BYTE    m_byDeadType;			// 0:��� �ִ� ���, 100:�����̺�Ʈ�� ���� ���
	short   m_sChangeSid;			// ���ϴ� ������ Sid��ȣ..
	short   m_sControlSid;			// �����ϴ� ������ Sid��ȣ..


	//----------------------------------------------------------------
	//	MONSTER_POS DB �ʿ� �ִ� ������
	//----------------------------------------------------------------
	int		m_Delay;			// ���� ���·� ���̵Ǳ� ������ �ð�
	float   m_fDelayTime;		// Npc Threadüũ Ÿ��...

	BYTE	m_byType;
	int		m_sRegenTime;		// NPC ����ð�

	int		m_nLimitMinX;		// Ȱ�� ����
	int		m_nLimitMinZ;
	int		m_nLimitMaxX;
	int		m_nLimitMaxZ;

	long	m_lEventNpc;

	float m_fAdd_x;
	float m_fAdd_z;

	float m_fBattlePos_x;	
	float m_fBattlePos_z;

	float m_fSecForRealMoveMetor;		// �ʴ� �� �� �ִ� �Ÿ�..(���� Ŭ���̾�Ʈ�� �����ִ� �Ÿ�)
	float m_fDir;						// NPC�� ����

	BOOL m_bPathFlag;					// �н� ���ε� ���࿩�� üũ ����..

	//----------------------------------------------------------------
	//	NPC �̵� ����
	//----------------------------------------------------------------
	_NpcPosition	m_pPoint[MAX_PATH_LINE];			// �̵��� ���� ��ǥ

	short m_iAniFrameIndex;
	short m_iAniFrameCount;
	BYTE m_byPathCount;					// �н��� ���� �̵��ϴ� ���� ���� ��ġ�� �ʵ���,, 
	BYTE m_byResetFlag;					// �������ݽ� �н����ε��� �ٽ� �Ұ�����,, ���������� �Ǵ�..
	BYTE m_byActionFlag;				// �ൿ��ȭ �÷��� ( 0 : �ൿ��ȭ ����, 1 : ���ݿ��� �߰�)

	short m_iRegion_X;					// ������ region - x pos
	short m_iRegion_Z;					// ������ region - z pos
	short m_iFind_X[4];					// find enemy���� ã�� Region�˻翵��
	short m_iFind_Y[4];

	float   m_fOldSpeed_1;			// �⺻ �̵� Ÿ��		(1�ʿ� �� �� �ִ� �Ÿ�)
	float   m_fOldSpeed_2;			// �ٴ� �̵� Ÿ��..		(1�ʿ� �� �� �ִ� �Ÿ�)

	// test
	short m_sThreadNumber;				// �ڽ��� ���� �������� ��ȣ


public:
	CNpc();
	virtual ~CNpc();

	void Init();	//	NPC �⺻���� �ʱ�ȭ
	void InitTarget(void);
	void InitUserList();
	void InitPos();
	void InitMagicValuable();

protected:
	void ClearPathFindData(void);

public:

	void FillNpcInfo(char *temp_send, int &index, BYTE flag);
	void NpcStrategy(BYTE type);
	void NpcTypeParser();
	int  FindFriend(int type=0);
	void  FindFriendRegion(int x, int z, MAP* pMap, _TargetHealer* pHealer, int type=0);
	//void  FindFriendRegion(int x, int z, MAP* pMap, int type=0);
	BOOL IsCloseTarget(CUser *pUser, int nRange);
	void ToTargetMove(CIOCPort* pIOCP, CUser* pUser);
	int SendDead(CIOCPort* pIOCP, int type = 1);			// Npc Dead
	void SendExpToUserList();								// User ����ġ �й�..
	BOOL SetDamage(int nAttackType, int nDamage, TCHAR *id, int uid, CIOCPort* pIOCP);	// Npc�� ������ ���..
	BOOL SetHMagicDamage(int nDamage, CIOCPort* pIOCP);	// Npc�� ������ ���..
	int GetDefense();										// Npc�� ��..
	void ChangeTarget(int nAttackType, CUser *pUser, CIOCPort* pIOCP);
	void ChangeNTarget(CNpc *pNpc, CIOCPort* pIOCP);
	int GetFinalDamage(CUser *pUser, int type = 1);
	int GetNFinalDamage(CNpc *pNpc);
	BYTE GetHitRate(float rate);
	BOOL ResetPath();
	BOOL GetTargetPos(float& x, float& z);
	BOOL IsChangePath(int nStep = 1);
	int Attack(CIOCPort* pIOCP);
	int LongAndMagicAttack(CIOCPort* pIOCP);
	int TracingAttack(CIOCPort* pIOCP);
	int GetTargetPath(int option = 0);
	int	GetPartyDamage(int iNumber);
	int IsCloseTarget(int nRange, int Flag=0);
	BOOL StepMove(int nStep, CIOCPort* pIOCP);
	BOOL StepNoPathMove(int nStep);
	BOOL IsMovingEnd();
	BOOL IsMovable(float x, float z);
	int  IsSurround(CUser* pUser);
	BOOL IsDamagedUserList(CUser *pUser);
	void IsUserInSight();
	BOOL IsLevelCheck(int iLevel);
	BOOL IsHPCheck(int iHP);
	BOOL IsCompStatus(CUser* pUser);
	BOOL IsPathFindCheck(float fDistance);						// �н� ���ε带 �Ұ������� üũ�ϴ� ��ƾ..
	void IsNoPathFind(float fDistance);						// �н� ���ε带 ���� �ʰ� ���ݴ������ ���� ��ƾ..
	BOOL IsInExpRange(CUser* pUser);
	void GiveNpcHaveItem(CIOCPort* pIOCP);		// NPC �� ���� �������� ������

	void NpcLive(CIOCPort* pIOCP);
	void NpcFighting(CIOCPort* pIOCP);
	void NpcTracing(CIOCPort* pIOCP);
	void NpcAttacking(CIOCPort* pIOCP);
	void NpcMoving(CIOCPort* pIOCP);
	void NpcSleeping(CIOCPort* pIOCP);
	void NpcFainting(CIOCPort* pIOCP, float currenttime);
	void NpcHealing(CIOCPort* pIOCP);
	void NpcStanding();
	void NpcBack(CIOCPort* pIOCP);
	BOOL SetLive(CIOCPort* pIOCP);

	BOOL IsInRange(int nX, int nZ);
	BOOL RandomMove();				//
	BOOL RandomBackMove();				//
	BOOL IsInPathRange();			//
	int GetNearPathPoint();			//

	// Packet Send�κ�..
	void SendAll(CIOCPort* pIOCP, TCHAR *pBuf, int nLength);
	void SendAttackSuccess(CIOCPort* pIOCP, BYTE byResult, int tuid, short sDamage, int nHP=0, BYTE byFlag = 0, short sAttack_type=1);
	void SendNpcInfoAll(char *temp_send, int &index, int count);	// game server�� npc������ ���� ����...

	// Inline Function
	BOOL SetUid(float x, float z, int id);

	void Dead(CIOCPort* pIOCP, int iDeadType = 0);
	BOOL FindEnemy();
	BOOL CheckFindEnermy();
	int FindEnemyRegion();
	float FindEnemyExpand(int nRX, int nRZ, float fCompDis, int nType);
	int GetMyField();

	void NpcTrace(TCHAR *pMsg);

	int GetDir(float x1, float z1, float x2, float z2);
	void NpcMoveEnd(CIOCPort* pIOCP);

	inline float RandomGenf(float max, float min);
	__Vector3 MyFunc(__Vector3 vCur, __Vector3 vDir, float fYDegree, float fDistance);
	__Vector3 GetDirection(__Vector3 vStart, __Vector3 vEnd);
	// GetVectorPosition : vOrig->vDest�������� vOrig���� fDis�Ÿ���ŭ ������ ��ǥ�� ����
	__Vector3 GetVectorPosition(__Vector3 vOrig, __Vector3 vDest, float fDis);
	// CalcAdaptivePosition : vPosDest->vPosOrig�������� vPosDest���� fDis�Ÿ���ŭ ������ ��ǥ�� ����
	__Vector3 CalcAdaptivePosition(__Vector3 vPosOrig, __Vector3 vPosDest, float fAttackDistance);
	__Vector3 ComputeDestPos( __Vector3 vCur, float fDegree, float fDegreeOffset, float fDistance);
	void Yaw2D(float fDirX, float fDirZ, float& fYawResult);
	float GetDistance(__Vector3 vOrig, __Vector3 vDest);
	int  PathFind(CPoint start, CPoint end, float fDistance);
	BOOL GetUserInView();	// Npc�� ���� �Ÿ��ȿ� User�� �ִ����� �Ǵ�
	BOOL GetUserInViewRange(int x, int z);
	void MoveAttack(CIOCPort* pIOCP);
	void HpChange(CIOCPort* pIOCP);
	void MSpChange(int type, int amount);
	void ItemWoreOut( int type, int damage );
	int	 ItemProdution(int item_number);
	int  GetItemGrade(int item_grade);
	int  GetItemCodeNumber(int level, int item_type);
	int  GetWeaponItemCodeNumber(int item_type);
	void DurationMagic_4(CIOCPort* pIOCP, float currenttime);
	void DurationMagic_3(CIOCPort* pIOCP, float currenttime);
	void ChangeMonsterInfomation(int iChangeType);
	int  GetPartyExp( int party_level, int man, int nNpcExp );
	void ChangeAbility(int iChangeType);
	BOOL Teleport(CIOCPort* pIOCP);
};

#endif // !defined(AFX_NPC_H__6077F7AF_6166_463A_AA80_FBF218781BC6__INCLUDED_)
