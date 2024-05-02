// N3FXGroup.cpp: implementation of the CN3FXGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3FXBundle.h"
#include "N3FXGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3FXGroup::CN3FXGroup()
{
	m_iVersion = 1;

	FXBList.clear();
}

CN3FXGroup::~CN3FXGroup()
{
	std::list<__FXBInfo*>::iterator it, ite;
	ite = FXBList.end();
	it = FXBList.begin();

	while(it!=ite)
	{
		__FXBInfo* pFXB = (*it);
		delete pFXB;
		it++;
	}

	FXBList.clear();
}

///////////////////////////////////////////////////////////////////////

bool CN3FXGroup::Load(HANDLE hFile)
{
	DWORD			dwRWC = 0;
	ReadFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

	int count;
	ReadFile(hFile, &count, sizeof(int), &dwRWC, NULL);

	for(int i=0;i<count;i++)
	{
		__FXBInfo* pFXB = new __FXBInfo;
		ReadFile(hFile, pFXB, sizeof(__FXBInfo), &dwRWC, NULL);
		FXBList.push_back(pFXB);
	}
	return true;
}

bool CN3FXGroup::Save(HANDLE hFile)
{
	DWORD			dwRWC = 0;
	WriteFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

	int count = GetCount();
	WriteFile(hFile, &count, sizeof(int), &dwRWC, NULL);

	std::list<__FXBInfo*>::iterator it, ite;
	ite = FXBList.end();
	it = FXBList.begin();

	while(it!=ite)
	{
		__FXBInfo* pFXB = (*it);
		WriteFile(hFile, pFXB, sizeof(__FXBInfo), &dwRWC, NULL);
		it++;
	}
	return true;
}

//
//	decode script file..
//	��ũ��Ʈ ���� �а� �ؼ���Ŵ...
//
#ifdef _N3TOOL
bool CN3FXGroup::DecodeScriptFile(const char* lpPathName)
{
	FILE* stream = fopen(lpPathName, "r");
	if(!stream) return false;

	char szGamePathName[_MAX_PATH];
	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(lpPathName, szDrive, szDir, szFName, szExt);
	_makepath(szGamePathName, szDrive, szDir, szFName, "fxg");

	CN3BaseFileAccess::FileNameSet(szGamePathName);

	char szLine[512] = "", szCommand[80] = "", szBuf[4][80] = { "", "", "", ""};
	char* pResult = fgets(szLine, 512, stream);
	sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);

	if(lstrcmpi(szCommand, "<n3fxgroup>"))
	{
		fclose(stream);
		return false;
	}

	while(!feof(stream))
	{
		char* pResult = fgets(szLine, 512, stream);
		if(pResult == NULL) continue;

		ZeroMemory(szCommand,80);
		ZeroMemory(szBuf[0],80);
		ZeroMemory(szBuf[1],80);
		ZeroMemory(szBuf[2],80);
		ZeroMemory(szBuf[3],80);

		sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);

		if(lstrcmpi(szCommand, "<fxb>")==0)
		{
			__FXBInfo* pFXB = new __FXBInfo;
			sprintf(pFXB->FXBName, szBuf[0]);
			pFXB->joint = atoi(szBuf[1]);
			if(lstrcmpi(szBuf[2], "TRUE")==0) pFXB->IsLooping = TRUE;

			FXBList.push_back(pFXB);
			continue;
		}		
	}
	fclose(stream);

	return true;
}
#endif // end of _N3TOOL

__FXBInfo* CN3FXGroup::GetFXBInfo(int idx)
{
	if(idx<0 || idx>=FXBList.size()) return NULL;

	std::list<__FXBInfo*>::iterator it;
	it = FXBList.begin();

	for(int i=0;i<idx;i++) it++;
	return (*it);
}