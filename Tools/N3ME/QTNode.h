// QTNode.h: interface for the CQTNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QTNODE_H__4B02CBDB_FCB9_4475_BF2F_7701DB658B28__INCLUDED_)
#define AFX_QTNODE_H__4B02CBDB_FCB9_4475_BF2F_7701DB658B28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eQTNODESTATE { QTNODESTATE_CLIPPED = 0, QTNODESTATE_NOTRENDER, QTNODESTATE_RENDER, QTNODESTATE_UNKNOWN };	// ����Ʈ�� ������ ���¼Ӽ�..
enum eDIR { DIR_LEFT = 0, DIR_TOP, DIR_RIGHT, DIR_BOTTOM, DIR_NUM };

class CLyTerrain;

class CQTNode  
{
protected:
	int								m_CenterX;	//�߽����� �����ʵ�迭���� ��ǥ.
	int								m_CenterZ;	//�߽����� �����ʵ�迭���� ��ǥ.
	
	int								m_Level;	//����Ʈ���󿡼� ���̰�..
	eQTNODESTATE					m_State;	//
		
	CQTNode*						m_pFriend[DIR_NUM];
	CQTNode*						m_pChild[DIR_NUM];

	CLyTerrain*						m_pRefTerrain;

public:
	float			m_MinY;		//������
	float			m_MaxY;		//�ְ���
	int				GetCenterX() { return m_CenterX; }
	int				GetCenterZ() { return m_CenterZ; }

private:
	bool		IsRender(POINT* pEye);
	inline int	Distance(int sx, int sz, int tx, int tz);
	void		RenderMaxLevel();
	void		RenderNormalLevel();
	
public:
	int		GetMaxLevel(eDIR dir);
	void	Render();
	void	Tick();
	void	ClearMinMaxY();
	void	SetWholeClipped();
	void	SetMinMaxY();
	void	SetCenterPoint(int x, int z);
	void	Init(int level = 0, CLyTerrain* pTerrain=NULL);
	void	Release();
	void	LinkFriend();	
	
	CQTNode();
	virtual ~CQTNode();
};

#endif // !defined(AFX_QTNODE_H__4B02CBDB_FCB9_4475_BF2F_7701DB658B28__INCLUDED_)
