// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "User.h"
#include "Serverdlg.h"
#include "define.h"
#include "Region.h"
#include "GameSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "extern.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
     ** Repent AI Server �۾��� ���� ���� **
	1. Initialize() ����
	2. SendAttackSuccess() ����
	3. GetDamage() ����
*/

#define MORAL_GOOD		0x01
#define MORAL_BAD		0x02
#define MORAL_NEUTRAL	0x03

// ��� ���̵� �ֱ�..
/*const char* g_pszOPID[] = 
{
	//"�����2",
	//"������",
	//"�̻ۿ���2"
	//"Morpheus"
//	"�Ǽ�",
//	"�μ�"
};*/

float surround_fx[8] = {0.0f, -0.7071f, -1.0f, -0.7083f,  0.0f,  0.7059f,  1.0000f, 0.7083f};
float surround_fz[8] = {1.0f,  0.7071f,  0.0f, -0.7059f, -1.0f, -0.7083f, -0.0017f, 0.7059f};

extern CRITICAL_SECTION g_region_critical;
extern CRITICAL_SECTION g_LogFileWrite;

CUser::CUser()
{

}

CUser::~CUser()
{

}

void CUser::Initialize()
{
	m_pMain = (CServerDlg*)AfxGetMainWnd();
	m_pIocport = (CIOCPort*)&m_pMain->m_Iocport;

	m_MagicProcess.m_pMain = m_pMain;
	m_MagicProcess.m_pSrcUser = this;

	memset(m_strUserID, 0, MAX_ID_SIZE+1);	// ĳ������ �̸�
	m_iUserId = -1;					// User�� ��ȣ
	m_bLive = USER_DEAD;			// �׾���? ��Ҵ�?
	m_curx = 0.0f;				// ���� X ��ǥ
	m_cury = 0.0f;				// ���� Y ��ǥ
	m_curz = 0.0f;				// ���� Z ��ǥ
	m_fWill_x = 0.0f;
	m_fWill_y = 0.0f;
	m_fWill_z = 0.0f;
	m_curZone = -1;				// ���� ��
	m_sZoneIndex = -1;
	m_bNation = 0;						// �Ҽӱ���
	m_sLevel = 0;						// ����
	m_sHP = 0;							// HP
	m_sMP = 0;							// MP
	m_sSP = 0;							// SP
	m_sMaxHP = 0;							// MaxHP
	m_sMaxMP = 0;							// MaxMP
	m_sMaxSP = 0;							// MaxSP
	m_state = 0;				// User�� ����
	m_sRegionX = 0;						// ���� ���� X ��ǥ
	m_sRegionZ = 0;						// ���� ���� Z ��ǥ
	m_sOldRegionX = 0;	
	m_sOldRegionZ = 0;	
	m_bResHp = 0;						// ȸ����
	m_bResMp = 0;
	m_bResSta = 0;
	m_sHitDamage = 0;					// Hit
	m_sAC = 0;
	m_sItemAC = 0;
	m_fHitrate = 0.0f;					// Ÿ�� ������
	m_fAvoidrate = 0;					// ȸ�� ������
	m_bLogOut = FALSE;				// Logout ���ΰ�?
	m_byNowParty = 0;
	m_sPartyTotalLevel = 0;
	m_byPartyTotalMan = 0;
	m_sPartyNumber = -1;
	m_byIsOP = 0;
	m_lUsed = 0;
	InitNpcAttack();

	m_bMagicTypeLeftHand = 0;			
	m_bMagicTypeRightHand = 0;			
	m_sMagicAmountLeftHand = 0;       
	m_sMagicAmountRightHand = 0;      
}

void CUser::Attack(int sid, int tid)
{
	CNpc* pNpc = m_pMain->m_arNpc.GetData(tid-NPC_BAND);
	if(pNpc == NULL)	return;
	if(pNpc->m_NpcState == NPC_DEAD) return;
	if(pNpc->m_iHP == 0) return;

/*	if(pNpc->m_tNpcType == NPCTYPE_GUARD)					// ����̸� Ÿ���� �ش� ������
	{
		pNpc->m_Target.id = m_iUserId + USER_BAND;
		pNpc->m_Target.x = m_curx;
		pNpc->m_Target.y = m_cury;
		pNpc->m_Target.failCount = 0;
		pNpc->Attack(m_pIocport);
	//	return;
	}	*/

	int nDefence = 0, nFinalDamage = 0;
	// NPC �� 
	nDefence = pNpc->GetDefense();

	// �����̸� //Damage ó�� ----------------------------------------------------------------//
	nFinalDamage = GetDamage(tid);
	if( m_pMain->m_byTestMode )		nFinalDamage = 3000;	// sungyong test
		
	// Calculate Target HP	 -------------------------------------------------------//
	short sOldNpcHP = pNpc->m_iHP;

	if(pNpc->SetDamage(0, nFinalDamage, m_strUserID, m_iUserId + USER_BAND, m_pIocport) == FALSE)
	{
		// Npc�� ���� ���,,
		pNpc->SendExpToUserList(); // ����ġ �й�!!
		pNpc->SendDead(m_pIocport);
		SendAttackSuccess(tid, ATTACK_TARGET_DEAD, nFinalDamage, pNpc->m_iHP);

	//	CheckMaxValue(m_dwXP, 1);		// ���� �������� 1 ����!	
	//	SendXP();
	}
	else
	{
		// ���� ��� ����
		SendAttackSuccess(tid, ATTACK_SUCCESS, nFinalDamage, pNpc->m_iHP);
	}
	//	m_dwLastAttackTime = GetTickCount();
}

