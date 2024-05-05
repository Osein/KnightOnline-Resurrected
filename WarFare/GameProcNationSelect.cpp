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
	m_fFadeOut = 0.0f;
	m_bFadeOutRender = false;
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
	m_fFadeOut = 0.0f;
	m_bFadeOutRender = false;

	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // 아직 국가를 선택하지 않았다..
}

void CGameProcNationSelect::Tick()
{
	CGameProcedure::Tick();	// 키, 마우스 입력 등등..

	if (m_bFadeOutRender) FadeOutProcess();

	if (m_bFadeOutRender && m_fFadeOut == 255) {
		if (NATION_KARUS == s_pPlayer->m_InfoBase.eNation || NATION_ELMORAD == s_pPlayer->m_InfoBase.eNation)
			CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterSelect);
	}
}

void CGameProcNationSelect::Render()
{
	DWORD color = 0x00000000; // 검은색으로..
	s_pEng->Clear(color); // 클리어..
	s_pEng->s_lpD3DDev->BeginScene();			// 씬 렌더 ㅅ작...

	CGameProcedure::Render(); // UI 나 그밖의 기본적인 것들 렌더링..

	if (m_bFadeOutRender) FadeOutRender();

	s_pEng->s_lpD3DDev->EndScene();			// 씬 렌더 시작...
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcNationSelect::FadeOutRender()
{
	__VertexTransformedColor pVertices[4];

	pVertices[0].Set(0.0f, 0.0f, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));
	pVertices[1].Set(s_CameraData.vp.Width, 0.0f, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));
	pVertices[2].Set(s_CameraData.vp.Width, s_CameraData.vp.Height, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));
	pVertices[3].Set(0.0f, s_CameraData.vp.Height, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));

	DWORD dwUsefog = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwUsefog);

	DWORD dwUseLighting = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwUseLighting);

	DWORD dwUseColorVertex = FALSE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_COLORVERTEX, &dwUseColorVertex);

	unsigned long  bUseAlphaBlend = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &bUseAlphaBlend);

	int	bLight[8];
	for (int i = 0; i < 8; i++)	CN3Base::s_lpD3DDev->GetLightEnable(i, &bLight[i]);

	if (bUseAlphaBlend == FALSE) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if (dwUseLighting) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (dwUsefog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	// set render states
	if (dwUseColorVertex == FALSE) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	for (auto i = 0; i < 8; i++)	CN3Base::s_lpD3DDev->LightEnable(i, FALSE);

	DWORD dwTexStageCO, dwTexStageCARG1, dwTexStageAO, dwTexStageAARG1, dwRSSB, dwRSDB;

	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwTexStageCO);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwTexStageCARG1);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwTexStageAO);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwTexStageAARG1);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwRSSB);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwRSDB);

	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_TRANSFORMEDCOLOR);
	CN3Base::s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(__VertexTransformedColor));

	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, dwTexStageCO);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwTexStageCARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwTexStageAO);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwTexStageAARG1);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwRSSB);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwRSDB);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_COLORVERTEX, dwUseColorVertex);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, bUseAlphaBlend);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwUseLighting);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwUsefog);
	for (auto i = 0; i < 8; i++)	CN3Base::s_lpD3DDev->LightEnable(i, bLight[i]);
}

void CGameProcNationSelect::FadeOutProcess()
{
	m_fFadeOut += (30.0f / CN3Base::s_fFrmPerSec) * 12.0f;

	if (m_fFadeOut > 255)
		m_fFadeOut = 255;
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

			m_bFadeOutRender = true;
			FadeOutProcess();
		}
		return true;
	}
	
	return false;
}