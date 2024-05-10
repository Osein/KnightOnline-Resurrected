// UICharacterCreate.cpp: implementation of the CUICharacterCreate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "UICharacterCreate.h"
#include "GameProcCharacterCreate.h"
#include "PlayerMySelf.h"

#include "../N3Base/N3UIButton.h"
#include "../N3Base/N3UIString.h"
#include "../N3Base/N3UIImage.h"
#include "../N3Base/N3UIArea.h"
#include "../N3Base/N3UIEdit.h"
#include "../N3Base/N3UITooltip.h"


CUICharacterCreate::CUICharacterCreate()
{
	memset(m_pBtn_Races, 0, sizeof(m_pBtn_Races));
	memset(m_pBtn_Classes, 0, sizeof(m_pBtn_Classes));
	memset(m_pImg_Disable_Classes, 0, sizeof(m_pImg_Disable_Classes));
	memset(m_pStr_Stats, 0, sizeof(m_pStr_Stats));
	memset(m_pArea_Stats, 0, sizeof(m_pArea_Stats));
	m_pStr_Desc = NULL;
	m_pStr_Bonus = NULL;

	m_pBtn_Face_Left = NULL;
	m_pBtn_Face_Right = NULL;
	m_pBtn_Hair_Left = NULL;
	m_pBtn_Hair_Right = NULL;

	m_pArea_Character = NULL;

	m_iBonusPoint = m_iMaxBonusPoint = 0;
}

CUICharacterCreate::~CUICharacterCreate()
{
}

void CUICharacterCreate::Release()
{
	CN3UIBase::Release();

	memset(m_pBtn_Races, 0, sizeof(m_pBtn_Races));
	memset(m_pBtn_Classes, 0, sizeof(m_pBtn_Classes));
	memset(m_pImg_Disable_Classes, 0, sizeof(m_pImg_Disable_Classes));
	memset(m_pStr_Stats, 0, sizeof(m_pStr_Stats));
	memset(m_pArea_Stats, 0, sizeof(m_pArea_Stats));

	m_pStr_Desc = NULL;
	m_pStr_Bonus = NULL;

	m_pBtn_Face_Left = NULL;
	m_pBtn_Face_Right = NULL;
	m_pBtn_Hair_Left = NULL;
	m_pBtn_Hair_Right = NULL;

	m_pArea_Character = NULL;

	m_iBonusPoint = m_iMaxBonusPoint = 0;
}

