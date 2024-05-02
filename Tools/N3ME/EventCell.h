// EventCell.h: interface for the CEventCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTCELL_H__B3DFDE57_2071_433F_A831_8B26FFE0004B__INCLUDED_)
#define AFX_EVENTCELL_H__B3DFDE57_2071_433F_A831_8B26FFE0004B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../N3Base/N3Base.h"

class CLyTerrain;

const MAX_CON = 5;

class CEventCell : public CN3Base
{
protected:
	__VertexXyzColor	m_TileVB[4];

public:
	short				m_ZoneID;
	short				m_EventID;
	short				m_EventType;
	char				m_Con[MAX_CON][256];
	char				m_Exe[MAX_CON][256];
	char				m_Name[80];
	RECT				m_Rect;
	POINT				m_BasePoint;

	CLyTerrain*			m_pRefTerrain;

protected:
	void	MakeTileVB(int x, int z, DWORD color);
	
public:
	void	InitRect(__Vector3 v);
	void	AddRect(__Vector3 v);
	void	Render(DWORD color);
	void	Load(HANDLE hFile);
	void	Save(HANDLE hFile);

	CEventCell();
	CEventCell(CLyTerrain* pRefTerrain);
	virtual ~CEventCell();
};

#endif // !defined(AFX_EVENTCELL_H__B3DFDE57_2071_433F_A831_8B26FFE0004B__INCLUDED_)
