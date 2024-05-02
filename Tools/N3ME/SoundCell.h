// SoundCell.h: interface for the CSoundCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDCELL_H__B3DFDE57_2071_433F_A831_8B26FFE0004B__INCLUDED_)
#define AFX_SOUNDCELL_H__B3DFDE57_2071_433F_A831_8B26FFE0004B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../N3Base/N3Base.h"

class CLyTerrain;

const MAX_CON = 5;

class CSoundCell : public CN3Base
{
protected:
	int					m_iVersion;
	__VertexXyzColor	m_TileVB[4];

public:
	CLyTerrain*			m_pRefTerrain;

	DWORD				m_dwSoundGroupID;

	RECT				m_Rect;
	POINT				m_BasePoint;

protected:
	void	MakeTileVB(int x, int z, DWORD color);
	
public:
	void	InitRect(__Vector3 v);
	void	AddRect(__Vector3 v);
	void	Render(DWORD color);
	void	Load(HANDLE hFile);
	void	Save(HANDLE hFile);

	CSoundCell();
	CSoundCell(CLyTerrain* pRefTerrain);
	virtual ~CSoundCell();
};

#endif // !defined(AFX_SOUNDCELL_H__B3DFDE57_2071_433F_A831_8B26FFE0004B__INCLUDED_)