bool CUICharacterCreate::Load(HANDLE hFile)
{
	CN3UIBase::Load(hFile);

	// 캐릭터 초기화..
	__InfoPlayerBase* pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);
	__InfoPlayerMySelf* pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

	pInfoBase->eRace = RACE_UNKNOWN;
	pInfoBase->eClass = CLASS_UNKNOWN;

	m_pEdit_Name = (CN3UIEdit*)(this->GetChildByID("edit_name")); __ASSERT(m_pEdit_Name, "NULL UI Component!!");
	if(m_pEdit_Name) m_pEdit_Name->SetString("");
	
	m_pStr_Desc = (CN3UIString*)(this->GetChildByID("text_desc")); __ASSERT(m_pStr_Desc, "NULL UI Component!!");
	e_Nation eNation = pInfoBase->eNation;
	if(m_pStr_Desc)
	{
		if(NATION_KARUS == eNation)
		{
			std::string szMsg;
			::_LoadStringFromResource(IDS_SETTING_KARUS_SCREEN, szMsg);
			m_pStr_Desc->SetString(szMsg);
		}
		else if(NATION_ELMORAD == eNation)
		{
			std::string szMsg;
			::_LoadStringFromResource(IDS_SETTING_ELMORAD_SCREEN, szMsg);
			m_pStr_Desc->SetString(szMsg);
		}
	}

	m_pArea_Character = (CN3UIArea*)(this->GetChildByID("area_character")); __ASSERT(m_pArea_Character, "NULL UI Component!!");
	
	std::string szTexts[MAX_STATS] = { "text_str", "text_sta", "text_dex", "text_int", "text_map" };
	std::string szAreas[MAX_STATS] = { "area_str", "area_sta", "area_dex", "area_int", "area_map" };
	std::string szImgs[MAX_STATS] = { "img_str", "img_sta", "img_dex", "img_int", "img_map" };
	DWORD dwResrcIDs[MAX_STATS] = { IDS_NEWCHR_POW, IDS_NEWCHR_STA, IDS_NEWCHR_DEX, IDS_NEWCHR_INT, IDS_NEWCHR_MAP };
	for(int i = 0; i < MAX_STATS; i++)
	{
		m_pStr_Stats[i] = (CN3UIString*)(this->GetChildByID(szTexts[i])); __ASSERT(m_pStr_Stats[i], "NULL UI Component!!");
		m_pArea_Stats[i] = (CN3UIArea*)(this->GetChildByID(szAreas[i])); __ASSERT(m_pArea_Stats[i], "NULL UI Component!!");
		m_pImg_Stats[i] = (CN3UIImage*)(this->GetChildByID(szImgs[i]));	__ASSERT(m_pImg_Stats[i], "NULL UI Component!!");
		if(m_pArea_Stats[i]) ::_LoadStringFromResource(dwResrcIDs[i], m_pArea_Stats[i]->m_szToolTip);
	}
	m_pStr_Bonus = (CN3UIString*)(this->GetChildByID("text_bonus")); __ASSERT(m_pStr_Bonus, "NULL UI Component!!");
	

	m_pBtn_Face_Left =	(CN3UIButton*)(this->GetChildByID("btn_face_left"));	__ASSERT(m_pBtn_Face_Left, "NULL UI Component!!");
	m_pBtn_Face_Right = (CN3UIButton*)(this->GetChildByID("btn_face_right"));	__ASSERT(m_pBtn_Face_Right, "NULL UI Component!!");
	m_pBtn_Hair_Left =	(CN3UIButton*)(this->GetChildByID("btn_hair_left"));	__ASSERT(m_pBtn_Hair_Left, "NULL UI Component!!");
	m_pBtn_Hair_Right = (CN3UIButton*)(this->GetChildByID("btn_hair_right"));	__ASSERT(m_pBtn_Hair_Right, "NULL UI Component!!");
	
	std::string szBtnIDs[MAX_RACE_SELECT];
	DWORD dwResrcID_Races[MAX_RACE_SELECT];

	if(eNation==NATION_KARUS)
	{
		szBtnIDs[0] = "btn_race_ka_at";
		szBtnIDs[1] = "btn_race_ka_tu";
		szBtnIDs[2] = "btn_race_ka_wt";
		szBtnIDs[3] = "btn_race_ka_pt";
		dwResrcID_Races[0] = IDS_NEWCHR_KA_ARKTUAREK;
		dwResrcID_Races[1] = IDS_NEWCHR_KA_TUAREK;
		dwResrcID_Races[2] = IDS_NEWCHR_KA_WRINKLETUAREK;
		dwResrcID_Races[3] = IDS_NEWCHR_KA_PURITUAREK;
	}
	else if(eNation==NATION_ELMORAD)
	{
		szBtnIDs[0] = "btn_race_el_ba";
		szBtnIDs[1] = "btn_race_el_rm";
		szBtnIDs[2] = "btn_race_el_rf";
		szBtnIDs[3] = "";
		dwResrcID_Races[0] = IDS_NEWCHR_EL_BABA;
		dwResrcID_Races[1] = IDS_NEWCHR_EL_MALE; 
		dwResrcID_Races[2] = IDS_NEWCHR_EL_FEMALE;
		dwResrcID_Races[3] = -1;
	}

	for(auto i = 0; i < MAX_RACE_SELECT; i++)
	{
		if(szBtnIDs[i].empty()) continue;
		m_pBtn_Races[i] = (CN3UIButton*)(this->GetChildByID(szBtnIDs[i])); __ASSERT(m_pBtn_Races[i], "NULL UI Component!!");
		if(m_pBtn_Races[i]) ::_LoadStringFromResource(dwResrcID_Races[i], m_pBtn_Races[i]->m_szToolTip);
	}

	std::string szBtns[MAX_CLASS_SELECT] = { "btn_class_warrior", "btn_class_rogue", "btn_class_mage", "btn_class_priest" };
	std::string szImgs2[MAX_CLASS_SELECT] = { "img_warrior", "img_rogue", "img_mage", "img_priest" };
	DWORD dwResrcID_Classes[MAX_CLASS_SELECT];
	if(eNation==NATION_ELMORAD)
	{
		dwResrcID_Classes[0] = IDS_NEWCHR_EL_WARRIOR;
		dwResrcID_Classes[1] = IDS_NEWCHR_EL_ROGUE;
		dwResrcID_Classes[2] = IDS_NEWCHR_EL_MAGE;
		dwResrcID_Classes[3] = IDS_NEWCHR_EL_PRIEST;
	}
	else if(eNation==NATION_KARUS)
	{
		dwResrcID_Classes[0] = IDS_NEWCHR_KA_WARRIOR;
		dwResrcID_Classes[1] = IDS_NEWCHR_KA_ROGUE;
		dwResrcID_Classes[2] = IDS_NEWCHR_KA_MAGE;
		dwResrcID_Classes[3] = IDS_NEWCHR_KA_PRIEST;
	}

	for(auto i = 0; i < MAX_CLASS_SELECT; i++)
	{
		m_pBtn_Classes[i] =	(CN3UIButton*)(this->GetChildByID(szBtns[i]));	__ASSERT(m_pBtn_Classes[i], "NULL UI Component!!");
		m_pImg_Disable_Classes[i] = (CN3UIImage*)(this->GetChildByID(szImgs2[i]));	__ASSERT(m_pImg_Disable_Classes[i], "NULL UI Component!!");
		::_LoadStringFromResource(dwResrcID_Classes[i], m_pBtn_Classes[i]->m_szToolTip);
	}

	RECT rc = this->GetRegion();
	int iX = ((int)s_CameraData.vp.Width - (rc.right - rc.left))/2;
	int iY = ((int)s_CameraData.vp.Height - (rc.bottom - rc.top))/2;
	this->SetPos(iX, iY);
	this->Reset();

	return true;
}