void CUser::SendAttackSuccess(int tuid, BYTE result, short sDamage, int nHP, short sAttack_type)
{
	int send_index = 0;
	int sid = -1, tid = -1;
	BYTE type, bResult;
	char buff[256];
	memset( buff, 0x00, 256 );
	float rx=0.0f, ry=0.0f, rz=0.0f;

	type = 0x01;
	bResult = result;
	sid = m_iUserId+USER_BAND;
	tid = tuid;

	SetByte( buff, AG_ATTACK_RESULT, send_index );
	SetByte( buff, type, send_index );
	SetByte( buff, bResult, send_index );
	SetShort( buff, sid, send_index );
	SetShort( buff, tid, send_index );
	SetShort( buff, sDamage, send_index );
	SetDWORD( buff, nHP, send_index );
	SetByte( buff, sAttack_type, send_index );

	//TRACE("User - SendAttackSuccess() : [sid=%d, tid=%d, result=%d], damage=%d, hp = %d\n", sid, tid, bResult, sDamage, sHP);

	SendAll(buff, send_index);   // thread ���� send
}

void CUser::SendMagicAttackResult(int tuid, BYTE result, short sDamage, short sHP)
{
	int send_index = 0;
	int sid = -1, tid = -1;
	BYTE type, bResult;
	char buff[256];
	memset( buff, 0x00, 256 );
	float rx=0.0f, ry=0.0f, rz=0.0f;

	type = 0x01;				
	bResult = result;
	sid = m_iUserId+USER_BAND;
	tid = tuid;

	SetByte( buff, AG_MAGIC_ATTACK_RESULT, send_index );
	SetByte( buff, type, send_index );
	SetByte( buff, bResult, send_index );
	SetShort( buff, sid, send_index );
	SetShort( buff, tid, send_index );
	SetShort( buff, sDamage, send_index );
	SetShort( buff, sHP, send_index );

	//TRACE("User - SendAttackSuccess() : [sid=%d, tid=%d, result=%d], damage=%d, hp = %d\n", sid, tid, bResult, sDamage, sHP);

	SendAll(buff, send_index);   // thread ���� send
}

// sungyong 2002.05.22
void CUser::SendAll(TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SOCKET_BUFF_SIZE) return;

	if( m_iUserId < 0 || m_iUserId >= MAX_USER)	{
		TRACE("#### User SendAll Fail : point fail ,, nid=%d, name=%s ####\n", m_iUserId, m_strUserID);
		return;
	}

	if( m_pIocport == NULL) return;
	if(m_sZoneIndex < 0 || m_sZoneIndex > m_pMain->g_arZone.size())	return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->sCurZone = m_curZone;
	pNewData->sLength = nLength;
	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	EnterCriticalSection( &(m_pIocport->m_critSendData) );
	m_pIocport->m_SendDataList.push_back( pNewData );
	LeaveCriticalSection( &(m_pIocport->m_critSendData) );

	PostQueuedCompletionStatus( m_pIocport->m_hSendIOCP, 0, 0, NULL );
}
// ~sungyong 2002.05.22

//	Damage ���, ���� m_sHP �� 0 �����̸� ���ó��
void CUser::SetDamage(int damage, int tid)
{
	if(damage <= 0) return;
	if(m_bLive == USER_DEAD) return;

	short sHP = m_sHP;

	m_sHP -= (short)damage;

	//TRACE("User - SetDamage() : old=%d, damage=%d, curHP = %d, id=%s, uid=%d\n", sHP, damage, m_sHP, m_strUserID, m_iUserId);

	if( m_sHP <= 0 )	{
		m_sHP = 0;
		Dead(tid, damage);
	}

	//SendHP();
	// �������̸� �ٸ� ��������� ������.
}

