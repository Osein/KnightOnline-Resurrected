// IOCPSocket2.h: interface for the CIOCPSocket2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPSOCKET2_H__36499609_63DD_459C_B4D0_1686FEEC67C2__INCLUDED_)
#define AFX_IOCPSOCKET2_H__36499609_63DD_459C_B4D0_1686FEEC67C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCPort.h"
#include "Define.h"
// Cryption
#include "JvCryption.h"
///~


#define receives				0
#define sends					1
#define both					2 

class CCircularBuffer;
class CCompressMng;

class CIOCPSocket2  
{
public:
	void SendCryptionKey();
	void RegioinPacketClear( char* GetBuf, int & len );
	void RegionPacketAdd( char* pBuf, int len );
	void SendCompressingPacket( const char* pData, int len );
	void InitSocket( CIOCPort* pIOCPort );
	void Close();
	BOOL AsyncSelect( long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE );
	BOOL SetSockOpt( int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET );
	BOOL ShutDown( int nHow = sends );
	BOOL PullOutCore(char *&data, int &length);
	void ReceivedData(int length);
	int  Receive();
	int  Send(char *pBuf, long length, int dwFlag=0);
	BOOL Connect( CIOCPort* pIocp, LPCTSTR lpszHostAddress, UINT nHostPort );
	BOOL Create( UINT nSocketPort = 0,
				 int nSocketType = SOCK_STREAM, 
				 long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
				 LPCTSTR lpszSocketAddress = NULL );
	BOOL Accept( SOCKET listensocket, struct sockaddr* addr, int* len );
	int	 GetSocketID() {return m_Sid;};
	void SetSocketID(int sid) { m_Sid = sid;};
	HANDLE GetSocketHandle() {return (HANDLE)m_Socket;};
	BYTE GetState() {return m_State;};
	BYTE GetSockType() {return m_Type;};

	virtual void CloseProcess();
	virtual void Parsing( int length, char* pData );
	virtual void Initialize();

	CIOCPSocket2();
	virtual ~CIOCPSocket2();

	short			m_nSocketErr;
	short			m_nPending;
	short			m_nWouldblock;
	_REGION_BUFFER*	m_pRegionBuffer;

protected:
	CCompressMng*	m_pCompressMng;
	CIOCPort* m_pIOCPort;
	CCircularBuffer*	m_pBuffer;

	SOCKET				m_Socket;

	char				m_pRecvBuff[MAX_PACKET_SIZE];
	char				m_pSendBuff[MAX_PACKET_SIZE];

	HANDLE				m_hSockEvent;

	OVERLAPPED		m_RecvOverlapped;
	OVERLAPPED		m_SendOverlapped;

	BYTE			m_Type;
	BYTE			m_State;
	int			m_Sid;
	LPCTSTR		m_ConnectAddress;

	// Cryption
	CJvCryption			jct;
	int					m_CryptionFlag;
	T_KEY				m_Public_key;
	DWORD				m_Sen_val;
	DWORD				m_Rec_val;
	///~

	DWORD		m_wPacketSerial;
};

#endif // !defined(AFX_IOCPSOCKET2_H__36499609_63DD_459C_B4D0_1686FEEC67C2__INCLUDED_)
