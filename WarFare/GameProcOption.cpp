// GameProcOption.cpp: implementation of the CGameProcOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcOption.h"

CGameProcOption::CGameProcOption()
{
}

CGameProcOption::~CGameProcOption()
{
}

void CGameProcOption::Init()
{
	CGameProcedure::Init();
}

void CGameProcOption::Release()
{
	CGameProcedure::Release();
}

void CGameProcOption::Render()
{
	CGameProcedure::Render(); // UI 나 그밖의 기본적인 것들 렌더링..
}

void CGameProcOption::Tick()
{
	CGameProcedure::Tick();	// 키, 마우스 입력 등등..
}
