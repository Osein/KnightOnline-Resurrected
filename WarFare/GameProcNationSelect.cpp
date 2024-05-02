// GameProcNationSelect.cpp: implementation of the CGameProcNationSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PacketDef.h"
#include "GameEng.h"
#include "APISocket.h"

#include "GameProcNationSelect.h"
#include "PlayerMySelf.h"
#include "UINationSelectDlg.h"
#include "UIManager.h"

CGameProcNationSelect::CGameProcNationSelect()
{
	m_pUINationSelectDlg = NULL;
	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // 아직 국가를 선택하지 않았다..
}

CGameProcNationSelect::~CGameProcNationSelect()
{
	delete m_pUINationSelectDlg; m_pUINationSelectDlg = NULL;
}

void CGameProcNationSelect::Release()
{
	CGameProcedure::Release();

	delete m_pUINationSelectDlg; m_pUINationSelectDlg = NULL;
}

void CGameProcNationSelect::Init()
{
	CGameProcedure::Init();

	std::string szTemp = "UI\\Co_NationSelect.uif";
	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->Find(NATION_ELMORAD);
	if(pTbl)
	{
		szTemp = pTbl->szNationSelect;
	}

	m_pUINationSelectDlg = new CUINationSelectDlg();
	m_pUINationSelectDlg->Init(s_pUIMgr);
	m_pUINationSelectDlg->LoadFromFile(szTemp);
	m_pUINationSelectDlg->m_pProcNationSelectRef = this; // 참조 포인터 넣기..

	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // 아직 국가를 선택하지 않았다..
}

void CGameProcNationSelect::Tick()
{
	CGameProcedure::Tick();	// 키, 마우스 입력 등등..

	if(NATION_KARUS == s_pPlayer->m_InfoBase.eNation || NATION_ELMORAD == s_pPlayer->m_InfoBase.eNation)
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterSelect); // 국가를 골랐으면 캐릭터 선택으로 바로 간다..
}

void CGameProcNationSelect::Render()
{
	DWORD color = 0x00000000; // 검은색으로..
	s_pEng->Clear(color); // 클리어..
	s_pEng->s_lpD3DDev->BeginScene();			// 씬 렌더 ㅅ작...

	CGameProcedure::Render(); // UI 나 그밖의 기본적인 것들 렌더링..

	s_pEng->s_lpD3DDev->EndScene();			// 씬 렌더 시작...
	s_pEng->Present(CN3Base::s_hWndBase);
}


void CGameProcNationSelect::MsgSendNationSelect(e_Nation eNation)
{
	BYTE byBuff[4];										// 패킷 버퍼..
	int iOffset=0;										// 버퍼의 오프셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_NATION_SELECT);	// 커멘드.
	CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)eNation);		// 아이디 길이..
		
	s_pSocket->Send(byBuff, iOffset);								// 보낸다

	s_pUIMgr->EnableOperationSet(false); // 응답 패킷을 받기 전까지 아무짓 못하게 한다..
}

bool CGameProcNationSelect::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;
	if(false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) iOffset = iOffsetPrev;
	else return true;

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 커멘드 파싱..
	switch ( iCmd )										// 커멘드에 다라서 분기..
	{
		case N3_NATION_SELECT:							// 캐릭터 선택 메시지..
		{
			int iNation = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 국가 - 0 실패.. 1 - 카루스 2 - 엘모라드..

			if( 0 == iNation ) 	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // 아직 국가를 선택하지 않았다..
			else if( 1 == iNation ) s_pPlayer->m_InfoBase.eNation = NATION_KARUS;
			else if( 2 == iNation ) s_pPlayer->m_InfoBase.eNation = NATION_ELMORAD;
		}
		return true;
	}
	
	return false;
}