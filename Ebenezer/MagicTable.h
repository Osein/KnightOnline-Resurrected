// MagicTable.h: interface for the CMagicTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICTABLE_H__3BEFBF64_0058_48B1_AB56_D98E86016DFE__INCLUDED_)
#define AFX_MAGICTABLE_H__3BEFBF64_0058_48B1_AB56_D98E86016DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMagicTable  
{
public:
	CMagicTable();
	virtual ~CMagicTable();

public:
	short	m_sNum;				// ���� ��ȣ
	char	m_strName[30];		// ���� �̸�
	BYTE	m_bMoral;			// ����
	BYTE	m_bClass;			// ��� Ŭ����
	BYTE	m_bLevel;			// ����	����
	BYTE	m_bEffect;			// ȿ��(0:fire, 1:ice, 2:lightning, 3:magic, 4:disease, 5:poison, 6:light, 7:heal, 8:dagger, 9:sword, 10:mace, 11:spear, 12:polearm)
	short	m_sMp;				// �Ҹ�
	short	m_sHp;				// �Ҹ�
	short	m_sSt;				// �Ҹ�
	int		m_iNItem;			// �ʿ� ������(����)
	short	m_sDistant;			// ���ðŸ�
	short	m_sContime;			// ���ӽð�(��)
	short	m_sRepeat;			// ���� ���� �ð�(��)
	BYTE	m_bExpect;			// ������
	BYTE	m_bTarget;			// Ÿ��(0:Ÿ��, 1:�ڽ��� �ݰ�, 2:Ÿ�ٰ� �ݰ�, 3:������ �ݰ�, 4:������Ʈ)
	BYTE	m_bRange;			// �ݰ�
	BYTE	m_bType;			// Ÿ��(0:����, 1:�ɷ�ġ��ȭ, 2:��Ʈ���̻�, 3:�׷��Ⱥ�ȯ, 4:ĳ���� ��Ʈ��, 5:�����̵�/��ȯ, 6:��ȣ��, 7:ĵ������, 8:���� ��Ÿ��)
	BYTE	m_bRate1;			// �����1(�Ҽ��� 1�ڸ�, �� ���� 10���� ������ ����ؾ� �Ѵ�.)
	BYTE	m_bVar1;			// ���뺯��1
	BYTE	m_bLimit1;			// ��ų ����ġ1
	BYTE	m_bRate2;			// �����2(�Ҽ��� 1�ڸ�, �� ���� 10���� ������ ����ؾ� �Ѵ�.)
	BYTE	m_bVar2;			// ���뺯��2
	BYTE	m_bLimit2;			// ��ų ����ġ2
	BYTE	m_bRate3;			// �����3(�Ҽ��� 1�ڸ�, �� ���� 10���� ������ ����ؾ� �Ѵ�.)
	BYTE	m_bVar3;			// ���뺯��3
	BYTE	m_bLimit3;			// ��ų ����ġ3

};

#endif // !defined(AFX_MAGICTABLE_H__3BEFBF64_0058_48B1_AB56_D98E86016DFE__INCLUDED_)
