// RiverMng.h: interface for the CRiverMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIVERMNG_H__3A365AC2_8F3E_4E3A_98B4_953352C11BA3__INCLUDED_)
#define AFX_RIVERMNG_H__3A365AC2_8F3E_4E3A_98B4_953352C11BA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../N3Base/N3BaseFileAccess.h"
#include "VtxPosDummy.h"
#include <afxtempl.h>

enum {RCM_NONE=0, RCM_SELECT=1, RCM_CREATE=2};

class CRiverMesh;
class CMainFrame;
class CDlgRiverProperty;

#include <list>
typedef std::list<CRiverMesh*>::iterator it_RiverMesh;

class CRiverMng  : public CN3BaseFileAccess
{
public:
	CRiverMng(CMainFrame* pMainFrm);
	virtual ~CRiverMng();

	class __VertexRiver
	{
	public:
		float x,y,z;
		float nx,ny,nz;
		D3DCOLOR	color;
		float u,v, u2,v2;
		void Set(float sx, float sy,float sz, float snx, float sny, float snz, D3DCOLOR scolor, float su, float sv, float su2, float sv2)
		{
			x = sx, y = sy, z = sz;
			nx = snx, ny = sny, nz = snz;
			color = scolor;
			u = su, v = sv;
			u2 = su2, v2 = sv2;
		}
	};

// Attributes
public:
	CRiverMesh*		GetSelRiver() const {return m_pSelRiver;}
	int				GetRCursorMode() const {return m_RCursorMode;}
	int				GetDrawRiverNum() {return m_RiverMeshes.size();}
	it_RiverMesh	GetDrawRiver();
protected:
	std::list<CRiverMesh*>			m_RiverMeshes;				// River Mesh list
	CRiverMesh*						m_pSelRiver;				// ���õ� RiverMesh
	CTypedPtrArray<CPtrArray, __VertexXyzT2*>	m_SelVtxArray;	// ���õ� ����
	CVtxPosDummy					m_VtxPosDummy;				// ���� �����ϼ� �ִ� dummy object
	BOOL							m_bEditMode;				// ���� ��������ΰ�?
	int								m_RCursorMode;				// ���� �߰�?�ϴ� ���ΰ� ���� �����ϴ� ���ΰ�?

	CMainFrame*						m_pMainFrm;					// mainframe pointer
	CDlgRiverProperty*				m_pDlgProperty;				// property dialog

	RECT							m_rcSelDrag;				// �巡�� ����
	__VertexXyzColor				m_CreateLine[2];			// �� ó�� �����Ҷ� ���̴� ��

// Operations
public:
	void			Tick();
	void			Render();
	virtual void	Release();
	virtual bool	Load(HANDLE hFile);
	virtual bool	Save(HANDLE hFile);

	void			MakeGameFiles(HANDLE hFile, float fSize);				// Game file�� ����
	CRiverMesh*		CreateNewRiverMesh(__Vector3& vPos1, __Vector3& vPos2,
		__Vector3& vPos3, __Vector3& vPos4);								// ���ο� ���� �߰��Ѵ�.
	void			RemoveRiverMesh(int iRiverID);							// ���õ� ���� �����Ѵ�.
	BOOL			MouseMsgFilter(LPMSG pMsg);								// Mouse �޼��� ó��
	BOOL			SetRiverID(CRiverMesh* pRiverMesh, int iRiverID);		// ���� ID�� ����
	void			SetEditMode(BOOL bEditMode);							// �� EditMode����
	void			ExtrudeRiverEdge();
	CRiverMesh*		GetRiverMesh(int iRiverID);								// RiverMesh ���
	void			DeleteSelectedVertex();									// ���õ� ���� �����
	void			ReCalcUV();												// ���õ� ���� UV��ǥ �ٽ� ���.
	void			ReCalcSelectedVertex();

	void			GoRiver(int iRiverID);									// ���õ� ������ ���ϴ�.
protected:
	void			SetSelRiver(CRiverMesh* pRiverMesh);					// RiverMesh �����ϱ�
	void			SelectVtxByDragRect(RECT* pRect, BOOL bAdd);

};

#endif // !defined(AFX_RIVERMNG_H__3A365AC2_8F3E_4E3A_98B4_953352C11BA3__INCLUDED_)
