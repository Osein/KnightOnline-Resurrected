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
	int index = 0, temp_rear = 0;

	if( size > m_wOffset ) {
		sprintf( logstr, "DataSize Over.. - %d bytes\r\n", size );
		LogFileWrite( logstr );
		return SMQ_PKTSIZEOVER;
	}

	if( m_pHeader->RearMode == W )
		return SMQ_WRITING;

	m_pHeader->RearMode = W;
	m_pHeader->WritePid = _getpid();

	aa();	// no operation function

	if( m_pHeader->WritePid != _getpid() ) {
		m_pHeader->RearMode = WR;
		return SMQ_WRITING;
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

	temp_rear = (m_pHeader->Rear + 1) % MAX_COUNT;
	m_pHeader->Rear = temp_rear;

	m_pHeader->RearMode = WR;

//	TRACE("Queue Writing : %d %d %x\n", m_pHeader->nCount, m_pHeader->Rear, m_pHeader);

	return 1;
}

int CSharedMemQueue::GetData( char* pBuf )
{
	int index = 0, size = 0, temp_front = 0;
	BYTE BlockMode;
	
	if( m_pHeader->FrontMode == R ) 
		return SMQ_READING;
	
//	if( m_pHeader->Front >= m_pHeader->Rear )
//		return SMQ_EMPTY;

	m_pHeader->FrontMode = R;
	m_pHeader->ReadPid = _getpid();

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
		return SMQ_EMPTY;
	}

	size = GetShort( (char*)pQueue, index );
	GetString( pBuf, (char*)pQueue, size, index );

	m_pHeader->nCount--;

	temp_front = (m_pHeader->Front + 1) % MAX_COUNT;
	m_pHeader->Front = temp_front;

	memset( (void*)pQueue, 0x00, m_wOffset );

	m_pHeader->FrontMode = WR;

//	TRACE("Queue Reading : %d %d %x\n", m_pHeader->nCount, m_pHeader->Front, m_pHeader);

	return size;
}