void CUser::Dead(int tid, int nDamage)
{
	if(m_bLive == USER_DEAD) return;

	// �� �κп��� update�� �ؾ� ��,,  ���Ӽ�������,, ó���ϵ���,,,
	m_sHP = 0;
	m_bLive = USER_DEAD;

	InitNpcAttack();

	// region���� ����...
	if(m_sZoneIndex < 0 || m_sZoneIndex > m_pMain->g_arZone.size())	{ 
		TRACE("#### User-Dead ZoneIndex Fail : [name=%s], zoneindex=%d #####\n", m_strUserID, m_sZoneIndex);
		return;
	}
	MAP* pMap = m_pMain->g_arZone[m_sZoneIndex];
	if(pMap == NULL)	{
		TRACE("#### CUser-Dead() Fail : [nid=%d, name=%s], pMap == NULL #####\n", m_iUserId, m_strUserID);
		return;
	}
	// map�� region���� ���� ���� ����..
	if(m_sRegionX < 0 || m_sRegionZ < 0 || m_sRegionX > pMap->GetXRegionMax() || m_sRegionZ > pMap->GetZRegionMax())	{
		TRACE("#### CUser-Dead() Fail : [nid=%d, name=%s], x1=%d, z1=%d #####\n", m_iUserId, m_strUserID, m_sRegionX, m_sRegionZ);
		return;
	}
	//pMap->m_ppRegion[m_sRegionX][m_sRegionZ].DeleteUser(m_iUserId);
	pMap->RegionUserRemove(m_sRegionX, m_sRegionZ, m_iUserId);
	//TRACE("*** User Dead()-> User(%s, %d)�� Region�� ����,, region_x=%d, y=%d\n", m_strUserID, m_iUserId, m_sRegionX, m_sRegionZ);

	m_sRegionX = -1;		m_sRegionZ = -1;

	int send_index = 0;
	int sid = -1, targid = -1;
	BYTE type, result;
	char buff[256];
	memset( buff, 0x00, 256 );

	wsprintf(buff, "*** User Dead = %d, %s ***", m_iUserId, m_strUserID);
	TimeTrace(buff);
	//TRACE("*** User Dead = %d, %s ********\n", m_iUserId, m_strUserID);
	memset( buff, 0x00, 256 );

	float rx=0.0f, ry=0.0f, rz=0.0f;

	type = 0x02;
	result = ATTACK_TARGET_DEAD;
	sid = tid;
	targid = m_iUserId+USER_BAND;

	SetByte( buff, AG_ATTACK_RESULT, send_index );
	SetByte( buff, type, send_index );
	SetByte( buff, result, send_index );
	SetShort( buff, sid, send_index );
	SetShort( buff, targid, send_index );
	SetShort( buff, nDamage, send_index );
	SetDWORD( buff, m_sHP, send_index );
	//SetShort( buff, m_sMaxHP, send_index );

	//TRACE("Npc - SendAttackSuccess()-User Dead : [sid=%d, tid=%d, result=%d], damage=%d, hp = %d\n", sid, targid, result, nDamage, m_sHP);

	if(tid > 0)
		SendAll(buff, send_index);   // thread ���� send

/*	SetByte(buff, AG_DEAD, send_index );
	SetShort(buff, m_iUserId, send_index );
	Setfloat(buff, m_curx, send_index);
	Setfloat(buff, m_curz, send_index);

	SendAll(buff, send_index);   // thread ���� send	*/
}

void CUser::SendHP()
{
	if(m_bLive == USER_DEAD) return;
	// HP �������� ���Ӽ�����...
	int send_index = 0;
	char buff[256];
	memset( buff, 0x00, 256 );

	SetByte(buff, AG_USER_SET_HP, send_index );
	SetShort(buff, m_iUserId, send_index );
	SetDWORD(buff, m_sHP, send_index );

	SendAll(buff, send_index);   
}

void CUser::SetExp(int iNpcExp, int iLoyalty, int iLevel)
{
	int nExp = 0;
	int nLoyalty = 0;
	int nLevel = 0;
	double TempValue = 0;
	nLevel = iLevel - m_sLevel;

	if(nLevel <= -14)	{
		//TRACE("$$ User - SetExp Level Fail : %s, exp=%d, loyalty=%d, mylevel=%d, level=%d $$\n", m_strUserID, iNpcExp, iLoyalty, m_sLevel, iLevel);
		//return;
		TempValue = iNpcExp * 0.2;
		nExp = (int)TempValue;
		if(TempValue > nExp)  nExp=nExp+1;
		TempValue = iLoyalty * 0.2;
		nLoyalty = (int)TempValue;
		if(TempValue > nLoyalty)  nLoyalty=nLoyalty+1;
	}
	else if(nLevel <= -8 && nLevel >= -13)
	{
		TempValue = iNpcExp * 0.5;
		nExp = (int)TempValue;
		if(TempValue > nExp)  nExp=nExp+1;
		TempValue = iLoyalty * 0.5;
		nLoyalty = (int)TempValue;
		if(TempValue > nLoyalty)  nLoyalty=nLoyalty+1;
	}
	else if(nLevel <= -2 && nLevel >= -7)
	{
		TempValue = iNpcExp * 0.8;
		nExp = (int)TempValue;
		if(TempValue > nExp)  nExp=nExp+1;
		TempValue = iLoyalty * 0.8;
		nLoyalty = (int)TempValue;
		if(TempValue > nLoyalty)  nLoyalty=nLoyalty+1;
	}
	else if(nLevel >= -1 ) // && nLevel < 2)
	{
		nExp = iNpcExp * 1;
		nLoyalty = iLoyalty * 1;
	}
/*	else if(nLevel >= 2 && nLevel < 5)
	{
		TempValue = iNpcExp * 1.2;
		nExp = (int)TempValue;
		if(TempValue > nExp)  nExp=nExp+1;
		TempValue = iLoyalty * 1.2;
		nLoyalty = (int)TempValue;
		if(TempValue > nLoyalty)  nLoyalty=nLoyalty+1;
	}
	else if(nLevel >= 5 && nLevel < 8)
	{
		TempValue = iNpcExp * 1.5;
		nExp = (int)TempValue;
		if(TempValue > nExp)  nExp=nExp+1;
		TempValue = iLoyalty * 1.5;
		nLoyalty = (int)TempValue;
		if(TempValue > nLoyalty)  nLoyalty=nLoyalty+1;
	}
	else if(nLevel >= 8)
	{
		nExp = iNpcExp * 2;
		nLoyalty = iLoyalty * 2;
	}	*/

	//TRACE("$$ User - SetExp Level : %s, exp=%d->%d, loy=%d->%d, mylevel=%d, monlevel=%d $$\n", m_strUserID, iNpcExp, nExp, iLoyalty, nLoyalty, m_sLevel, iLevel);

	SendExp(nExp, nLoyalty);
}