bool CUICharacterCreate::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if( dwMsg == UIMSG_BUTTON_CLICK )
	{
		__InfoPlayerBase* pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);
		__InfoPlayerMySelf* pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

		//종족 고르기..
		e_Nation eNation = pInfoBase->eNation;
		e_Race eRacePrev = pInfoBase->eRace;
		bool bNeedUpdateRaceButtons = false;
		if(eNation == NATION_ELMORAD)
		{
			if ( pSender == m_pBtn_Races[0] )
			{
				pInfoBase->eRace = RACE_EL_BABARIAN;
				bNeedUpdateRaceButtons = true;
			}
			else if ( pSender == m_pBtn_Races[1] )
			{
				pInfoBase->eRace = RACE_EL_MAN;
				bNeedUpdateRaceButtons = true;
			}
			else if ( pSender == m_pBtn_Races[2] )
			{
				pInfoBase->eRace = RACE_EL_WOMEN;
				bNeedUpdateRaceButtons = true;
			}
		}
		else if(eNation ==NATION_KARUS)
		{
			if ( pSender == m_pBtn_Races[0] )
			{
				pInfoBase->eRace = RACE_KA_ARKTUAREK;
				bNeedUpdateRaceButtons = true;
			}
			else if ( pSender == m_pBtn_Races[1] )
			{
				pInfoBase->eRace = RACE_KA_TUAREK;
				bNeedUpdateRaceButtons = true;
			}
			else if ( pSender == m_pBtn_Races[2] )
			{
				pInfoBase->eRace = RACE_KA_WRINKLETUAREK;
				bNeedUpdateRaceButtons = true;
			}
			else if ( pSender == m_pBtn_Races[3] )
			{
				pInfoBase->eRace = RACE_KA_PURITUAREK;
				bNeedUpdateRaceButtons = true;
			}
		}

		if(	bNeedUpdateRaceButtons ) // 몬가 바뀌었으면..
		{
			if(eRacePrev != pInfoBase->eRace) // 종족을 바꿨을때만..
				CGameProcedure::s_pProcCharacterCreate->SetChr(); // 캐릭터 세팅..
			this->UpdateRaceAndClassButtons(pInfoBase->eRace);
		}

		int iFacePrev = pInfoExt->iFace;
		int iHairPrev = pInfoExt->iHair;

		if ( pSender->m_szID == "btn_cancel" )
		{
			CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcCharacterSelect); // 캐릭터 선택 프로시저로 한다..
			return true;
		}
		else if ( pSender->m_szID == "btn_create" && m_pEdit_Name)
		{
			CGameBase::s_pPlayer->IDSet(0, m_pEdit_Name->GetString(), 0); // 이름을 넣어주고...
			return CGameProcedure::s_pProcCharacterCreate->MsgSendCharacterCreate(); // 캐릭터 만들기 메시지 보내기...
		}
		else if ( pSender == m_pBtn_Face_Left ) // 얼굴
		{
			pInfoExt->iFace--;
			if(pInfoExt->iFace < 0) pInfoExt->iFace = 0;
		}
		else if ( pSender == m_pBtn_Face_Right )
		{
			pInfoExt->iFace++;
			if(pInfoExt->iFace > 3) pInfoExt->iFace = 3;
		}
		else if ( pSender == m_pBtn_Hair_Left ) // 머리카락..
		{
			pInfoExt->iHair--;
			if(pInfoExt->iHair < 0) pInfoExt->iHair = 0;
		}
		else if ( pSender == m_pBtn_Hair_Right )
		{
			pInfoExt->iHair++;
			if(pInfoExt->iHair > 2) pInfoExt->iHair = 2;
		}

		if(	iFacePrev != pInfoExt->iFace ) CGameBase::s_pPlayer->InitFace(); // 얼굴이 바뀌면..
		if(	iHairPrev != pInfoExt->iHair ) CGameBase::s_pPlayer->InitHair(); // 머리카락이 바뀌면..

		//직업 고르기..
		bool bNeedUpdateClassButton = false;
		if ( pSender == m_pBtn_Classes[0] ) // 전사
		{
			if(	NATION_KARUS == eNation ) pInfoBase->eClass = CLASS_KA_WARRIOR;
			else if(NATION_ELMORAD == eNation) pInfoBase->eClass = CLASS_EL_WARRIOR;
			bNeedUpdateClassButton = true;
		}
		else if ( pSender == m_pBtn_Classes[1] ) // 로그
		{
			if(	NATION_KARUS == eNation ) pInfoBase->eClass = CLASS_KA_ROGUE;
			else if(NATION_ELMORAD == eNation) pInfoBase->eClass = CLASS_EL_ROGUE;
			bNeedUpdateClassButton = true;
		}
		else if ( pSender == m_pBtn_Classes[2] ) // 마법사
		{
			if(	NATION_KARUS == eNation ) pInfoBase->eClass = CLASS_KA_WIZARD;
			else if(NATION_ELMORAD == eNation) pInfoBase->eClass = CLASS_EL_WIZARD;
			bNeedUpdateClassButton = true;
		}
		else if ( pSender == m_pBtn_Classes[3] ) // 사제
		{
			if(	NATION_KARUS == eNation ) pInfoBase->eClass = CLASS_KA_PRIEST;
			else if(NATION_ELMORAD == eNation) pInfoBase->eClass = CLASS_EL_PRIEST;
			bNeedUpdateClassButton = true;
		}
		
		if(bNeedUpdateClassButton)
			this->UpdateClassButtons(pInfoBase->eClass);

		//수치 올리기..
		if ( pSender->m_szID == "btn_str_right" ) // 힘
		{
			if(m_iBonusPoint>0)
			{
				pInfoExt->iStrength++;
				m_iBonusPoint--;

				if(m_pStr_Stats[0]) m_pStr_Stats[0]->SetStringAsInt(pInfoExt->iStrength);
				if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
			}
		}
		else if ( pSender->m_szID == "btn_sta_right" ) // 체력
		{
			if(m_iBonusPoint>0)
			{
				pInfoExt->iStamina++;
				m_iBonusPoint--;

				if(m_pStr_Stats[1]) m_pStr_Stats[1]->SetStringAsInt(pInfoExt->iStamina);
				if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
			}
		}
		else if ( pSender->m_szID == "btn_dex_right" ) // 민첩
		{
			if(m_iBonusPoint>0)
			{
				pInfoExt->iDexterity++;
				m_iBonusPoint--;

				if(m_pStr_Stats[2]) m_pStr_Stats[2]->SetStringAsInt(pInfoExt->iDexterity);
				if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);				
			}
		}
		else if ( pSender->m_szID == "btn_int_right" ) // 지능
		{
			if(m_iBonusPoint>0)
			{
				pInfoExt->iIntelligence++;
				m_iBonusPoint--;

				if(m_pStr_Stats[3]) m_pStr_Stats[3]->SetStringAsInt(pInfoExt->iIntelligence);
				if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
			}
		}
		else if ( pSender->m_szID == "btn_map_right" ) // 마력
		{
			if(m_iBonusPoint>0)
			{
				pInfoExt->iMagicAttak++;
				m_iBonusPoint--;

				if(m_pStr_Stats[4]) m_pStr_Stats[4]->SetStringAsInt(pInfoExt->iMagicAttak);
				if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
			}
		}

		__TABLE_NEW_CHR* pTbl = CGameProcedure::s_pProcCharacterCreate->m_Tbl_InitValue.Find(pInfoBase->eRace * 10000 + pInfoBase->eClass);
		if(pTbl)
		{
			//수치 내리기..
			if ( pSender->m_szID == "btn_str_left" ) // 힘
			{
				if( m_iBonusPoint < m_iMaxBonusPoint && pInfoExt->iStrength > pTbl->iStr )
				{
					pInfoExt->iStrength--;
					m_iBonusPoint++;

					if(m_pStr_Stats[0]) m_pStr_Stats[0]->SetStringAsInt(pInfoExt->iStrength);
					if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
				}
			}
			else if ( pSender->m_szID == "btn_sta_left" ) // 체력
			{
				if( m_iBonusPoint < m_iMaxBonusPoint && pInfoExt->iStamina > pTbl->iSta )
				{
					pInfoExt->iStamina--;
					m_iBonusPoint++;

					if(m_pStr_Stats[1]) m_pStr_Stats[1]->SetStringAsInt(pInfoExt->iStamina);
					if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
				}
			}
			else if ( pSender->m_szID == "btn_dex_left" ) // 민첩
			{
				if( m_iBonusPoint < m_iMaxBonusPoint && pInfoExt->iDexterity > pTbl->iDex )
				{
					pInfoExt->iDexterity--;
					m_iBonusPoint++;

					if(m_pStr_Stats[2]) m_pStr_Stats[2]->SetStringAsInt(pInfoExt->iDexterity);
					if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
				}
			}
			else if ( pSender->m_szID == "btn_int_left" ) // 지능
			{
				if( m_iBonusPoint < m_iMaxBonusPoint && pInfoExt->iIntelligence > pTbl->iInt )
				{
					pInfoExt->iIntelligence--;
					m_iBonusPoint++;

					if(m_pStr_Stats[3]) m_pStr_Stats[3]->SetStringAsInt(pInfoExt->iIntelligence);
					if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
				}
			}
			else if ( pSender->m_szID == "btn_map_left" ) // 마력
			{
				if( m_iBonusPoint < m_iMaxBonusPoint && pInfoExt->iMagicAttak > pTbl->iMAP )
				{
					pInfoExt->iMagicAttak--;
					m_iBonusPoint++;
	
					if(m_pStr_Stats[4]) m_pStr_Stats[4]->SetStringAsInt(pInfoExt->iMagicAttak);
					if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
				}
			}
		}
	}
	return true;
}

