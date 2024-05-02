// SharedMem.cpp: implementation of the CSharedMemQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SharedMem.h"
#include <process.h>

void aa() {};		// nop function

CSharedMemQueue::CSharedMemQueue()
{
	m_hMMFile = NULL;
	m_lpMMFile = NULL;
	m_bMMFCreate = FALSE;
	m_nMaxCount = 0;
	m_wOffset = 0;
	m_pHeader = NULL;
}

CSharedMemQueue::~CSharedMemQueue()
{
	if( m_lpMMFile )
		UnmapViewOfFile(m_lpMMFile);
	if( m_hMMFile )
		CloseHandle(m_hMMFile);
}

BOOL CSharedMemQueue::InitailizeMMF(DWORD dwOffsetsize, int maxcount, LPCTSTR lpname, BOOL bCreate )
{
	char logstr[256];
	memset( logstr, 0x00, 256);
	if( maxcount < 1 )
		return FALSE;
	DWORD dwfullsize = dwOffsetsize * maxcount + sizeof(_SMQ_HEADER);

	m_nMaxCount = maxcount;
	m_wOffset = dwOffsetsize;

	if( bCreate )
		m_hMMFile = CreateFileMapping( (HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, dwfullsize, lpname );
	else
		m_hMMFile = OpenFileMapping( FILE_MAP_ALL_ACCESS, TRUE, lpname );
	
	if( m_hMMFile == NULL ) {
		strcpy( logstr , "Shared Memory Open Fail!!\r\n" );
		LogFileWrite( logstr );
		return FALSE;
	}

    m_lpMMFile = (char *)MapViewOfFile(m_hMMFile, FILE_MAP_WRITE, 0, 0, 0);
	if( !m_lpMMFile )
		return FALSE;
	TRACE("%s Address : %x\n", lpname, m_lpMMFile);

	m_bMMFCreate = bCreate;
	m_pHeader = (_SMQ_HEADER *)m_lpMMFile;
	m_lReference = (LONG)(m_lpMMFile + sizeof(_SMQ_HEADER));		// 초기 위치 셋팅

	if( bCreate ) {
		memset( m_lpMMFile, 0x00, dwfullsize );
		m_pHeader->Rear = m_pHeader->Front = 0;
		m_pHeader->nCount = 0;
		m_pHeader->RearMode = m_pHeader->FrontMode = E;
		m_pHeader->CreatePid = _getpid();
	}

	return TRUE;
}

int CSharedMemQueue::PutData(char *pBuf, int size)
{
	char logstr[256];
	memset( logstr, 0x00, 256);
	BYTE BlockMode;
	int index = 0, count = 0;

	if( size > m_wOffset ) {
		sprintf( logstr, "DataSize Over.. - %d bytes\r\n", size );
		LogFileWrite( logstr );
		return SMQ_PKTSIZEOVER;
	}

	do {
		if( m_pHeader->RearMode == W ) {
			aa();
			count++;
			continue;
		}

		m_pHeader->RearMode = W;
		m_pHeader->WritePid = ::GetCurrentThreadId();	// writing side (game server) is multi thread

		aa();	// no operation function

		if( m_pHeader->WritePid != ::GetCurrentThreadId() ) {
			count++;
			continue;
		}

		LONG pQueue = m_lReference + (m_pHeader->Rear * m_wOffset);
		BlockMode = GetByte( (char*)pQueue, index );
		if( BlockMode == WR && m_pHeader->nCount >= MAX_COUNT-1 ) {
			m_pHeader->RearMode = WR;
			return SMQ_FULL;
		}

		index = 0;
		SetByte( (char*)pQueue, WR, index );	// Block Mode Set to WR	-> Data Exist
		SetShort( (char*)pQueue, size, index );
		SetString( (char*)pQueue, pBuf, size, index );

		m_pHeader->nCount++;

		m_pHeader->Rear = (m_pHeader->Rear + 1) % MAX_COUNT;
		m_pHeader->RearMode = WR;
		
		break;

	} while( count < 50 );
	if( count >= 50 ) {
		m_pHeader->RearMode = WR;
		return SMQ_WRITING;
	}

	return 1;
}

int CSharedMemQueue::GetData( char* pBuf )
{
	int index = 0, size = 0, temp_front = 0;
	BYTE BlockMode;
	
	if( m_pHeader->FrontMode == R ) 
		return SMQ_READING;

	m_pHeader->FrontMode = R;
	m_pHeader->ReadPid = _getpid();	// reading side ( agent ) is multi process ( one process -> act each single thread )

	aa();	// no operation function
	
	if( m_pHeader->ReadPid != _getpid() ) {
		m_pHeader->FrontMode = WR;
		return SMQ_READING;
	}

	LONG pQueue = m_lReference + (m_pHeader->Front * m_wOffset);

	index = 0;
	BlockMode = GetByte( (char *)pQueue, index );
	if( BlockMode == E ) {
		m_pHeader->FrontMode = WR;
		if( m_pHeader->Front < m_pHeader->Rear || (m_pHeader->Front > m_pHeader->Rear && m_pHeader->Front > MAX_COUNT-100) ) {
			temp_front = (m_pHeader->Front + 1) % MAX_COUNT;
			m_pHeader->Front = temp_front;
			m_pHeader->nCount--;
			char logstr[256]; memset( logstr, 0x00, 256 );
			sprintf( logstr, "SMQ EMPTY Block Find - F:%d, R:%d\n", m_pHeader->Front, m_pHeader->Rear);
			LogFileWrite( logstr );
			TRACE(logstr);
		}
		return SMQ_EMPTY;
	}

	size = GetShort( (char*)pQueue, index );
	GetString( pBuf, (char*)pQueue, size, index );

	m_pHeader->nCount--;

	temp_front = (m_pHeader->Front + 1) % MAX_COUNT;
	m_pHeader->Front = temp_front;

	memset( (void*)pQueue, 0x00, m_wOffset );

	m_pHeader->FrontMode = WR;

	return size;
}