void CUser::SetPartyExp(int iNpcExp, int iLoyalty, int iPartyLevel, int iMan)
{
	int nExp = 0;
	int nLoyalty = 0;
	int nPercent = 0, nLevelPercent = 0, nExpPercent = 0;
	double TempValue = 0;

	TempValue = (double)iPartyLevel / 100.0;
	nExpPercent = iNpcExp * TempValue;

	//TRACE("$$ User - SetPartyExp Level : %s, exp=%d->%d, loy=%d->%d, mylevel=%d, iPartyLevel=%d $$\n", m_strUserID, iNpcExp, nExpPercent, iLoyalty, nLoyalty, m_sLevel, iPartyLevel);

	SendExp(iNpcExp, iLoyalty);
}

//  ����ġ�� ������. (�������϶� ���� ��ġ�� �ش�)
void CUser::SendExp(int iExp, int iLoyalty, int tType)
{
	int send_index = 0;
	char buff[256];
	memset( buff, 0x00, 256 );

	SetByte(buff, AG_USER_EXP, send_index );
	SetShort(buff, m_iUserId, send_index );
	SetShort(buff, iExp, send_index );
	SetShort(buff, iLoyalty, send_index );

	//TRACE("$$ User - SendExp : %s, exp=%d, loyalty=%d $$\n", m_strUserID, iExp, iLoyalty);

	SendAll(buff, send_index);   	
}

short CUser::GetDamage(int tid, int magicid)
{
	short damage = 0;	float Attack = 0;	float Avoid = 0;
	short Hit = 0;	short HitB = 0;
	short Ac = 0; int random = 0;	BYTE result;

	_MAGIC_TABLE* pTable = NULL;
	_MAGIC_TYPE1* pType1 = NULL; 
	_MAGIC_TYPE2* pType2 = NULL;

	if (tid < NPC_BAND || tid > INVALID_BAND)	return damage;
	CNpc* pNpc = NULL;
	pNpc = m_pMain->m_arNpc.GetData(tid-NPC_BAND);
	if(pNpc == NULL)		return damage;
	if(pNpc->m_tNpcType == NPC_ARTIFACT || pNpc->m_tNpcType == NPC_PHOENIX_GATE || pNpc->m_tNpcType == NPC_GATE_LEVER || pNpc->m_tNpcType == NPC_SPECIAL_GATE ) return damage;
	
	Attack = (float)m_fHitrate;			// ���ݹ�ø
	Avoid = (float)pNpc->m_sEvadeRate;	// ����ø	
	Hit = m_sHitDamage;					// ������ Hit 
//	Ac = (short)(pNpc->m_sDefense) + pNpc->m_sLevel;		// ����� Ac 2002.07.06
	Ac = (short)(pNpc->m_sDefense);		// ����� Ac 
	HitB = (int)((Hit * 200) / (Ac + 240)) ;	// ���ο� ���ݽ��� B

	if( magicid > 0 )	{	 // Skill Hit.
		pTable = m_pMain->m_MagictableArray.GetData( magicid );     // Get main magic table.
		if( !pTable ) return -1; 
		
		if (pTable->bType1 == 1)	{	// SKILL HIT!
			pType1 = m_pMain->m_Magictype1Array.GetData( magicid );	    // Get magic skill table type 1.
			if( !pType1 ) return -1;     	                                

			if(pType1->bHitType)	{  // Non-relative hit.
				random = myrand(0,100) ;       
				if (pType1->sHitRate <= random)			result = FAIL;
				else 									result = SUCCESS;
			}
			else	{     // Relative hit.
				result = GetHitRate( (Attack / Avoid) * (pType1->sHitRate / 100.0f) );
			}
/*
			if(pType1->bHitType) {  // Non-relative hit.
				Hit = m_sHitDamage * (pType1->sHit / 100.0f);
			}
			else {
//				Hit = (m_sHitDamage - pNpc->m_sDefense) 
				Hit = HitB * (pType1->sHit / 100.0f) ;
			}		
*/
			Hit = HitB * (pType1->sHit / 100.0f) ;
		}
		else if (pTable->bType1 == 2)   { // ARROW HIT!
			pType2 = m_pMain->m_Magictype2Array.GetData( magicid );	    // Get magic skill table type 1.
			if( !pType2 ) return -1; 
			
			if(pType2->bHitType == 1 || pType2->bHitType == 2 )   {  // Non-relative/Penetration hit.
				random = myrand(0,100) ; 
				if (pType2->sHitRate <= random)			result = FAIL;
				else									result = SUCCESS;
				//result = SUCCESS;
			}
			else	{     // Relative hit/Arc hit.
				result = GetHitRate( (Attack / Avoid) * (pType2->sHitRate / 100.0f) );
			}
			
			if(pType2->bHitType == 1 /* || pType2->bHitType == 2 */ )   {
				Hit = m_sHitDamage * (pType2->sAddDamage / 100.0f);
			}
			else{				
//				Hit = (m_sHitDamage - pNpc->m_sDefense) * (pType2->sAddDamage / 100.0f);
				Hit = HitB * (pType2->sAddDamage / 100.0f);
			}
		}
	}
	else	{	// Normal Hit.
		result = GetHitRate(Attack/Avoid);		// Ÿ�ݺ� ���ϱ�
	}

	switch(result)	{
		case GREAT_SUCCESS:
		case SUCCESS:
		case NORMAL:		
			if( magicid > 0 ) {	 // ��ų ����
				damage = (short)Hit;
				random = myrand(0, damage);
//				damage = (short)((0.85f * (float)Hit) + 0.3f * (float)random);
				if (pTable->bType1 == 1) {
					damage = (short)((float)Hit + 0.3f * (float)random + 0.99);
				}
				else {
					damage = (short)((float)(Hit*0.6f) + 1.0f * (float)random + 0.99);
				}
			}				
			else {			//�Ϲ� ����	
				damage = (short)(HitB);
				random = myrand(0, damage);
				damage = (short)((0.85f * (float)HitB) + 0.3f * (float)random);
			}

			break;
		case FAIL:  // ����� �䱸 
				damage = 0;
			break;
	}
	
	damage = GetMagicDamage(damage,tid);	// 2. Magical item damage....

	//return 3000;
	return damage;		
}