void CUICharacterCreate::Reset()
{
	__InfoPlayerBase* pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);
	__InfoPlayerMySelf* pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

	if(m_pArea_Character) 
		CGameProcedure::s_pProcCharacterCreate->m_rcChr = m_pArea_Character->GetRegion();

	for(int i = 0; i < MAX_CLASS_SELECT; i++)
	{
		if(m_pImg_Disable_Classes[i]) m_pImg_Disable_Classes[i]->SetVisible(true);
		if(m_pBtn_Classes[i]) m_pBtn_Classes[i]->SetVisible(false);
	}

	int iStats[MAX_STATS] = { pInfoExt->iStrength, pInfoExt->iStamina, pInfoExt->iDexterity, pInfoExt->iIntelligence, pInfoExt->iMagicAttak };
	for(auto i = 0; i < MAX_STATS; i++)
	{
		if(m_pImg_Stats[i]) m_pImg_Stats[i]->SetVisible(false);
		if(m_pStr_Stats[i]) m_pStr_Stats[i]->SetStringAsInt(iStats[i]);
	}

	if(m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
	this->UpdateStats();
}

void CUICharacterCreate::UpdateStats()
{
	__InfoPlayerMySelf* pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);
	int iStats[MAX_STATS] = { pInfoExt->iStrength, pInfoExt->iStamina, pInfoExt->iDexterity, pInfoExt->iIntelligence, pInfoExt->iMagicAttak };

	for (int i = 0; i < MAX_STATS; i++)
	{
		if (m_pImg_Stats[i]) m_pImg_Stats[i]->SetVisible(false);
		if (m_pStr_Stats[i]) m_pStr_Stats[i]->SetStringAsInt(iStats[i]);
	}

	if (m_pStr_Bonus) m_pStr_Bonus->SetStringAsInt(m_iBonusPoint);
}

