// NpcTable.cpp: implementation of the CNpcTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "NpcTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcTable::CNpcTable()
{

}

CNpcTable::~CNpcTable()
{

}

void CNpcTable::Initialize()
{
	m_sSid = -1;				// MONSTER(NPC) Serial ID
	memset(m_strName, 0x00, MAX_ID_SIZE+1);		// MONSTER(NPC) Name
	m_sPid = 0;
	m_sSize = 0;
	m_iWeapon_1 = 0;			// ����迭
	m_iWeapon_2 = 0;			// ����迭
	m_byGroup = 0;			// �Ҽ�����
	m_byActType = 0;		// �ൿ����
	m_byRank = 0;			// ����
	m_byTitle = 0;			// ����
	m_iSellingGroup = 0;	// ������ �׷�(���ǸŸ� ��� NPC�� ��츸)
	m_sLevel = 0;			// level
	m_iExp = 0;				// ����ġ
	m_iLoyalty = 0;			// loyalty
	m_iMaxHP = 0;			// �ִ� HP
	m_sMaxMP = 0;			// �ִ� MP
	m_sAttack = 0;			// ���ݰ�
	m_sDefense = 0;			// ��
	m_sHitRate = 0;			// Ÿ�ݼ�����
	m_sEvadeRate = 0;		// ȸ�Ǽ�����
	m_sDamage = 0;			// �⺻ ������
	m_sAttackDelay = 0;		// ���ݵ�����
	m_sSpeed = 0;			// �̵��ӵ�	
	m_sStandTime = 0;		// ���ִ� �ð�
	m_iMagic1 = 0;			// ��븶�� 1
	m_iMagic2 = 0;			// ��븶�� 2
	m_iMagic3 = 0;			// ��븶�� 3	
	m_byFireR = 0;			// ȭ�� ���׷�
	m_byColdR = 0;			// �ñ� ���׷�
	m_byLightR = 0;			// �� ���׷�
	m_byMagicR = 0;			// ���� ���׷�
	m_byDiseaseR = 0;		// ���� ���׷�
	m_byPoisonR = 0;		// �� ���׷�
	m_sBulk  = 0;
	m_bySearchRange = 0;	// �� Ž�� ����
	m_byAttackRange = 0;	// �����Ÿ�
	m_byTracingRange = 0;
	m_sAI = 0;				// �ΰ����� �ε���
	m_tNpcType = 0;			// NPC Type
						// 0 : Monster
						// 1 : Normal NPC
	m_byFamilyType = 0;		// ������̿��� �������踦 �����Ѵ�.
	m_tItemPer = 0;			// �������� ������ Ȯ��
	m_tDnPer = 0;			// ���� ������Ȯ��
	m_iMoney = 0;			// �������� ��
	m_iItem = 0;			// �������� ������
	m_byDirectAttack = 0;	// ���ݹ��( 0 : ����, 1:�հ���(��������), 2:��,�������� )
	m_byMagicAttack = 0;	// ��������( 0:�������� ����, 1:��������, 2:������, 3:����)
}