short CUser::GetMagicDamage(int damage, short tid)
{
	short total_r = 0;
	short temp_damage = 0;

	CNpc* pNpc = NULL;
	pNpc = m_pMain->m_arNpc.GetData(tid-NPC_BAND);
	if(!pNpc) return damage;
	
	// RIGHT HAND!!! by Yookozuna
	if (m_bMagicTypeRightHand > 4 && m_bMagicTypeRightHand < 8) {
		temp_damage = damage * m_sMagicAmountRightHand / 100;
	}

	switch (m_bMagicTypeRightHand) {	// RIGHT HAND!!!
		case 1 :	// Fire Damage
			total_r = pNpc->m_byFireR ;
			break;
		case 2 :	// Ice Damage
			total_r = pNpc->m_byColdR ;
			break;
		case 3 :	// Lightning Damage
			total_r = pNpc->m_byLightningR ;
			break;
		case 4 :	// Poison Damage
			total_r = pNpc->m_byPoisonR ;
			break;
		case 5 :	// HP Drain						
			break;
		case 6 :	// MP Damage		
			pNpc->MSpChange(2, -temp_damage);
			break;
		case 7 :	// MP Drain				
			break;
		case 0:
			break;
	}

	if (m_bMagicTypeRightHand > 0 && m_bMagicTypeRightHand < 5) {	
		temp_damage = m_sMagicAmountRightHand - m_sMagicAmountRightHand * total_r / 200;
		damage = damage + temp_damage;
	}

	total_r = 0 ;		// Reset all temporary data.
	temp_damage = 0 ;

	// LEFT HAND!!! by Yookozuna
	if (m_bMagicTypeLeftHand > 4 && m_bMagicTypeLeftHand < 8) {
		temp_damage = damage * m_sMagicAmountLeftHand / 100;
	}

	switch (m_bMagicTypeLeftHand) {	// LEFT HAND!!!
		case 1 :	// Fire Damage
			total_r = pNpc->m_byFireR;
			break;
		case 2 :	// Ice Damage
			total_r = pNpc->m_byColdR;
			break;
		case 3 :	// Lightning Damage
			total_r = pNpc->m_byLightningR;
			break;
		case 4 :	// Poison Damage
			total_r = pNpc->m_byPoisonR;
			break;
		case 5 :	// HP Drain					
			break;
		case 6 :	// MP Damage		
			pNpc->MSpChange(2, -temp_damage);
			break;
		case 7 :	// MP Drain		
			break;
		case 0:
			break;
	}

	if (m_bMagicTypeLeftHand > 0 && m_bMagicTypeLeftHand < 5) {
		if (total_r > 200) total_r = 200;
		temp_damage = m_sMagicAmountLeftHand - m_sMagicAmountLeftHand * total_r / 200;
		damage = damage + temp_damage;
	}

	return damage;
}

