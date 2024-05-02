// BirdMng.cpp: implementation of the CBirdMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "BirdMng.h"
#include "Bird.h"

CBirdMng::CBirdMng()
{
	m_pBird = NULL;
	Release();
}

CBirdMng::~CBirdMng()
{
	Release();
}

void CBirdMng::Release()
{
	if(m_pBird)
	{
		delete[] m_pBird;
		m_pBird = NULL;
	}
	m_iBirdCount = 0;
}

void CBirdMng::LoadFromFile(const std::string& szFN)
{
	Release();

	if(szFN.empty()) return;
	FILE* stream = fopen(szFN.c_str(), "r"); //text파일로 만든다 

	if(NULL == stream)
	{
#if _DEBUG
		char szErr[512];
		wsprintf(szErr, "failed to open file - %s", szFN.c_str());
		__ASSERT(stream, szErr);
#endif
		return;
	}

	int i;
	char szRrcName[_MAX_PATH];
	int result = fscanf(stream, "count = %d\n", &m_iBirdCount);			__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");

	if(m_iBirdCount>0) m_pBird = new CBird[m_iBirdCount];

	for (i=0; i<m_iBirdCount; i++)
	{
		result = fscanf(stream, "%s\n", szRrcName);	__ASSERT(result != EOF, "잘못된 bird list 세팅 파일");
		m_pBird[i].LoadBird(szRrcName);		
	}
	fclose(stream);
}

void CBirdMng::Tick()
{
	for(int i=0; i<m_iBirdCount; i++)
	{
		m_pBird[i].Tick();
	}
}

void CBirdMng::Render()
{
	for(int i=0; i<m_iBirdCount; i++)
	{
		m_pBird[i].Render();
	}
}