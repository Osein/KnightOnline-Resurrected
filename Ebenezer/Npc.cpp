// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ebenezer.h"
#include "Npc.h"
#include "EbenezerDlg.h"
#include "Map.h"
#include "PacketDefine.h"

extern CRITICAL_SECTION g_region_critical;

CNpc::CNpc()
{

}

CNpc::~CNpc()
{

}

void CNpc::Initialize()
{
	m_pMain = (CEbenezerDlg*)AfxGetApp()->GetMainWnd();

	m_sNid = -1;				// NPC (서버상의)일련번호
	m_sSid = 0;
	m_sZoneIndex = -1;			// Current Zone Index(배열)
	m_sCurZone = -1;			// Current Zone number
	m_fCurX = 0;			// Current X Pos;
	m_fCurY = 0;			// Current Y Pos;
	m_fCurZ = 0;			// Current Z Pos;
	m_sPid = 0;				// MONSTER(NPC) Picture ID
	m_sSize = 100;				// MONSTER(NPC) Size
	memset(m_strName, 0, MAX_ID_SIZE+1);;		// MONSTER(NPC) Name
	m_iMaxHP = 0;				// 최대 HP
	m_iHP = 0;					// 현재 HP
	//m_byState = 0;			// 몬스터 (NPC) 상태이상
	m_tNpcType = 0;				// NPC Type
								// 0 : Normal Monster
								// 1 : NPC
								// 2 : 각 입구,출구 NPC
								// 3 : 경비병
	m_byGroup = 0;
	m_byLevel = 0;
	m_iSellingGroup = 0;
//	m_dwStepDelay = 0;		

	m_sRegion_X = 0;			// region x position
	m_sRegion_Z = 0;			// region z position
	m_fDir = 0.0f;				// npc의 방향,,
	m_iWeapon_1 = 0;
	m_iWeapon_2 = 0;
	m_NpcState = NPC_LIVE;
	m_byGateOpen = 1;
	m_sHitRate = 0;
	m_byObjectType = NORMAL_OBJECT;

	m_byEvent = -1;				//  This is for the event.
}

void CNpc::MoveResult(float xpos, float ypos, float zpos, float speed)
{
	m_fCurX = xpos;
	m_fCurZ = zpos;
	m_fCurY = ypos;

	RegisterRegion();

	int send_index = 0;
	char pOutBuf[1024];
	memset(pOutBuf, 0, 1024);

	SetByte(pOutBuf, WIZ_NPC_MOVE, send_index);
	SetShort(pOutBuf, m_sNid, send_index);
	SetShort(pOutBuf, (WORD)m_fCurX*10, send_index);
	SetShort(pOutBuf, (WORD)m_fCurZ*10, send_index);
	SetShort(pOutBuf, (short)m_fCurY*10, send_index);
	SetShort(pOutBuf, (short)speed*10, send_index);

	m_pMain->Send_Region(pOutBuf, send_index, m_sCurZone, m_sRegion_X, m_sRegion_Z, NULL, false );
	//TRACE("RecvNpcMove ==> nid = %d, zone=%d, x = %f, z = %f\n", m_sNid, m_sCurZone, m_fCurX, m_fCurZ);
}

void CNpc::NpcInOut(BYTE Type, float fx, float fz, float fy)
{
	int send_index = 0;
	char buff[1024];
	memset( buff, 0x00, 1024 );

	C3DMap *pMap = m_pMain->m_ZoneArray[m_sZoneIndex];
	if( !pMap )	{
		TRACE("NpcInOut : ,, nid=%d, zone=%d, type, region_x=%d, y=%d\n", m_sNid, m_sZoneIndex, Type, m_sRegion_X, m_sRegion_Z);
		return;
	}

	if( Type == NPC_OUT ) {
		pMap->RegionNpcRemove( m_sRegion_X, m_sRegion_Z, m_sNid );
	}
	else	{
		pMap->RegionNpcAdd( m_sRegion_X, m_sRegion_Z, m_sNid );
		m_fCurX = fx;	m_fCurZ = fz;	m_fCurY = fy;
	}

	SetByte( buff, WIZ_NPC_INOUT, send_index );
	SetByte( buff, Type, send_index );
	SetShort( buff, m_sNid, send_index );
	if( Type == NPC_OUT ) {
		m_pMain->Send_Region(buff, send_index, m_sCurZone, m_sRegion_X, m_sRegion_Z);
		return;
	}
	SetShort( buff, m_sPid, send_index );
	SetByte( buff, m_tNpcType, send_index );
	SetDWORD( buff, m_iSellingGroup, send_index );
	SetShort( buff, m_sSize, send_index );
	SetDWORD( buff, m_iWeapon_1, send_index );
	SetDWORD( buff, m_iWeapon_2, send_index );
	SetShort( buff, strlen(m_strName), send_index );
	SetString( buff, m_strName, strlen(m_strName), send_index );
	SetByte( buff, m_byGroup, send_index );
	SetByte( buff, m_byLevel, send_index );
	SetShort( buff, (WORD)m_fCurX*10, send_index );
	SetShort( buff, (WORD)m_fCurZ*10, send_index );
	SetShort( buff, (short)m_fCurY*10, send_index );
	SetDWORD( buff, (int)m_byGateOpen, send_index );
	SetByte( buff, m_byObjectType, send_index );

	m_pMain->Send_Region(buff, send_index, m_sCurZone, m_sRegion_X, m_sRegion_Z);
}

