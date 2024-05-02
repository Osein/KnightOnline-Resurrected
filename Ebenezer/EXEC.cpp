// EXEC.cpp: implementation of the EXEC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ebenezer.h"
#include "Define.h"
#include "EXEC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EXEC::EXEC()
{

}

EXEC::~EXEC()
{

}

void EXEC::Parse(char *pBuf)
{
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace( temp, pBuf+index );


	if( !strcmp( temp, "SAY" ) )
	{
		m_Exec = EXEC_SAY;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ1
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ2
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ3
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ4
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ5
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ6
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ7
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ8
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ9
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ10
	}
	else if( !strcmp( temp, "SELECT_MSG" ) )
	{
		m_Exec = EXEC_SELECT_MSG;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ����
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���� ��ȣ

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 1
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 1

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 2
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 2

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 3
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 3

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 4
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 4

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 5
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 5
// �񷯸ӱ� ����Ʈ >.<
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 6
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 6

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 7
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 7

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 8
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 8

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 9
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 9

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// ���ù� 10
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );		// �̺�Ʈ 10		
//
	}
	else if( !strcmp( temp, "RUN_EVENT" ) )
	{
		m_Exec = EXEC_RUN_EVENT;
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "GIVE_ITEM" ) )
	{
		m_Exec = EXEC_GIVE_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Item no
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Item count
	}
	else if( !strcmp( temp, "ROB_ITEM" ) )
	{
		m_Exec = EXEC_ROB_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Item no.
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Item count
	}
// �񷯸ӱ� ����Ʈ >.<
	else if( !strcmp( temp, "OPEN_EDITBOX") ) 
	{
		m_Exec = EXEC_OPEN_EDITBOX;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Npc id.
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Input message.
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Next Event.		
	}
	else if( !strcmp( temp, "GIVE_NOAH") ) 
	{
		m_Exec = EXEC_GIVE_NOAH;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Amount of Noah
	}
	else if( !strcmp(temp, "LOG_COUPON_ITEM"))
	{
		m_Exec = EXEC_LOG_COUPON_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Item ID.
		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Number of Item.
	}
//
	else if( !strcmp( temp, "RETURN" ) )
	{
		m_Exec = EXEC_RETURN;
	}

	// ���� ��������� ���Դϴ�. �Ʒ��� ���߿� �°� �����ؼ� ������.


/*
	else if( !strcmp( temp, "ROB_NOAH" ) )
	{
		m_Exec = EXEC_ROB_NOAH;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// NOAH
	}
	else if( !strcmp( temp, "GIVE_QUEST" ) )
	{
		m_Exec = EXEC_GIVE_QUEST;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Quest no.
	}
	else if( !strcmp( temp, "DONE_QUEST" ) )
	{
		m_Exec = EXEC_QUEST_END;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Quest no.
	}
	else if( !strcmp( temp, "SAVE_QUEST" ) )
	{
		m_Exec = EXEC_QUEST_SAVE;

		index += ParseSpace( temp, pBuf+index );	m_ExecInt[i++] = atoi( temp );	// Quest no.
	}
*/
}

void EXEC::Init()
{
	for( int i = 0; i < MAX_EXEC_INT; i++)
	{
		m_ExecInt[i] = -1;
	}
}
