// RoomEvent.cpp: implementation of the CRoomEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "RoomEvent.h"
#include "ServerDlg.h"
#include "define.h"
//#include "Npc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CRITICAL_SECTION g_region_critical;

CRoomEvent::CRoomEvent()
{
	m_iZoneNumber = 0;
	m_sRoomNumber = 0;
	m_byStatus	= 1;
	m_iInitMinX = 0;
	m_iInitMinZ = 0;
	m_iInitMaxX = 0;
	m_iInitMaxZ = 0;
	m_iEndMinX = 0;
	m_iEndMinZ = 0;
	m_iEndMaxX = 0;
	m_iEndMaxZ = 0;
	m_byCheck = 0;
	m_byRoomType = 0;
	m_pMain = (CServerDlg*) AfxGetMainWnd();

	Initialize();
}

CRoomEvent::~CRoomEvent()
{
	if( !m_mapRoomNpcArray.IsEmpty() )	
		m_mapRoomNpcArray.DeleteAllData();
}

void CRoomEvent::Initialize()
{
	m_fDelayTime = 0.0f;
	m_byLogicNumber = 1;

	for(int i=0; i<MAX_CHECK_EVENT; i++)	{
		m_Logic[i].sNumber = 0;
		m_Logic[i].sOption_1 = 0;
		m_Logic[i].sOption_2 = 0;
		m_Exec[i].sNumber = 0;
		m_Exec[i].sOption_1 = 0;
		m_Exec[i].sOption_2 = 0;
	}
}

void CRoomEvent::MainRoom( float fcurtime )
{
	// ���� �˻����� �ؾ� ����..
	BOOL bCheck = FALSE, bRunCheck = FALSE;
	char notify[50];	memset(notify, 0x00, 50);

	int event_num  = m_Logic[m_byLogicNumber-1].sNumber;

	bCheck = CheckEvent( event_num, fcurtime );

	if( bCheck )	{
		event_num = m_Exec[m_byLogicNumber-1].sNumber; 
		bRunCheck = RunEvent( event_num );
		if( bRunCheck )	{
			//wsprintf(notify, "** �˸� : [%d]���� Ŭ���� �Ǿ���ϴ�. **", m_sRoomNumber);
			//m_pMain->SendSystemMsg( notify, m_iZoneNumber, PUBLIC_CHAT, SEND_ALL);
			m_byStatus = 3;
		}
	}
}

BOOL  CRoomEvent::CheckEvent( int event_num, float fcurtime )
{
	int nMinute = 0, nOption_1 = 0, nOption_2 = 0;
	CNpc* pNpc = NULL;
	BOOL bRetValue = FALSE;

	if( m_byLogicNumber == 0 || m_byLogicNumber > MAX_CHECK_EVENT )	{
		TRACE("### Check Event Fail :: array overflow = %d ###\n", m_byLogicNumber);
		return FALSE;
	}

	switch( event_num )	{
	case 1:					// Ư�� ���͸� ���̴� ���
		nOption_1 = m_Logic[ m_byLogicNumber-1 ].sOption_1;
		pNpc = GetNpcPtr( nOption_1 );
		if( pNpc )	{
			if( pNpc->m_byChangeType == 100 )	return TRUE;
		}
		else	{
			TRACE("### CheckEvent Error : monster nid = %d, logic=%d ###\n", nOption_1, m_byLogicNumber);
		}
		//TRACE("---Check Event : monster dead = %d \n", nMonsterNid);
		break;
	case 2:					// ��� ���͸� �׿���
		bRetValue = CheckMonsterCount( 0, 0, 3 );
		if( bRetValue )	{
			TRACE("��� ���͸� �׿��� ����\n");
			return TRUE;
		}
		break;
	case 3:					// ��е��� ���߶�
		nMinute = m_Logic[ m_byLogicNumber-1 ].sOption_1;
		nMinute = nMinute * 60;								// ���� �ʷ� ��ȯ
		if( fcurtime >= m_fDelayTime + nMinute )	{		// ���ѽð� ����
			TRACE("---Check Event : curtime=%.2f, starttime=%.2f \n", fcurtime, m_fDelayTime);
			TRACE("��Ƽ�� ����\n");
			return TRUE;
		}
		//TRACE("---Check Event : curtime=%.2f, starttime=%.2f \n", fcurtime, m_fDelayTime);
		break;
	case 4:					// ��ǥ�������� �̵�
		
		break;
	case 5:					// Ư�����͸� �ɼ�2�� ������ ��ŭ �׿���
		nOption_1 = m_Logic[ m_byLogicNumber-1 ].sOption_1;
		nOption_2 = m_Logic[ m_byLogicNumber-1 ].sOption_2;
		bRetValue = CheckMonsterCount( nOption_1, nOption_2, 1 );
		if( bRetValue )	{
			TRACE("Ư������(%d)�� %d���� ����\n", nOption_1, nOption_2);
			return TRUE;
		}
		break;
	default:
		TRACE("### Check Event Fail :: event number = %d ###\n", event_num);
		break;
	}

	return FALSE;
}