void CNpc::RegisterRegion()
{
	int iRegX = 0, iRegZ = 0, old_region_x = 0, old_region_z = 0;
	iRegX = (int)(m_fCurX / VIEW_DISTANCE);
	iRegZ = (int)(m_fCurZ / VIEW_DISTANCE);

	if(m_sRegion_X != iRegX || m_sRegion_Z != iRegZ)
	{
		C3DMap* pMap = m_pMain->m_ZoneArray[m_sZoneIndex];
		if( !pMap )
			return;
		
		old_region_x = m_sRegion_X;	old_region_z = m_sRegion_Z;
		pMap->RegionNpcRemove(m_sRegion_X, m_sRegion_Z, m_sNid);
		m_sRegion_X = iRegX;		m_sRegion_Z = iRegZ;
		pMap->RegionNpcAdd(m_sRegion_X, m_sRegion_Z, m_sNid);

		RemoveRegion( old_region_x - m_sRegion_X, old_region_z - m_sRegion_Z );	// delete npc 는 계산 방향이 진행방향의 반대...
		InsertRegion( m_sRegion_X - old_region_x, m_sRegion_Z - old_region_z );	// add npc 는 계산 방향이 진행방향...
	}
}

void CNpc::RemoveRegion(int del_x, int del_z)
{
	int send_index = 0, i=0;
	int region_x = -1, region_z = -1, uid = -1;
	char buff[128];
	memset( buff, NULL, 128 );
	C3DMap* pMap = NULL;

	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_sZoneIndex];
	if( !pMap )
		return;

	SetByte( buff, WIZ_NPC_INOUT, send_index );
	SetByte( buff, NPC_OUT, send_index );
	SetShort( buff, m_sNid, send_index );
/*
	SetShort( buff, m_sPid, send_index );
	SetByte( buff, m_tNpcType, send_index );
	SetDWORD( buff, m_iSellingGroup, send_index );
	SetShort( buff, m_sSize, send_index );
	SetDWORD( buff, m_iWeapon_1, send_index );
	SetDWORD( buff, m_iWeapon_2, send_index );
	SetShort( buff, strlen(m_strName), send_index );
	SetString( buff, m_strName, strlen(m_strName), send_index );
	SetByte( buff, m_byGroup, send_index );
	SetByte( buff, m_byLevel, send_index );
	SetShort( buff, (WORD)m_fCurX*10, send_index );
	SetShort( buff, (WORD)m_fCurZ*10, send_index );
	SetShort( buff, (short)m_fCurY*10, send_index );
	SetDWORD( buff, (int)m_byGateOpen, send_index );
	SetByte( buff, m_byObjectType, send_index );
*/
	if( del_x != 0 ) {	// x 축으로 이동되었을때...
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x*2, m_sRegion_Z+del_z-1 );
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x*2, m_sRegion_Z+del_z );
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x*2, m_sRegion_Z+del_z+1 );
//		TRACE("Remove : (%d %d), (%d %d), (%d %d)\n", m_sRegion_X+del_x*2, m_sRegion_Z+del_z-1, m_sRegion_X+del_x*2, m_sRegion_Z+del_z, m_sRegion_X+del_x*2, m_sRegion_Z+del_z+1 );
	}
	if( del_z != 0 ) {	// z 축으로 이동되었을때...
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x, m_sRegion_Z+del_z*2 );
		if( del_x < 0 ) // x, z 축 둘다 이동되었을때 겹치는 부분 한번만 보낸다..
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x+1, m_sRegion_Z+del_z*2 );
		else if( del_x > 0 )
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x-1, m_sRegion_Z+del_z*2 );
		else {
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x-1, m_sRegion_Z+del_z*2 );
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x+1, m_sRegion_Z+del_z*2 );
//			TRACE("Remove : (%d %d), (%d %d), (%d %d)\n", m_sRegion_X+del_x-1, m_sRegion_Z+del_z*2, m_sRegion_X+del_x, m_sRegion_Z+del_z*2, m_sRegion_X+del_x+1, m_sRegion_Z+del_z*2 );
		}
	}
}