BYTE CUser::GetHitRate(float rate)
{
/*
	BYTE result;
	int random = 0;
	random = myrand(1, 10000);

	if( rate >= 5.0 )
	{
		if( random >= 1 && random <= 3500)
			result = GREAT_SUCCESS;
		else if( random >= 3501 && random <= 7500)
			result = SUCCESS;
		else if( random >= 7501 && random <= 9500)
			result = NORMAL;
		else if( random >= 9501 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 5.0 && rate >= 3.0)
	{
		if( random >= 1 && random <= 2500)
			result = GREAT_SUCCESS;
		else if( random >= 2501 && random <= 6000)
			result = SUCCESS;
		else if( random >= 6001 && random <= 9000)
			result = NORMAL;
		else if( random >= 9001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 3.0 && rate >= 2.0)
	{
		if( random >= 1 && random <= 2000)
			result = GREAT_SUCCESS;
		else if( random >= 2001 && random <= 5000)
			result = SUCCESS;
		else if( random >= 5001 && random <= 9000)
			result = NORMAL;
		else if( random >= 9001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 2.0 && rate >= 1.25)
	{
		if( random >= 1 && random <= 1500)
			result = GREAT_SUCCESS;
		else if( random >= 1501 && random <= 4000)
			result = SUCCESS;
		else if( random >= 4001 && random <= 8500)
			result = NORMAL;
		else if( random >= 8501 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 1.25 && rate >= 0.8)
	{
		if( random >= 1 && random <= 1000)
			result = GREAT_SUCCESS;
		else if( random >= 1001 && random <= 3000)
			result = SUCCESS;
		else if( random >= 3001 && random <= 8000)
			result = NORMAL;
		else if( random >= 8001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}	
	else if ( rate < 0.8 && rate >= 0.5)
	{
		if( random >= 1 && random <= 800)
			result = GREAT_SUCCESS;
		else if( random >= 801 && random <= 2400)
			result = SUCCESS;
		else if( random >= 2401 && random <= 7000)
			result = NORMAL;
		else if( random >= 7001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 0.5 && rate >= 0.33)
	{
		if( random >= 1 && random <= 600)
			result = GREAT_SUCCESS;
		else if( random >= 601 && random <= 1800)
			result = SUCCESS;
		else if( random >= 1801 && random <= 6000)
			result = NORMAL;
		else if( random >= 6001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 0.33 && rate >= 0.2)
	{
		if( random >= 1 && random <= 400)
			result = GREAT_SUCCESS;
		else if( random >= 401 && random <= 1300)
			result = SUCCESS;
		else if( random >= 1301 && random <= 5000)
			result = NORMAL;
		else if( random >= 5001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else
	{
		if( random >= 1 && random <= 200)
			result = GREAT_SUCCESS;
		else if( random >= 201 && random <= 800)
			result = SUCCESS;
		else if( random >= 801 && random <= 4000)
			result = NORMAL;
		else if( random >= 4001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	
	return result;
*/

	BYTE result;
	int random = 0;
	random = myrand(1, 10000);

	if( rate >= 5.0 )
	{
		if( random >= 1 && random <= 3500)
			result = GREAT_SUCCESS;
		else if( random >= 3501 && random <= 7500)
			result = SUCCESS;
		else if( random >= 7501 && random <= 9800)
			result = NORMAL;
		else if( random >= 9801 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 5.0 && rate >= 3.0)
	{
		if( random >= 1 && random <= 2500)
			result = GREAT_SUCCESS;
		else if( random >= 2501 && random <= 6000)
			result = SUCCESS;
		else if( random >= 6001 && random <= 9600)
			result = NORMAL;
		else if( random >= 9601 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 3.0 && rate >= 2.0)
	{
		if( random >= 1 && random <= 2000)
			result = GREAT_SUCCESS;
		else if( random >= 2001 && random <= 5000)
			result = SUCCESS;
		else if( random >= 5001 && random <= 9400)
			result = NORMAL;
		else if( random >= 9401 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 2.0 && rate >= 1.25)
	{
		if( random >= 1 && random <= 1500)
			result = GREAT_SUCCESS;
		else if( random >= 1501 && random <= 4000)
			result = SUCCESS;
		else if( random >= 4001 && random <= 9200)
			result = NORMAL;
		else if( random >= 9201 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 1.25 && rate >= 0.8)
	{
		if( random >= 1 && random <= 1000)
			result = GREAT_SUCCESS;
		else if( random >= 1001 && random <= 3000)
			result = SUCCESS;
		else if( random >= 3001 && random <= 9000)
			result = NORMAL;
		else if( random >= 9001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}	
	else if ( rate < 0.8 && rate >= 0.5)
	{
		if( random >= 1 && random <= 800)
			result = GREAT_SUCCESS;
		else if( random >= 801 && random <= 2500)
			result = SUCCESS;
		else if( random >= 2501 && random <= 8000)
			result = NORMAL;
		else if( random >= 8001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 0.5 && rate >= 0.33)
	{
		if( random >= 1 && random <= 600)
			result = GREAT_SUCCESS;
		else if( random >= 601 && random <= 2000)
			result = SUCCESS;
		else if( random >= 2001 && random <= 7000)
			result = NORMAL;
		else if( random >= 7001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 0.33 && rate >= 0.2)
	{
		if( random >= 1 && random <= 400)
			result = GREAT_SUCCESS;
		else if( random >= 401 && random <= 1500)
			result = SUCCESS;
		else if( random >= 1501 && random <= 6000)
			result = NORMAL;
		else if( random >= 6001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else
	{
		if( random >= 1 && random <= 200)
			result = GREAT_SUCCESS;
		else if( random >= 201 && random <= 1000)
			result = SUCCESS;
		else if( random >= 1001 && random <= 5000)
			result = NORMAL;
		else if( random >= 5001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	
	return result;
}


void CUser::SendSystemMsg(TCHAR *pMsg, BYTE type, int nWho)
{
	int send_index = 0;
	char buff[1024];
	memset( buff, 0x00, 1024 );
	short sLength = _tcslen(pMsg);

	SetByte(buff, AG_SYSTEM_MSG, send_index );
	SetByte(buff, type, send_index );				// ä������
	SetShort(buff, nWho, send_index );				// ��������
	SetShort(buff, m_iUserId, send_index );
	SetShort(buff, sLength, send_index );
	SetString( buff, pMsg, sLength, send_index );

	SendAll(buff, send_index);   	
}

void CUser::InitNpcAttack()
{
	for(int i=0; i<8; i++)
	{
		m_sSurroundNpcNumber[i] = -1;
	}
}

int CUser::IsSurroundCheck(float fX, float fY, float fZ, int NpcID)
{
	int nDir = 0;
	__Vector3 vNpc, vUser, vDis;
	vNpc.Set(fX, fY, fZ);
	float fDX, fDZ;
	float fDis = 0.0f, fCurDis=1000.0f;
	BOOL bFlag = FALSE;
	for(int i=0; i<8; i++)
	{
		//if(m_sSurroundNpcNumber[i] != -1) continue;
		if(m_sSurroundNpcNumber[i] == NpcID)
		{
			if(bFlag == TRUE)
				m_sSurroundNpcNumber[i] = -1;
			else
			{
				m_sSurroundNpcNumber[i] = NpcID;
				nDir = i+1;
				bFlag = TRUE;
			}
			//return nDir;
		}

		if(m_sSurroundNpcNumber[i] == -1 && bFlag==FALSE)
		{
			fDX = m_curx + surround_fx[i]; 
			fDZ = m_curz + surround_fz[i]; 
			vUser.Set(fDX, 0.0f, fDZ);
			vDis = vUser - vNpc;
			fDis = vDis.Magnitude();
			if(fDis < fCurDis)
			{
				nDir = i+1;
				fCurDis = fDis;
			}
		}
	}

	
/*	TRACE("User-Sur : [0=%d,1=%d,2=%d,3=%d,4=%d,5=%d,6=%d,7=%d]\n", m_sSurroundNpcNumber[0], 
		m_sSurroundNpcNumber[1], m_sSurroundNpcNumber[2], m_sSurroundNpcNumber[3], m_sSurroundNpcNumber[4],
		m_sSurroundNpcNumber[5],m_sSurroundNpcNumber[6], m_sSurroundNpcNumber[7]);
	*/
	if(nDir != 0)
	{
		m_sSurroundNpcNumber[nDir-1] = NpcID;
	}

	return nDir;
}

BOOL CUser::IsOpIDCheck(char* szName)
{
/*	int nSize = sizeof(g_pszOPID)/sizeof(char*);
	CString szCheck = szName;
	CString szCheck2;

	szCheck.MakeLower();

	for (int i=0; i< nSize; i++) 
	{
		szCheck2 = g_pszOPID[i];
		szCheck2.MakeLower();

		if(szCheck.Find(szCheck2) != -1) return TRUE;
	}	*/

	return FALSE;
}

void CUser::HealMagic()
{
	int region_x = m_curx / VIEW_DIST;
	int region_z = m_curz / VIEW_DIST;

	if(m_sZoneIndex < 0 || m_sZoneIndex > m_pMain->g_arZone.size()) {
		TRACE("#### CUser--HealMagic ZoneIndex Fail : [name=%s], zoneindex=%d #####\n", m_strUserID, m_sZoneIndex);
		return;
	}

	MAP* pMap = m_pMain->g_arZone[m_sZoneIndex];
	if(pMap == NULL) return;
	int max_xx = pMap->m_sizeRegion.cx;
	int max_zz = pMap->m_sizeRegion.cy;

	int min_x = region_x - 1;	if(min_x < 0) min_x = 0;
	int min_z = region_z - 1;	if(min_z < 0) min_z = 0;
	int max_x = region_x + 1;	if(max_x >= max_xx) max_x = max_xx - 1;
	int max_z = region_z + 1;	if(min_z >= max_zz) min_z = max_zz - 1;

	int search_x = max_x - min_x + 1;		
	int search_z = max_z - min_z + 1;	
	
	int i, j;

	for(i = 0; i < search_x; i++)	{
		for(j = 0; j < search_z; j++)	{
			HealAreaCheck( min_x+i, min_z+j );
		}
	}
}

void CUser::HealAreaCheck(int rx, int rz)
{
	if(m_sZoneIndex < 0 || m_sZoneIndex > m_pMain->g_arZone.size()) {
		TRACE("#### CUser--HealAreaCheck ZoneIndex Fail : [name=%s], zoneindex=%d #####\n", m_strUserID, m_sZoneIndex);
		return;
	}

	MAP* pMap = m_pMain->g_arZone[m_sZoneIndex];
	if(pMap == NULL) return;
	// �ڽ��� region�� �ִ� NpcArray�� ���� �˻��Ͽ�,, ����� �Ÿ��� Monster�� �ִ����� �Ǵ�..
	if(rx < 0 || rz < 0 || rx > pMap->GetXRegionMax() || rz > pMap->GetZRegionMax())	{
		TRACE("#### CUser-HealAreaCheck() Fail : [nid=%d, name=%s], nRX=%d, nRZ=%d #####\n", m_iUserId, m_strUserID, rx, rz);
		return;
	}

	float fRadius = 10.0f;				// 30m

	__Vector3 vStart, vEnd;
	CNpc* pNpc = NULL ;      // Pointer initialization!
	float fDis = 0.0f;
	vStart.Set(m_curx, (float)0, m_curz);
	char send_buff[256];	memset(send_buff, 0x00, 256);
	int nid = 0, send_index=0, result = 1, count = 0, total_mon = 0; 
	int* pNpcIDList = NULL;

	EnterCriticalSection( &g_region_critical );
	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	Iter1 = pMap->m_ppRegion[rx][rz].m_RegionNpcArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_ppRegion[rx][rz].m_RegionNpcArray.m_UserTypeMap.end();

	total_mon = pMap->m_ppRegion[rx][rz].m_RegionNpcArray.GetSize();
	pNpcIDList = new int[total_mon];
	for( ; Iter1 != Iter2; Iter1++ ) {
		nid = *( (*Iter1).second );
		pNpcIDList[count] = nid;
		count++;
	}
	LeaveCriticalSection( &g_region_critical );

	for(int i = 0 ; i < total_mon; i++ ) {
		nid = pNpcIDList[i];
		if( nid < NPC_BAND ) continue;
		pNpc = (CNpc*)m_pMain->m_arNpc.GetData(nid - NPC_BAND);

		if( pNpc != NULL && pNpc->m_NpcState != NPC_DEAD)	{
			if( m_bNation == pNpc->m_byGroup ) continue;
			vEnd.Set(pNpc->m_fCurX, pNpc->m_fCurY, pNpc->m_fCurZ); 
			fDis = pNpc->GetDistance(vStart, vEnd);

			if(fDis <= fRadius)	{	// NPC�� �ݰ�ȿ� ���� ���...
				pNpc->ChangeTarget(1004, this, m_pIocport);
			}	
			else continue;
		}
	}

	if(pNpcIDList)	{
		delete [] pNpcIDList;
		pNpcIDList = NULL;
	}
}

void CUser::WriteUserLog()
{
	CString string;
	list<_USERLOG*>::iterator	Iter;

	for( Iter = m_UserLogList.begin(); Iter != m_UserLogList.end(); Iter++ ) {
		//string.ReleaseBuffer();
		if( (*Iter)->byFlag == USER_LOGIN )
			string.Format( "%d-%d-%d %d:%d, %s, %d, %s\r\n", (*Iter)->t.GetYear(), (*Iter)->t.GetMonth(), (*Iter)->t.GetDay(), (*Iter)->t.GetHour(), (*Iter)->t.GetMinute(), "LogIn", (*Iter)->byLevel, (*Iter)->strUserID);
		else if( (*Iter)->byFlag == USER_LOGOUT )
			string.Format( "%d-%d-%d %d:%d, %s, %d, %s\r\n", (*Iter)->t.GetYear(), (*Iter)->t.GetMonth(), (*Iter)->t.GetDay(), (*Iter)->t.GetHour(), (*Iter)->t.GetMinute(), "LogOut", (*Iter)->byLevel, (*Iter)->strUserID);
		else if( (*Iter)->byFlag == USER_LEVEL_UP )
			string.Format( "%d-%d-%d %d:%d, %s, %d, %s\r\n", (*Iter)->t.GetYear(), (*Iter)->t.GetMonth(), (*Iter)->t.GetDay(), (*Iter)->t.GetHour(), (*Iter)->t.GetMinute(), "LevelUp", (*Iter)->byLevel, (*Iter)->strUserID);
		EnterCriticalSection( &g_LogFileWrite );
		m_pMain->m_UserLogFile.Write(string, string.GetLength());
		LeaveCriticalSection( &g_LogFileWrite );
	}

	InitUserLog();
}

void CUser::InitUserLog()
{
	_USERLOG* pUserLog = NULL;

	while(m_UserLogList.size()) {
		pUserLog = m_UserLogList.front();
		if( pUserLog )
			delete pUserLog;
		m_UserLogList.pop_front();
	}
	m_UserLogList.clear();
}