DWORD CUICharacterCreate::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	if(m_pStr_Desc)
	{
		m_pStr_Desc->SetColor(0xffffff00);
		for(int i = 0; i < MAX_STATS; i++)
		{
			if(m_pArea_Stats[i] && m_pArea_Stats[i]->IsIn(ptCur.x, ptCur.y))
			{
				m_pStr_Desc->SetString(m_pArea_Stats[i]->m_szToolTip);
				break;
			}
		}
		for(auto i = 0; i < MAX_CLASS_SELECT; i++)
		{
			if(m_pBtn_Classes[i] && m_pBtn_Classes[i]->IsIn(ptCur.x, ptCur.y))
			{
				m_pStr_Desc->SetString(m_pBtn_Classes[i]->m_szToolTip);
				break;
			}
		}
		for(auto i = 0; i < MAX_RACE_SELECT; i++)
		{
			if(m_pBtn_Races[i] && m_pBtn_Races[i]->IsIn(ptCur.x, ptCur.y))
			{
				m_pStr_Desc->SetString(m_pBtn_Races[i]->m_szToolTip);
				break;
			}
		}
	}

	CN3UIBase::s_pTooltipCtrl->SetVisible(false);

	return CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
}

void CUICharacterCreate::UpdateRaceAndClassButtons(e_Race eRace) // 종족에 따라 직업 버튼 다시 설정..
{
	eUI_STATE eUIStateRaces[4] = {	UI_STATE_BUTTON_NORMAL, // 종족 0
									UI_STATE_BUTTON_NORMAL,
									UI_STATE_BUTTON_NORMAL,
									UI_STATE_BUTTON_NORMAL };

	eUI_STATE eUIStateClasses[4] = {	UI_STATE_BUTTON_DISABLE, // 직업 0
										UI_STATE_BUTTON_DISABLE,
										UI_STATE_BUTTON_DISABLE,
										UI_STATE_BUTTON_DISABLE  };

	switch(eRace)
	{
		case RACE_EL_BABARIAN: // 전사만 가능
			eUIStateRaces[0] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[0] = UI_STATE_BUTTON_NORMAL;
			break;
		case RACE_EL_MAN: // 모든 직업 가능
			eUIStateRaces[1] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[0] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[1] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[2] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[3] = UI_STATE_BUTTON_NORMAL;
			break;
		case RACE_EL_WOMEN: // 모든 직업 가능
			eUIStateRaces[2] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[0] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[1] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[2] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[3] = UI_STATE_BUTTON_NORMAL;
			break;
		
		case RACE_KA_ARKTUAREK: // 전사만 가능
			eUIStateRaces[0] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[0] = UI_STATE_BUTTON_NORMAL;
			break;
		case RACE_KA_TUAREK: // 로그, 사제 가능
			eUIStateRaces[1] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[1] = UI_STATE_BUTTON_NORMAL;
			eUIStateClasses[3] = UI_STATE_BUTTON_NORMAL;
			break;
		case RACE_KA_WRINKLETUAREK: // 마법사만 가능..
			eUIStateRaces[2] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[2] = UI_STATE_BUTTON_NORMAL;
			break;
		case RACE_KA_PURITUAREK: // 사제만 가능..
			eUIStateRaces[3] = UI_STATE_BUTTON_DOWN;
			eUIStateClasses[3] = UI_STATE_BUTTON_NORMAL;
			break;

		default:
			break;
	}

	// 기본 요건들 초기화..
	__InfoPlayerBase*	pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);	
	pInfoBase->eRace = eRace;
	pInfoBase->eClass = CLASS_UNKNOWN;
	
	for(int i = 0; i < MAX_RACE_SELECT; i++)
		if(m_pBtn_Races[i]) m_pBtn_Races[i]->SetState(eUIStateRaces[i]);

	for(auto i = 0; i < MAX_CLASS_SELECT; i++)
	{
		if(m_pBtn_Classes[i]) m_pBtn_Classes[i]->SetState(eUIStateClasses[i]);
	
		bool bVisible = (UI_STATE_BUTTON_DISABLE == eUIStateClasses[i]) ? true : false;
		if(m_pImg_Disable_Classes[i]) m_pImg_Disable_Classes[i]->SetVisible(bVisible);
		if(m_pBtn_Classes[i]) m_pBtn_Classes[i]->SetVisible(!bVisible);
	}
}

