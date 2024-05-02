// APISocket.cpp: implementation of the CAPISocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APISocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
WSAData		CAPISocket::s_WSData;
int			CAPISocket::s_nInstanceCount = 0;


//#define STX 1007
//#define ETX 1005
const WORD PACKET_HEADER = 0XAA55;
const WORD PACKET_TAIL = 0X55AA;

CAPISocket::CAPISocket()
{
	m_hSocket = INVALID_SOCKET;
	if (s_nInstanceCount == 0)
	{
		WSAStartup(0x0101, &s_WSData);
	}
	s_nInstanceCount++;

	m_iSendByteCount = 0;
}

CAPISocket::~CAPISocket()
{
	Release();

	s_nInstanceCount--;
	if (s_nInstanceCount==0)
	{
		WSACleanup();
	}
}

void CAPISocket::Release()
{
	this->Disconnect();

	DataPack* pBuf;
	while ( m_qRecvPkt.size() > 0 )
	{
		pBuf = m_qRecvPkt.front();
		delete pBuf;
		m_qRecvPkt.pop();
	}

	m_iSendByteCount = 0;
}

BOOL CAPISocket::Disconnect()
{
	if (m_hSocket != INVALID_SOCKET)
		closesocket(m_hSocket);
	m_hSocket = NULL;

	return TRUE;
}

BOOL CAPISocket::Connect(HWND hWnd, const char* pszIP, DWORD port)
{	
	if (!pszIP || !port) return FALSE;

	//
	if (m_hSocket != INVALID_SOCKET)
	{	
		this->Disconnect();
	}

	//
	int i=0;
	struct sockaddr_in far server;
	struct hostent far *hp;
  
	if( (pszIP[0] >= '0') && (pszIP[0] <= '9') ) 
	{
	   memset((char *) &server,0,sizeof(server));
	   server.sin_family      = AF_INET;
	   server.sin_addr.s_addr = inet_addr(pszIP);
	   server.sin_port        = htons((u_short)port);
	}
	else
	{ 
		if ( (hp = (hostent far *)gethostbyname(pszIP)) == NULL)
		{
#ifdef _DEBUG
			char msg[256];	
			sprintf(msg,"Error: Connecting to %s.",pszIP);
			MessageBox(hWnd, msg,"socket error", MB_OK | MB_ICONSTOP );
#endif
			return FALSE;
		}
		memset((char *) &server,0,sizeof(server));
		memcpy((char *) &server.sin_addr,hp->h_addr,hp->h_length);
		server.sin_family = hp->h_addrtype;
		server.sin_port = htons((u_short)port);  
	}// else 

	// create socket 
	if( (m_hSocket = socket(AF_INET, SOCK_STREAM, 0)) < 1) 
	{
#ifdef _DEBUG
		char msg[256];	
		sprintf(msg,"Error opening stream socket");
		MessageBox(hWnd, msg,"socket error", MB_OK | MB_ICONSTOP);
#endif
		return FALSE;
	}
  
	if (connect(m_hSocket, (struct sockaddr far *)&server, sizeof(server)) != 0) 
	{
		int iErrCode = ::WSAGetLastError();

		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

#ifdef _DEBUG
		char msg[256];
		sprintf(msg, "Cannot connect to %s on port %u : ErrorCode : %d", pszIP, port, iErrCode);
		MessageBox(hWnd, msg, "socket error", MB_OK | MB_ICONSTOP);
#endif
		return FALSE;
	}

//	WSAAsyncSelect(m_hSocket, hWnd, WM_SOCKETMSG, FD_CONNECT | FD_ACCEPT | FD_READ | FD_CLOSE);
	WSAAsyncSelect(m_hSocket, hWnd, WM_SOCKETMSG, FD_CONNECT | FD_READ | FD_CLOSE);

	m_hWndTarget = hWnd;
	
	return TRUE;
}

void CAPISocket::Receive()
{
	if (m_hSocket == INVALID_SOCKET)	return;

	DWORD	dwPktSize;
	DWORD	dwRead = 0;
	int		count = 0;

	ioctlsocket(m_hSocket, FIONREAD, &dwPktSize);
	while(dwRead < dwPktSize)
	{
		count = recv(m_hSocket, (char*)m_RecvBuf, RECEIVE_BUF_SIZE, 0);
		if (count == SOCKET_ERROR)
		{
			ASSERT(0);
			break;
		}
		if (count)
		{
			dwRead += count;
			m_CB.PutData(m_RecvBuf, count);
		}
	}

	// packet analysis.
	while(ReceiveProcess());
}

BOOL CAPISocket::ReceiveProcess()
{
	int iCount = m_CB.GetValidCount();
	BOOL bFoundTail = FALSE;
	if (iCount >=7 )
	{
		BYTE *pData = new BYTE[iCount];
		m_CB.GetData(pData, iCount);
		int head_inc_size = 0;

		if ( PACKET_HEADER == ntohs(*((WORD*)pData)) )
		{
			short siCore = *((short*)(pData+2));
			if ( siCore <= iCount )
			{
				if ( PACKET_TAIL == ntohs(*((WORD*)(pData+iCount-2))) ) // ��Ŷ ���� �κ� �˻�..
				{
					DataPack *pDP = new DataPack(siCore, pData+4);
					m_qRecvPkt.push(pDP);
					m_CB.HeadIncrease(siCore + 6); // ȯ�� ���� �ε��� ���� ��Ű��..

					bFoundTail = TRUE;
				}
			}
		}

		delete[] pData, pData = NULL;
	}

	return bFoundTail;
}

void CAPISocket::Send(BYTE* pData, int nSize)
{
	if (m_hSocket == INVALID_SOCKET)	return;

	// UZDream ��Ŷ ���Ŀ� �����ִ� �κ�. STX, ETX, size�� �ٿ��ش�. ���� ������ �κ��� ��Ŷ���鶧 �ٿ��� �־���� ��. 
	// ���ո������� ������ ��Ŷ ���Ŀ� ���߱� ���ؼ�... 
//	int nTotalSize = nSize+10;
//	BYTE *pSndData = m_RecvBuf;
//	*((WORD*)pSndData) = STX;			pSndData+=2;
//	*((WORD*)pSndData) = nTotalSize;	pSndData+=2;
//	*((WORD*)pSndData) = 0x0000;		pSndData+=2;
//	*((WORD*)pSndData) = 0x0000;		pSndData+=2;
//	memcpy(pSndData, pData, nSize);		pSndData += nSize;
//	*((WORD*)pSndData) = ETX;			pSndData+=2;	
	
	int nTotalSize = nSize+6;
	BYTE *pSendData = m_RecvBuf;
	*((WORD*)pSendData) = htons(PACKET_HEADER);	pSendData+=2;
	*((WORD*)pSendData) = nSize;			pSendData+=2;
	memcpy(pSendData, pData, nSize);		pSendData += nSize;
	*((WORD*)pSendData) = htons(PACKET_TAIL);		pSendData+=2;

	int nSent = 0;
	int count = 0;
	while(nSent < nTotalSize)
	{
		count = send(m_hSocket, (char*)m_RecvBuf, nTotalSize, 0);
		if (count == SOCKET_ERROR)
		{
//			__ASSERT(0,"socket send error!");
//			DWORD dwErr = GetLastError();
//			TRACE("%d\n", dwErr);
			break;
		}
		if (count)
		{
			nSent += count;
		}
	}

	m_iSendByteCount += nTotalSize;
}