void CNpc::InsertRegion(int del_x, int del_z)
{
	int send_index = 0, buff_index = 0, i=0;
	int region_x = -1, region_z = -1;
	char buff[128];
	memset( buff, NULL, 128 );
	C3DMap* pMap = NULL;

	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_sZoneIndex];
	if( !pMap )
		return;

	SetByte( buff, WIZ_NPC_INOUT, send_index );
	SetByte( buff, NPC_IN, send_index );
	SetShort( buff, m_sNid, send_index );
	SetShort( buff, m_sPid, send_index );
	SetByte( buff, m_tNpcType, send_index );
	SetDWORD( buff, m_iSellingGroup, send_index );
	SetShort( buff, m_sSize, send_index );
	SetDWORD( buff, m_iWeapon_1, send_index );
	SetDWORD( buff, m_iWeapon_2, send_index );
	SetShort( buff, strlen(m_strName), send_index );
	SetString( buff, m_strName, strlen(m_strName), send_index );
	SetByte( buff, m_byGroup, send_index );
	SetByte( buff, m_byLevel, send_index );
	SetShort( buff, (WORD)m_fCurX*10, send_index );
	SetShort( buff, (WORD)m_fCurZ*10, send_index );
	SetShort( buff, (short)m_fCurY*10, send_index );
	SetDWORD( buff, (int)m_byGateOpen, send_index );
	SetByte( buff, m_byObjectType, send_index );

	if( del_x != 0 ) {	// x 축으로 이동되었을때...
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x, m_sRegion_Z-1 );
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x, m_sRegion_Z );
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+del_x, m_sRegion_Z+1 );
//		TRACE("Insert : (%d %d), (%d %d), (%d %d)\n", m_sRegion_X+del_x, m_sRegion_Z-1, m_sRegion_X+del_x, m_sRegion_Z, m_sRegion_X+del_x, m_sRegion_Z+1 );
	}
	if( del_z != 0 ) {	// z 축으로 이동되었을때...
		m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X, m_sRegion_Z+del_z );
		
		if( del_x < 0 )	{// x, z 축 둘다 이동되었을때 겹치는 부분 한번만 보낸다..
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+1, m_sRegion_Z+del_z );
		}
		else if( del_x > 0 ) {
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X-1, m_sRegion_Z+del_z );
		}
		else {
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X-1, m_sRegion_Z+del_z );
			m_pMain->Send_UnitRegion( buff, send_index, m_sZoneIndex, m_sRegion_X+1, m_sRegion_Z+del_z );

//			TRACE("Insert : (%d %d), (%d %d), (%d %d)\n", m_sRegion_X-1, m_sRegion_Z+del_z, m_sRegion_X, m_sRegion_Z+del_z, m_sRegion_X+1, m_sRegion_Z+del_z );
		}
	}
}

int CNpc::GetRegionNpcList(int region_x, int region_z, char *buff, int &t_count)
{
	if( m_pMain->m_bPointCheckFlag == FALSE)	return 0;	// 포인터 참조하면 안됨

	int buff_index = 0, i=0, j=0;
	int user_count = 0, nid = -1;
	C3DMap* pMap = NULL;
	CNpc* pNpc = NULL;
	
	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_sZoneIndex];
	if( !pMap )
		return 0;
	if( region_x<0 || region_z<0 || region_x>pMap->GetXRegionMax() || region_z>pMap->GetZRegionMax() )
		return 0;

	EnterCriticalSection( &g_region_critical );

	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;
	
	Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.end();

	for( ; Iter1 != Iter2; Iter1++ ) {
		nid = *( (*Iter1).second );
		if( nid < 0 )
			continue;
		pNpc = (CNpc*)m_pMain->m_arNpcArray.GetData(nid);
		if( pNpc) {
			SetShort( buff, pNpc->m_sNid, buff_index );
			t_count++;
		}
	}

	LeaveCriticalSection( &g_region_critical );

	return buff_index;
}