BOOL  CRoomEvent::RunEvent( int event_num )
{
	char notify[50];	memset(notify, 0x00, 50);
	CNpc* pNpc = NULL;
	int nOption_1 = 0, nOption_2 = 0;
	BOOL bRetValue = FALSE;
	switch( event_num )	{
	case 1:					// �ٸ� ������ ����
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		pNpc = GetNpcPtr( nOption_1 );
		if( pNpc )	{
			pNpc->m_byChangeType = 3;	// ���� �������ּ���...
			pNpc->SetLive( &m_pMain->m_Iocport );
		}
		else	{
			TRACE("### RunEvent Error : ���� ���� �� �� ���� = %d, logic=%d ###\n", nOption_1, m_byLogicNumber);
		}
		if( m_byCheck == m_byLogicNumber )	{	// ���� Ŭ����
			return TRUE;
		}
		else		m_byLogicNumber++;

		break;
	case 2:					// ���� ����
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		pNpc = GetNpcPtr( nOption_1 );
		if( pNpc )	{
			
		}
		else	{
			TRACE("### RunEvent Error : �� ��� ���� ���� �� �� ���� = %d, logic=%d ###\n", nOption_1, m_byLogicNumber);
		}

		//wsprintf(notify, "** �˸� : [%d] ���� �����ϴ� **", m_sRoomNumber);
		//m_pMain->SendSystemMsg( notify, m_iZoneNumber, PUBLIC_CHAT, SEND_ALL);

		if( m_byCheck == m_byLogicNumber )	{	// ���� Ŭ����
			return TRUE;
		}
		else		m_byLogicNumber++;

		break;
	case 3:					// �ٸ� ���ͷ� ��ȯ
		if( m_byCheck == m_byLogicNumber )	{	// ���� Ŭ����
			return TRUE;
		}
		break;
	case 4:					// Ư������ �ɼ�2�� ��������ŭ ����
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		nOption_2 = m_Exec[ m_byLogicNumber-1 ].sOption_2;
		bRetValue = CheckMonsterCount( nOption_1, nOption_2, 2 );

		//wsprintf(notify, "** �˸� : [%d, %d] ���� ���� **", nOption_1, nOption_2);
		//m_pMain->SendSystemMsg( notify, m_iZoneNumber, PUBLIC_CHAT, SEND_ALL);

		if( m_byCheck == m_byLogicNumber )	{	// ���� Ŭ����
			return TRUE;
		}
		else		m_byLogicNumber++;
		break;
	case 100:					// Ư������ �ɼ�2�� ��������ŭ ����
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		nOption_2 = m_Exec[ m_byLogicNumber-1 ].sOption_2;

		TRACE("RunEvent - room=%d, option1=%d, option2=%d\n", m_sRoomNumber, nOption_1, nOption_2);
		if( nOption_1 != 0 )	{
			EndEventSay( nOption_1, nOption_2 );
		}
		if( m_byCheck == m_byLogicNumber )	{	// ���� Ŭ����
			return TRUE;
		}
		else		m_byLogicNumber++;
		break;
	default:
		TRACE("### RunEvent Fail :: event number = %d ###\n", event_num);
		break;
	}

	return FALSE;
}

