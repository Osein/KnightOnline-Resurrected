// RoomEvent.h: interface for the CRoomEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROOMEVENT_H__70001CA8_64B8_422F_B0F4_ED0F2FA95E0E__INCLUDED_)
#define AFX_ROOMEVENT_H__70001CA8_64B8_422F_B0F4_ED0F2FA95E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "STLMap.h"

#define MAX_CHECK_EVENT	5

typedef CSTLMap <int>			mapNpcArray;

struct _RoomEvent
{
	short	sNumber;			// ��ɾ�, ���ǹ� ��ȣ
	short	sOption_1;			// option 1 (������ ��ȣ�� �ַ� ������ ����)
	short	sOption_2;			// option 2 (������ ������)
};

class CNpc;
class CServerDlg;

class CRoomEvent  
{
public:
	int     m_iZoneNumber;		// zone number
	short	m_sRoomNumber;		// room number (0:empty room)
	BYTE	m_byStatus;			// room status (1:init, 2:progress, 3:clear)
	BYTE	m_byCheck;			// ���ǹ��� ����
	BYTE	m_byRoomType;		// ���� Ÿ��(0:�Ϲ�, 1:������, 2:,,,,)

	int		m_iInitMinX;		// room region x
	int		m_iInitMinZ;
	int		m_iInitMaxX;
	int		m_iInitMaxZ;

	int		m_iEndMinX;			// room end region x ��������,,
	int		m_iEndMinZ;
	int		m_iEndMaxX;
	int		m_iEndMaxZ;

	_RoomEvent  m_Logic[MAX_CHECK_EVENT];		// ���ǵ�
	_RoomEvent  m_Exec[MAX_CHECK_EVENT];		// ���๮

	float   m_fDelayTime;						// time

	mapNpcArray	m_mapRoomNpcArray;				// room npc uid array
	CServerDlg* m_pMain;

private:
	BYTE    m_byLogicNumber;	// ������ ���ǹ� �˻� ��ȣ (���ǹ�ȣ�� 1���� ���۵�) (m_byCheck�� m_byLogicNumber�� ���ٸ� Ŭ���� ����)


public:
	CRoomEvent();
	virtual ~CRoomEvent();

	void MainRoom( float fcurtime );
	void InitializeRoom();

private:
	void Initialize();
	BOOL  CheckEvent( int event_num, float fcurtime );
	BOOL  RunEvent( int event_num );
	BOOL  CheckMonsterCount( int sid, int count, int type );
	CNpc* GetNpcPtr( int sid );
	void  EndEventSay( int option1, int option2 );

};

#endif // !defined(AFX_ROOMEVENT_H__70001CA8_64B8_422F_B0F4_ED0F2FA95E0E__INCLUDED_)
