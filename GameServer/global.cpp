#include "stdafx.h"
#include "global.h"

BOOL CheckGetVarString(int nLength, TCHAR* tBuf, TCHAR* sBuf, int nSize, int& index)
{
	int nRet = GetVarString(tBuf, sBuf, nSize, index);
	if(nRet <= 0 || nRet > nLength) return FALSE;
	return TRUE;
}

int GetVarString(TCHAR* tBuf, TCHAR* sBuf, int nSize, int& index)
{
	int nLen = 0;
	
	if(nSize == sizeof(BYTE))	nLen = GetByte(sBuf, index);
	else nLen = GetShort(sBuf, index);

	GetString(tBuf, sBuf, nLen, index);
	*(tBuf + nLen) = 0;

	return nLen;
}

void GetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf, sBuf+index, len);
	index += len;
}

BYTE GetByte(char* sBuf, int& index)
{
	int t_index = index;
	index++;
	return (BYTE)(*(sBuf+t_index));
}

int GetShort(char* sBuf, int& index)
{
	index += 2;
	return *(short*)(sBuf+index-2);
}

int GetInt(char* sBuf, int& index)
{
	index += 4;
	return *(int*)(sBuf+index-4);
}

DWORD GetDWORD(char* sBuf, int& index)
{
	index += 4;
	return *(DWORD*)(sBuf+index-4);
}

float Getfloat(char* sBuf, int& index)
{
	index += 4;
	return *(float*)(sBuf+index-4);
}

void SetString(char* tBuf, char* sBuf, int len, int& index)
{
	CopyMemory(tBuf+index, sBuf, len);
	index += len;
}

void SetVarString(TCHAR *tBuf, TCHAR* sBuf, int len, int &index)
{
	*(tBuf+index) = (BYTE)len;
	index ++;

	CopyMemory(tBuf+index, sBuf, len);
	index += len;
}

void SetByte(char* tBuf, BYTE sByte, int& index)
{
	*(tBuf+index) = (char)sByte;
	index++;
}

void SetShort(char* tBuf, int sShort, int& index)
{
	short temp = (short)sShort;

	CopyMemory( tBuf+index, &temp, 2);
	index += 2;
}

void SetInt(char* tBuf, int sInt, int& index)
{
	CopyMemory( tBuf+index, &sInt, 4);
	index += 4;
}

void SetDWORD(char* tBuf, DWORD sDword, int& index)
{
	CopyMemory( tBuf+index, &sDword, 4);
	index += 4;
}

void Setfloat ( char* tBuf, float sFloat, int& index )
{
	CopyMemory( tBuf+index, &sFloat, 4);
	index += 4;
}

int ParseSpace( char* tBuf, char* sBuf)
{
	int i = 0, index = 0;
	BOOL flag = FALSE;
	
	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	while(sBuf[index] !=' ' && sBuf[index] !='\t' && sBuf[index] !=(BYTE) 0){
		tBuf[i++] = sBuf[index++];
		flag = TRUE;
	}
	tBuf[i] = 0;

	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	if(!flag) return 0;	
	return index;
}

CString GetProgPath()
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	_splitpath(Buf,drive,dir,fname,ext);
	strcpy(Path, drive);
	strcat(Path, dir);		
	CString _Path = Path;
	return _Path;
}

int myrand( int min, int max, BOOL bSame )
{
	static int nOld = 0;
	int nRet = 0;
	int nLoop = 10;

	if( min == max ) return min;

	while(nLoop--)
	{
		nRet =  (rand() % (max-min+1)) + min;
		if(bSame == FALSE)
		{
			if(nRet != nOld) 
			{
				nOld = nRet;
				return nRet;
			}
		}
		else return nRet;
	}

	return nRet;
}

///////////////////////////////////////////////////////////////////////////
//	XdY 형식의 주사위 굴리기
//
int XdY(int x, int y)
{
	int temp = 0;
	if(x <= 0) return myrand(x, y, TRUE);

	for(int i = 0; i < x; i++)	temp += myrand(1, y, TRUE);
	return temp;
}

///////////////////////////////////////////////////////////////////////////
//	string의 길이가 size 보다 작으면 size 만큼 공백을 채워서 복사한다.
//
void SpaceCopy(LPSTR source, CString dest, int size)
{
	strncpy(source, dest, size);
	
	int len = strlen(source);
	for(int i = size - 1; i >= len; i--)
	{
		source[i] = ' ';
	}
}

///////////////////////////////////////////////////////////////////////////
//	string의 길이가 size 보다 작으면 size 만큼 공백을 채워서 복사한다.
//
void SpaceCopy(CString& source, CString dest, int size)
{
	source = dest.Left(size);
	
	int len = source.GetLength();
	if(len < size)
	{
		for(int i = size; i > len; i--)
		{
			source += ' ';
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//	DWORD 의 Max 값을 채크하면서 증가시킨다.
//
void CheckMaxValue(DWORD& dest, DWORD add)
{
	DWORD Diff = _MAX_DWORD - dest;

	if(add <= Diff) dest += add;
	else dest = _MAX_DWORD;
}

///////////////////////////////////////////////////////////////////////////
//	int 의 Max 값을 채크하면서 증가시킨다.
//
void CheckMaxValue(int& dest, int add)
{
	int Diff = _MAX_INT - dest;

	if(add <= Diff) dest += add;
	else dest = _MAX_INT;
}

///////////////////////////////////////////////////////////////////////////
//	short 의 Max 값을 채크하면서 증가시킨다.
//
void CheckMaxValue(short& dest, short add)
{
	short Diff = _MAX_SHORT - dest;

	if(add <= Diff) dest += add;
	else dest = _MAX_SHORT;
}

BOOL CheckMaxValueReturn(DWORD& dest, DWORD add)
{
	DWORD Diff = _MAX_DWORD - dest;

	if(add <= Diff) return TRUE;//dest += add;
	else return FALSE;//dest = _MAX_DWORD;
}

void LogFileWrite( CString logstr )
{
	CString ProgPath, LogFileName;
	CFile file;
	int loglength;

	ProgPath = GetProgPath();
	loglength = logstr.GetLength();

	LogFileName.Format("%s\\AIServer.log", ProgPath);
	
	file.Open( LogFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );

	file.SeekToEnd();
	file.Write(logstr, loglength);
	file.Close();
}

void	TimeTrace(TCHAR* pMsg)
{
	CString szMsg = _T("");
	CTime time = CTime::GetCurrentTime();
	szMsg.Format("%s,,  time : %d-%d-%d, %d:%d]\n", pMsg, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
	TRACE(szMsg);
}