void CUICharacterCreate::UpdateClassButtons(e_Class eClass)
{
	eUI_STATE eUIStates[MAX_CLASS_SELECT] = {	UI_STATE_BUTTON_NORMAL, // 직업 0
								UI_STATE_BUTTON_NORMAL,
								UI_STATE_BUTTON_NORMAL,
								UI_STATE_BUTTON_NORMAL  };

	bool bVisibles[MAX_STATS] = { false, false, false, false, false };
	CGameProcedure::s_pProcCharacterCreate->SetStats();

	switch(eClass)
	{
	case CLASS_EL_WARRIOR:
	case CLASS_KA_WARRIOR:
		eUIStates[0] = UI_STATE_BUTTON_DOWN;
		bVisibles[0] = true; // 힘
		bVisibles[1] = true; // 체력
		break;
	case CLASS_EL_ROGUE:
	case CLASS_KA_ROGUE:
		eUIStates[1] = UI_STATE_BUTTON_DOWN;
		bVisibles[0] = true; // 힘
		bVisibles[2] = true; // 민첩
		break;
	case CLASS_EL_WIZARD:
	case CLASS_KA_WIZARD:
		eUIStates[2] = UI_STATE_BUTTON_DOWN;
		bVisibles[3] = true; // 지능
		bVisibles[4] = true; // 마력
		break;
	case CLASS_EL_PRIEST:
	case CLASS_KA_PRIEST:
		eUIStates[3] = UI_STATE_BUTTON_DOWN;
		bVisibles[0] = true; // 힘
		bVisibles[3] = true; // 지능
		break;
	}
	
	for(int i = 0; i < MAX_CLASS_SELECT; i++)
		if(m_pBtn_Classes[i]) m_pBtn_Classes[i]->SetState(eUIStates[i]);

	for(auto i = 0; i < MAX_STATS; i++)
		if(m_pImg_Stats[i]) m_pImg_Stats[i]->SetVisible(bVisibles[i]);
}
 