CNpc* CRoomEvent::GetNpcPtr( int sid )
{
	CNpc* pNpc = NULL;
	int* pIDList = NULL;
	int nMonsterid = 0, count = 0;
	
	EnterCriticalSection( &g_region_critical );
	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	int nMonster = m_mapRoomNpcArray.GetSize();
	if( nMonster == 0 )	{
		TRACE("### RoomEvent-GetNpcPtr() : monster empty ###\n");
		LeaveCriticalSection( &g_region_critical );
		return NULL;
	}
	
	Iter1 = m_mapRoomNpcArray.m_UserTypeMap.begin();
	Iter2 = m_mapRoomNpcArray.m_UserTypeMap.end();

	pIDList = new int[nMonster];
	for( ; Iter1 != Iter2; Iter1++ ) {
		nMonsterid = *( (*Iter1).second );
		pIDList[count] = nMonsterid;
		count++;
	}
	LeaveCriticalSection( &g_region_critical );

	for(int i=0 ; i<nMonster; i++ ) {
		nMonsterid = pIDList[i];
		if( nMonsterid < 0 )	continue;
		pNpc = m_pMain->m_arNpc.GetData( nMonsterid );
		if( !pNpc )		continue;
		if( pNpc->m_sSid == sid )	{
			if(pIDList)	{
				delete [] pIDList;
				pIDList = NULL;
			}
			return pNpc;
		}
	}

	if(pIDList)	{
		delete [] pIDList;
		pIDList = NULL;
	}

	return NULL;
}

BOOL  CRoomEvent::CheckMonsterCount( int sid, int count, int type )
{
	int nMonsterCount = 0;
	CNpc* pNpc = NULL;
	int* pIDList = NULL;
	int nMonsterid = 0, nTotalMonster = 0;
	BOOL bRetValue = FALSE;
	
	EnterCriticalSection( &g_region_critical );
	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	int nMonster = m_mapRoomNpcArray.GetSize();
	if( nMonster == 0 )	{
		TRACE("### RoomEvent-GetNpcPtr() : monster empty ###\n");
		LeaveCriticalSection( &g_region_critical );
		return NULL;
	}
	
	Iter1 = m_mapRoomNpcArray.m_UserTypeMap.begin();
	Iter2 = m_mapRoomNpcArray.m_UserTypeMap.end();

	pIDList = new int[nMonster];
	for( ; Iter1 != Iter2; Iter1++ ) {
		nMonsterid = *( (*Iter1).second );
		pIDList[nTotalMonster] = nMonsterid;
		nTotalMonster++;
	}
	LeaveCriticalSection( &g_region_critical );

	for(int i=0 ; i<nMonster; i++ ) {
		nMonsterid = pIDList[i];
		if( nMonsterid < 0 )	continue;
		pNpc = m_pMain->m_arNpc.GetData( nMonsterid );
		if( !pNpc )		continue;
		if( type == 4 )	{
			if( pNpc->m_byRegenType == 2 )	pNpc->m_byRegenType = 0;
			pNpc->m_byChangeType = 0;
		}
		else if( type == 3 )	{				// ��� ���͸� �׾������� �Ǵ�
			if( pNpc->m_byDeadType == 100 )	nMonsterCount++;
			if( nMonsterCount == nMonster )	bRetValue = TRUE;
		}
		else	if( pNpc->m_sSid == sid )	{
			if( type == 1 )	{					// Ư�� ���Ͱ� ������ ��ŭ �׾������� �Ǵ�
				if( pNpc->m_byChangeType == 100 )	nMonsterCount++;
				if( nMonsterCount == count )	bRetValue = TRUE;
			}
			else if( type == 2 )	{			// Ư�� ���͸� ������ ��ŭ ���� ���Ѷ�,,
				pNpc->m_byChangeType = 3;	nMonsterCount++;
				if( nMonsterCount == count )	bRetValue = TRUE;
			}
		}
	}

	if(pIDList)	{
		delete [] pIDList;
		pIDList = NULL;
	}

	return bRetValue;
}

