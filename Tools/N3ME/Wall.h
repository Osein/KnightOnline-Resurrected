// Wall.h: interface for the CWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALL_H__77875B11_2DB7_42DB_8C20_95A1AEDC17B8__INCLUDED_)
#define AFX_WALL_H__77875B11_2DB7_42DB_8C20_95A1AEDC17B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\N3BASE\N3Base.h"
#include <list>

class CWall : public CN3Base  
{
public:
	char	m_Name[80];
	std::list<__Vector3> m_Wall;

public:
	void	Load(HANDLE hFile);
	void	Save(HANDLE hFile);
	int		GetSize() { return m_Wall.size(); }
	bool	GetVertex(int idx, __Vector3* pPos);	// idx° �� ��������..
	void	AddVertex(__Vector3 Vertex);	//path���ϱ�.
	void	DelPrevVertex();

	CWall();
	virtual ~CWall();
};

#endif // !defined(AFX_WALL_H__77875B11_2DB7_42DB_8C20_95A1AEDC17B8__INCLUDED_)