void CRoomEvent::InitializeRoom()
{
	m_byStatus	= 1;			
	m_fDelayTime = 0.0f;
	m_byLogicNumber = 1;

	CheckMonsterCount( 0, 0, 4);	// ������ m_byChangeType=0���� �ʱ�ȭ 
}

void CRoomEvent::EndEventSay( int option1, int option2 )
{
	char notify[256];		memset(notify, 0x00, 256);
	char send_buff[128];	memset(send_buff, 0x00, 128);
	int send_index = 0;

	std::string buff;

	switch( option1 )	{
	case 1:							// Ŭ���� ���¿��� Ŭ���̾�Ʈ�� ������ ����
		switch( option2 )	{
		case 1:
			::_LoadStringFromResource(IDS_KARUS_CATCH_1, buff);
			sprintf( notify, buff.c_str());
			break;
		case 2:
			::_LoadStringFromResource(IDS_KARUS_CATCH_2, buff);
			sprintf( notify, buff.c_str());
			break;
		case 11:
			::_LoadStringFromResource(IDS_ELMORAD_CATCH_1, buff);
			sprintf( notify, buff.c_str());
			break;
		case 12:
			::_LoadStringFromResource(IDS_ELMORAD_CATCH_2, buff);
			sprintf( notify, buff.c_str());
			break;
		}

		m_pMain->SendSystemMsg( notify, m_iZoneNumber, WAR_SYSTEM_CHAT, SEND_ALL);

		break;
	case 2:							// Ŭ���� ���¿��� Ŭ���̾�Ʈ�� ������ ����� �������� �� �� �ִ� �̺�Ʈ �� �����ֱ�
		if( option2 == KARUS_ZONE )	{
			::_LoadStringFromResource(IDS_KARUS_PATHWAY, buff);
			sprintf( notify, buff.c_str());

			SetByte( send_buff, AG_BATTLE_EVENT, send_index );
			SetByte( send_buff, BATTLE_MAP_EVENT_RESULT, send_index );
			SetByte( send_buff, KARUS_ZONE, send_index );
			m_pMain->Send( send_buff, send_index, m_iZoneNumber );
		}
		else if( option2 == ELMORAD_ZONE )	{
			::_LoadStringFromResource(IDS_ELMORAD_PATHWAY, buff);
			sprintf( notify, buff.c_str());

			SetByte( send_buff, AG_BATTLE_EVENT, send_index );
			SetByte( send_buff, BATTLE_MAP_EVENT_RESULT, send_index );
			SetByte( send_buff, ELMORAD_ZONE, send_index );
			m_pMain->Send( send_buff, send_index, m_iZoneNumber );
		}

		m_pMain->SendSystemMsg( notify, m_iZoneNumber, WAR_SYSTEM_CHAT, SEND_ALL);

		break;
	case 3:							// Ŭ���� ���¿��� Ŭ���̾�Ʈ�� ������ ����� �¸����� �˷��ش�.
		if( option2 == KARUS_ZONE )	{
			SetByte( send_buff, AG_BATTLE_EVENT, send_index );
			SetByte( send_buff, BATTLE_EVENT_RESULT, send_index );
			SetByte( send_buff, KARUS_ZONE, send_index );
			m_pMain->Send( send_buff, send_index, m_iZoneNumber );
		}
		else if( option2 == ELMORAD_ZONE )	{
			SetByte( send_buff, AG_BATTLE_EVENT, send_index );
			SetByte( send_buff, BATTLE_EVENT_RESULT, send_index );
			SetByte( send_buff, ELMORAD_ZONE, send_index );
			m_pMain->Send( send_buff, send_index, m_iZoneNumber );
		}
		break;
	}
}
