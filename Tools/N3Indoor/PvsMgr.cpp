// PvsBase.cpp: implementation of the CPvsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "n3indoor.h"

#include "PvsMgr.h"
#include "PvsBase.h"

#include "PortalWall.h"
#include "PortalVol.h"

#include "OrganizeView.h"
#include "MainFrm.h"
#include "N3IndoorDoc.h"
#include "N3IndoorView.h"

#include "../N3Base/N3Base.h"
#include "../N3Base/N3Shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPvsMgr::CPvsMgr()
{
	m_iIncreseIndex = 0;
	m_iTotalCount = 0;
	m_iCurIndex = -1;

	m_bShapePerVolumn = true;

	m_MtxShapeMove.Identity();	
	m_MtxShapeScale.Identity();	
	m_MtxShapeRot.Identity();
}

CPvsMgr::~CPvsMgr()
{
	DeleteAllPvsObj();
}

//.......
void CPvsMgr::InsertNewPortalVolumn()
{
	m_pPvsList.push_back(m_Factory.CreatePvsVol(m_iIncreseIndex++));
	m_iTotalCount++;
}

void CPvsMgr::InsertNewPortalWall(e_WallType eWT, CPortalVol * const pVol)
{
	if (!pVol)	return;

	m_pPvsList.push_back(m_Factory.CreatePvsWall(m_iIncreseIndex++, eWT, pVol));
	m_iTotalCount++;
}

CPortalWall* CPvsMgr::InsertNewPortalWallGetPointer(e_WallType eWT)
{
	CPvsBase* pBase = NULL;
	pBase = m_Factory.CreatePvsWall(m_iIncreseIndex++, eWT);
	if (!pBase) return NULL;
	m_pPvsList.push_back(pBase);
	m_iTotalCount++;
	return (CPortalWall* )pBase;
}

void CPvsMgr::DeleteAllPvsObj()
{
	iter it = m_pPvsList.begin();

	while (it != m_pPvsList.end())	
	{
		CPvsBase* pvs = *it;
		if (pvs) delete pvs;
		it++;
	}	
	m_pPvsList.clear();

	m_iTotalCount = 0;
	m_iIncreseIndex = 0;
}

CPvsBase* CPvsMgr::GetPvsObjByiOrder(int iOrder)
{
	if ((m_pPvsList.size() <= 0) || (iOrder < 0) || (m_pPvsList.size()-1 < iOrder))
		return NULL;

	iter it = m_pPvsList.begin();
	for( int i = 0; i < iOrder; i++)
		it++;

	return *it;
}

CPvsBase* CPvsMgr::GetPvsWallByiOrder(int iOrder)
{
	int i = 0;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		CPvsBase* pvs = *it++;
		if (pvs->IsKindOf(RUNTIME_CLASS(CPortalWall)))
		{
			if (i++ == iOrder) 
				return pvs;
		}		
	}

	return NULL;
}

bool CPvsMgr::DeletePvsObjByiOrder(int iOrder)
{
	if (iOrder == -1)	return false;

	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();
	for( int i = 0; i < iOrder; i++)
		it++;

	pBase = *it;
	if (pBase)	delete pBase;
	else	return false;

	m_pPvsList.erase(it);
	m_iTotalCount--;

	return true;
}

void CPvsMgr::DeletePvsLinkedObjByiID(int iID)
{
	iter itBase = m_pPvsList.begin();
	iter itLinked; 
	CPvsBase *pvsBase, *pvsLinked;

	while (itBase != m_pPvsList.end())	
	{
		pvsBase = *itBase++;
		if (!pvsBase->IsKindOf(RUNTIME_CLASS(CPortalVol))) continue;
		CPortalVol* pVol = (CPortalVol* )pvsBase;

		itLinked = pVol->m_pPvsList.begin();
		while (itLinked != pVol->m_pPvsList.end())
		{
			pvsLinked = *itLinked;	
			if (pvsLinked->m_iID == iID)
				itLinked = pVol->m_pPvsList.erase(itLinked);
			else
				itLinked++;
		}
	}		
}

void CPvsMgr::TickEdit()
{
	CPvsBase* pBase = NULL;
	CPvsBase* pLinked = NULL;

	int i = 0;

	iter it = m_pPvsList.begin();
	iter itLinked; 

	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		pBase->SetState(STATE_NONE);
	}

	it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		if (m_iCurIndex == i)
		{
			pBase = *it;
			pBase->SetState(STATE_SELECTED);

			if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol))) 
			{
				CPortalVol* pVol = (CPortalVol* )pBase;

				itLinked = pVol->m_pPvsList.begin();
				while( itLinked != pVol->m_pPvsList.end() )
				{
					pLinked = *itLinked;
					pLinked->SetState(STATE_LINKED);
					itLinked++;
				}
			}
		}			

		it++;
		i++;
	}
}

void CPvsMgr::RenderEdit()
{
	CPvsBase* pBase = NULL;

	iter it = m_pPvsList.begin();
	it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		pBase->RenderEdit();
	}

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	if ( (pFrm->m_eShapeManage == SHAPEMANAGE_STATE_ONLYONE) && (pFrm->m_eShapeView == SHAPEVIEW_STATE_TOTAL) )
	{
		// Shape..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
			TotalShapeRender();

		// Collision..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
			TotalCollisionRender();	
	}
}

void CPvsMgr::TickCompile()
{
	if (m_iCurIndex != -1)
	{
	}
	else
	{
	}
}

void CPvsMgr::RenderCompile()
{
	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if ( (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol))) && (((CPortalVol* )pBase)->m_eRenderType == TYPE_TRUE) )
		{
			pBase->RenderCompile();
		}
	}

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	if ( (pFrm->m_eShapeManage == SHAPEMANAGE_STATE_ONLYONE) && (pFrm->m_eShapeView == SHAPEVIEW_STATE_TOTAL) )
	{
		// Shape..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
			TotalShapeRender();

		// Collision..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
			TotalCollisionRender();	
	}

// Debug�� ������..
/*	typedef std::list<__Collision>::iterator citer;
	citer cit = m_ColList.begin();
	while (cit != m_ColList.end())
	{
		__Collision col = *cit++;
		RenderCollision(col);
	}
*/
}

void CPvsMgr::TickExecute()
{
	if (m_iCurIndex != -1)
	{
	}
	else
	{
	}
}

void CPvsMgr::RenderExecute()
{
	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if ( (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol))) && (((CPortalVol* )pBase)->m_eRenderType == TYPE_TRUE) )
		{
			pBase->RenderExecute();
		}
	}

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	if ( (pFrm->m_eShapeManage == SHAPEMANAGE_STATE_ONLYONE) && (pFrm->m_eShapeView == SHAPEVIEW_STATE_TOTAL) )
	{
		// Shape..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
			TotalShapeRender();

		// Collision..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
			TotalCollisionRender();	
	}
}

void CPvsMgr::TotalShapeRender()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CN3Shape* pSh = ((CMainFrame*)AfxGetMainWnd())->m_pShapeBg;

	if (pSh)	
	{
		__Vector3 vec; vec.Set(1.0f, 1.0f, 1.0f);
		vec *= m_MtxShapeScale;
		pSh->ScaleSet(vec);
		__Quaternion qt; D3DXQuaternionRotationMatrix(&qt, &m_MtxShapeRot);
		pSh->RotSet(qt.x, qt.y, qt.z, qt.w);
		vec.Set(0.0f, 0.0f, 0.0f);
		vec *= m_MtxShapeMove;
		pSh->PosSet(vec);
		pSh->Tick(-1000);

		pSh->Render();
	    CN3Base::s_AlphaMgr.Render(); // Alpha primitive �׸���...
	}

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPvsMgr::TotalCollisionRender()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CN3Shape* pSh = ((CMainFrame*)AfxGetMainWnd())->m_pShapeBg;

	if (pSh)	
	{
		__Vector3 vec; vec.Set(1.0f, 1.0f, 1.0f);
		vec *= m_MtxShapeScale;
		pSh->ScaleSet(vec);
		__Quaternion qt; D3DXQuaternionRotationMatrix(&qt, &m_MtxShapeRot);
		pSh->RotSet(qt.x, qt.y, qt.z, qt.w);
		vec.Set(0.0f, 0.0f, 0.0f);
		vec *= m_MtxShapeMove;
		pSh->PosSet(vec);
		pSh->Tick(-1000);

		pSh->CollisionMesh()->Render(0xffffffff);
		CN3Base::s_AlphaMgr.Render(); // Alpha primitive �׸���...
	}

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPvsMgr::Load(FILE* stream)
{
	m_iIncreseIndex = 0;

	// Total Count.. 
	fread(&m_iTotalCount, sizeof(int), 1, stream);

	int iType, iID;
	e_WallType eWT;
	CPvsBase* pBase = NULL;
	CPvsBase* pLinked = NULL;

	for( int i = 0; i < m_iTotalCount; i++ )
	{
		fread(&iType, sizeof(int), 1, stream);
		switch (iType)
		{
			case _iTypeWall:		
				fread(&eWT, sizeof(int), 1, stream);
				fread(&iID, sizeof(int), 1, stream);
				m_iIncreseIndex = iID+1;
				pBase = m_Factory.CreatePvsWall(iID, eWT);
				((CPortalWall* )pBase)->m_eWallType = (e_WallType)eWT;
				pBase->Load(stream);
				m_pPvsList.push_back(pBase);
				break;

			case _iTypeVolumn:
				fread(&iID, sizeof(int), 1, stream);
				m_iIncreseIndex = iID+1;
				pBase = m_Factory.CreatePvsVol(iID);
				pBase->Load(stream);
				m_pPvsList.push_back(pBase);
				break;
		}
	}

	// ��ũ�� ���̵���� �о �����Ŵ..
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)))
		{
			int iID;
			CPortalVol* pVol = (CPortalVol* )pBase;
			iIDiter iditer = pVol->m_piIDList.begin();

			while ( iditer != pVol->m_piIDList.end() )
			{
				iID = *iditer++;
				pLinked = GetPvsWallByiID(iID);
				if (pLinked)
				{
					pVol->m_pPvsList.push_back(pLinked);
				}
				else
				{
					pLinked = GetPvsVolByiID(iID);
					if (pLinked)
					{
						pVol->m_pPvsList.push_back(pLinked);
					}
				}
			}
		}
	}

	it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)))
		{
			CPortalVol* pVol = (CPortalVol* )pBase;
			pVol->m_piIDList.clear();
		}
	}
}

void CPvsMgr::Save(FILE* stream)
{
	// Total Count..
	fwrite(&m_iTotalCount, sizeof(int), 1, stream);	
	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		// �ڽ��� ������ ����..
		pBase = *it++;
		pBase->Save(stream);
	}
}

CPvsBase* CPvsMgr::GetPvsWallByiID(int iID)
{
	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalWall)) && (pBase->m_iID == iID))
			return pBase;
	}

	return NULL;
}

CPvsBase* CPvsMgr::GetPvsVolByiID(int iID)
{
	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)) && (pBase->m_iID == iID))
			return pBase;
	}

	return NULL;
}

void CPvsMgr::ComputeVisibilty(CPortalVol * const pVolMy)
{
	m_dcol.Vvec[0] = __Vector3(0, 0, 0);
	m_dcol.Vvec[1] = __Vector3(0, 0, 0);

	SetPriority(pVolMy);

	// PortalVol�θ� �̷���� ����Ʈ�� ����..
	std::priority_queue< CPortalVol*, std::vector<CPortalVol*>, Myless<CPortalVol*> > pQueue; 

	CPvsBase* pBase = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)))
			pQueue.push((CPortalVol* )pBase);
	}

	// ����׿� Ʈ���̽�..
	CPortalVol* pVol = NULL;
	while (!pQueue.empty())
	{
		pVol = pQueue.top();
		TRACE2("%s Priority is %d\n", pVol->m_strID.c_str(), pVol->m_iPriority);
		pVol->m_eRenderType = TYPE_UNKNOWN;
		m_pVoltList.push_back(pVol);			
		pQueue.pop();
	}
	pVolMy->m_eRenderType = TYPE_TRUE;
	
	// �浹üũ �غ�.. 
	PrepareCollisionDetect(pVolMy);
	CollisionDetectMain(pVolMy);
}

void CPvsMgr::SetPriority(CPortalVol * const pVolMy)
{
	CPortalVol* pVol = NULL;
	CPvsBase* pBase = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)))
		{
			pVol = (CPortalVol* )pBase;
			pVol->m_iPriority = -1;
		}
	}

	pVolMy->m_iPriority = 0;
	int igPriority = 0;

// ��������� �켱���� ����..	���� �����ϱ� ��Ȯ�� �Ϸ���.. ù��°�� Wall�� ���� ������ �켱������ ����..
	SetPriorityRecursive(pVolMy, igPriority+1);
}

void CPvsMgr::SetPriorityRecursive(CPortalVol * const pVolMy, int iRecursive)
{
	CPortalVol* pVol = NULL;
	CPvsBase* pBase = NULL;

	iter it = pVolMy->m_pPvsList.begin();
	while(it != pVolMy->m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)))
		{
			pVol = (CPortalVol* )pBase;
			if ( (pVol->m_iPriority == -1) || (pVol->m_iPriority > iRecursive) )	// �켱������ �������� �ʾҰų� ���� �켱�������� ũ��..
			{
				pVol->m_iPriority = iRecursive;
				SetPriorityRecursive(pVol, iRecursive+1);		
			}
		}
	}
}

void CPvsMgr::PrepareCollisionDetect(CPortalVol* const pVol)
{
	CPvsBase* pBase = NULL;
	iter it = pVol->m_pPvsList.begin();
	while(it != pVol->m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalWall)))
		{
			PrepareCollisionDetectOne(pVol, (CPortalWall* )pBase);
		}
	}
}

void CPvsMgr::PrepareCollisionDetectOne(CPortalVol* const pVol, CPortalWall* const pWall)
{
#define SMALL_OFFSET 0.6f;
	int i = 0;

	__Collision Col;
	Col.eWT = pWall->m_eWallType;
	Col.Wvec[0] = pWall->m_pvVertex[0];
	Col.Wvec[1] = pWall->m_pvVertex[1];
	Col.Wvec[2] = pWall->m_pvVertex[2];
	Col.Wvec[3] = pWall->m_pvVertex[3];
	switch (Col.eWT)
	{
		case WALL_ZB:
			for(i = 0; i < 4; i++)
				Col.Wvec[i] = pWall->m_pvVertex[i];	Col.Wvec[i].z = pWall->m_pvVertex[i].z + SMALL_OFFSET;
			break;

		case WALL_ZF:
			for(i = 0; i < 4; i++)
				Col.Wvec[i] = pWall->m_pvVertex[i];	Col.Wvec[i].z = pWall->m_pvVertex[i].z - SMALL_OFFSET;
			break;

		case WALL_XL:
			for(i = 0; i < 4; i++)
				Col.Wvec[i] = pWall->m_pvVertex[i];	Col.Wvec[i].x = pWall->m_pvVertex[i].x + SMALL_OFFSET;
			break;
 
		case WALL_XR:
			for(i = 0; i < 4; i++)
				Col.Wvec[i] = pWall->m_pvVertex[i];	Col.Wvec[i].x = pWall->m_pvVertex[i].x - SMALL_OFFSET;
			break;

		case WALL_YT:
			for(i = 0; i < 4; i++)
				Col.Wvec[i] = pWall->m_pvVertex[i];	Col.Wvec[i].y = pWall->m_pvVertex[i].y - SMALL_OFFSET;
			break;

		case WALL_YB:
			for(i = 0; i < 4; i++)
				Col.Wvec[i] = pWall->m_pvVertex[i];	Col.Wvec[i].y = pWall->m_pvVertex[i].y + SMALL_OFFSET;
			break;
	}

	Col.Wvec[4] = (Col.Wvec[0] + Col.Wvec[1] + Col.Wvec[2] + Col.Wvec[3])/4; 

	PrepareCollisionDetectTwo(pVol, pWall, pWall->m_eWallType, Col);

	m_ColList.push_back(Col);
}

void CPvsMgr::PrepareCollisionDetectTwo(CPortalVol* pVol, CPortalWall* pWall, e_WallType eWT, __Collision& Col)
{
	switch (Col.eWT)
	{
		case WALL_ZB:		// ����..
			Col.Vvec[0] = pVol->m_pvVertex[7];
			Col.Vvec[1] = pVol->m_pvVertex[3];
			Col.Vvec[2] = pVol->m_pvVertex[6];
			Col.Vvec[3] = pVol->m_pvVertex[2];
							// ���� ������ ��..
			Col.Vvec[4] = pWall->m_pvVertex[0];	Col.Vvec[4].z = pVol->m_pvVertex[7].z;
			Col.Vvec[5] = pWall->m_pvVertex[1];	Col.Vvec[5].z = pVol->m_pvVertex[3].z;
			Col.Vvec[6] = pWall->m_pvVertex[2];	Col.Vvec[6].z = pVol->m_pvVertex[6].z;
			Col.Vvec[7] = pWall->m_pvVertex[3];	Col.Vvec[7].z = pVol->m_pvVertex[2].z;
			break;

		case WALL_ZF:		// ����..
			Col.Vvec[0] = pVol->m_pvVertex[5];
			Col.Vvec[1] = pVol->m_pvVertex[1];
			Col.Vvec[2] = pVol->m_pvVertex[4];
			Col.Vvec[3] = pVol->m_pvVertex[0];
							// ���� ������ ��..
			Col.Vvec[4] = pWall->m_pvVertex[0];	Col.Vvec[4].z = pVol->m_pvVertex[5].z;
			Col.Vvec[5] = pWall->m_pvVertex[1];	Col.Vvec[5].z = pVol->m_pvVertex[1].z;
			Col.Vvec[6] = pWall->m_pvVertex[2];	Col.Vvec[6].z = pVol->m_pvVertex[4].z;
			Col.Vvec[7] = pWall->m_pvVertex[3];	Col.Vvec[7].z = pVol->m_pvVertex[0].z;
			break;
				
		case WALL_XL:		// ����..
			Col.Vvec[0] = pVol->m_pvVertex[6];
			Col.Vvec[1] = pVol->m_pvVertex[2];
			Col.Vvec[2] = pVol->m_pvVertex[5];
			Col.Vvec[3] = pVol->m_pvVertex[1];
							// ���� ������ ��..
			Col.Vvec[4] = pWall->m_pvVertex[0];	Col.Vvec[4].x = pVol->m_pvVertex[6].x;
			Col.Vvec[5] = pWall->m_pvVertex[1];	Col.Vvec[5].x = pVol->m_pvVertex[2].x;
			Col.Vvec[6] = pWall->m_pvVertex[2];	Col.Vvec[6].x = pVol->m_pvVertex[5].x;
			Col.Vvec[7] = pWall->m_pvVertex[3];	Col.Vvec[7].x = pVol->m_pvVertex[1].x;
			break;

		case WALL_XR:	// ������..
			Col.Vvec[0] = pVol->m_pvVertex[4];
			Col.Vvec[1] = pVol->m_pvVertex[0];
			Col.Vvec[2] = pVol->m_pvVertex[7];
			Col.Vvec[3] = pVol->m_pvVertex[3];
						// ������ ������ ��..
			Col.Vvec[4] = pWall->m_pvVertex[0];	Col.Vvec[4].x = pVol->m_pvVertex[4].x;
			Col.Vvec[5] = pWall->m_pvVertex[1];	Col.Vvec[5].x = pVol->m_pvVertex[0].x;
			Col.Vvec[6] = pWall->m_pvVertex[2];	Col.Vvec[6].x = pVol->m_pvVertex[7].x;
			Col.Vvec[7] = pWall->m_pvVertex[3];	Col.Vvec[7].x = pVol->m_pvVertex[3].x;
			break;

		case WALL_YT:	// ����
			Col.Vvec[0] = pVol->m_pvVertex[0];
			Col.Vvec[1] = pVol->m_pvVertex[1];
			Col.Vvec[2] = pVol->m_pvVertex[3];
			Col.Vvec[3] = pVol->m_pvVertex[2];
						// ���� ������ ��..
			Col.Vvec[4] = pWall->m_pvVertex[0];	Col.Vvec[4].y = pVol->m_pvVertex[0].y;
			Col.Vvec[5] = pWall->m_pvVertex[1];	Col.Vvec[5].y = pVol->m_pvVertex[1].y;
			Col.Vvec[6] = pWall->m_pvVertex[2];	Col.Vvec[6].y = pVol->m_pvVertex[3].y;
			Col.Vvec[7] = pWall->m_pvVertex[3];	Col.Vvec[7].y = pVol->m_pvVertex[2].y;
			break;

		case WALL_YB:	// �Ʒ���
			Col.Vvec[0] = pVol->m_pvVertex[7];
			Col.Vvec[1] = pVol->m_pvVertex[6];
			Col.Vvec[2] = pVol->m_pvVertex[4];
			Col.Vvec[3] = pVol->m_pvVertex[5];
						// �Ʒ��� ������ ��..
			Col.Vvec[4] = pWall->m_pvVertex[0];	Col.Vvec[4].y = pVol->m_pvVertex[7].y;
			Col.Vvec[5] = pWall->m_pvVertex[1];	Col.Vvec[5].y = pVol->m_pvVertex[6].y;
			Col.Vvec[6] = pWall->m_pvVertex[2];	Col.Vvec[6].y = pVol->m_pvVertex[4].y;
			Col.Vvec[7] = pWall->m_pvVertex[3];	Col.Vvec[7].y = pVol->m_pvVertex[5].y;
			break;
	}

	Col.Vvec[8] = (Col.Vvec[4] + Col.Vvec[5] + Col.Vvec[6] + Col.Vvec[7])/4;
	Col.Vvec[8].x -= 0.04f;	
}

bool CPvsMgr::CollisionDetectMain(CPortalVol* const pVolMy)
{
	__Collision Col;
	itviter ivit;
	CPortalVol* pVol = NULL;
	int i, j;
	__Vector3 vDir;

	iciter icit = m_ColList.begin();
	while( icit != m_ColList.end() )
	{
		Col = *icit++;

		// Volumn�� �ִ� �浹 üũ ����..
		for( i = 0; i < 9; i++ )
		{
			for( j = 0; j < 5; j++ )
			{
				if ( (i == 8) && (j == 4) )
				{
					int k = 98;
				}
				// ������ �����..
				vDir = Col.Wvec[j] - Col.Vvec[i];	vDir.Normalize();

				// �켱 ������� �浹 üũ..
				ivit = m_pVoltList.begin();
				while(ivit != m_pVoltList.end())
				{
					pVol = *ivit++;
					// �ڱ� �ڽ��� �˻����� �ʴ´�..
					if (pVol == pVolMy)
						continue;

// Debug��..
/*					if ( (i == 1) && (j == 1) && (pVol->m_iID == 0) )
					{
						m_dcol.Vvec[0] = Col.Wvec[j];
						m_dcol.Vvec[1] = Col.Vvec[i];
					}*/

					// Portal Wall�� �ִ� ���� Portal Wall�� ���� �˻�����.. �浹���� �ʴ� �鸸 ����.. !!
					switch (CollisionDetectSub(Col.Vvec[i], vDir, pVol))
					{
						case COLLISION_AND_CONTINUE:
							if (pVol->m_eRenderType == TYPE_UNKNOWN)
							{
								pVol->m_eRenderType = TYPE_TRUE;
								TRACE3("id %d, i %d, j %d \n", pVol->m_iID, i, j);
							}
							break;
						case COLLISION_AND_STOP:	// �ش� ���п� ���ؼ��� �˻����� �ʴ´�..
							if (pVol->m_eRenderType == TYPE_UNKNOWN)
							{
								pVol->m_eRenderType = TYPE_TRUE;
								TRACE3("id %d, i %d, j %d \n", pVol->m_iID, i, j);
							}
							goto again;
							break;
						case NO_COLLISION:
							break;
					}
				}
again:;		
			}
		}
	}

	return false;
}

// Debug��..
/*
void CPvsMgr::RenderCollision(__Collision& col)
{
	__VertexColor				pvVertex[9];
	unsigned short				pIndex[9];

	__VertexColor				pvWVertex[4];
	__VertexColor				pvDVertex[8];
	unsigned short				pWIndex[4];
	unsigned short				pDIndex[8];

	for( int i = 0; i < 9; i++ )
	{
		pvVertex[i] = col.Vvec[i];
		pvVertex[i].color = 0xffffffff;
		pIndex[i] = i;
	}
	for( i = 0; i < 4; i++ )
	{
		pvWVertex[i] = col.Wvec[i];
		pvWVertex[i].color = 0xffff0000;
		pWIndex[i] = i;
	}
	for( i = 0; i < 8; i++ )
	{
		pvDVertex[i] = m_dcol.Vvec[i];
		pvDVertex[i].color = 0xffff0000;
		pDIndex[i] = i;
	}

	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	float fPointSize = 8.0f;
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fPointSize));
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
//	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_POINTLIST, 0, 9, 9, pIndex, D3DFMT_INDEX16, pvVertex, sizeof(__VertexColor) );
//	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_POINTLIST, 0, 4, 4, pWIndex, D3DFMT_INDEX16, pvWVertex, sizeof(__VertexColor) );
//	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_POINTLIST, 0, 2, 2, pDIndex, D3DFMT_INDEX16, pvDVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}
*/

e_ReturnCode CPvsMgr::CollisionDetectSub(const __Vector3& vOrig, const __Vector3& vDir, CPortalVol* pVolMy)
{
	// ���ľ� �ɲ���..
// 1. Wall������ �˻��ؼ�..Wall�� �浹 üũ ���� Volumn�� ��� �浹�� ���� ������.. 
// 2. Return COLLISION_AND_STOP;
// 3. �Ѵ� �浹 ������.. �ϴ� COLLISION_AND_CONTINUE�� �����ϰ� �ٸ� Wall�� ��� �˻�..
// 4. COLLISION_AND_CONTINUE�� ���õǾ� ������.. COLLISION_AND_CONTINUE�� return..
// 5. �ƴϸ�..NO_COLLISION�� ����..

#define TM_DEF_MAC(A)	\
{						\
	bColWall = false;	\
	bColVol  = false;	\
	bColWall = CheckPvsWall(vOrig, vDir, pVolMy, A);			\
	bColVol  = CheckPvsVolumnWall(vOrig, vDir, pVolMy, A);		\
	if (bColWall && bColVol)						\
		eRC = COLLISION_AND_CONTINUE;				\
	else if (!bColWall && bColVol)					\
		return COLLISION_AND_STOP;					\
}						

	e_ReturnCode eRC = NO_COLLISION;
	bool bColWall = false;
	bool bColVol  = false;

	TM_DEF_MAC(WALL_ZB)
	TM_DEF_MAC(WALL_ZF)
	TM_DEF_MAC(WALL_XL)
	TM_DEF_MAC(WALL_XR)
	TM_DEF_MAC(WALL_YT)
	TM_DEF_MAC(WALL_YB)

	return eRC;
}

bool CPvsMgr::CheckPvsWall(const __Vector3& vOrig, const __Vector3& vDir, CPortalVol* pVolMy, e_WallType eWT)
{
	__Vector3 vec1, vec2, vec3;
	float ft, fu, fv;

	CPvsBase* pBase = NULL;
	CPortalWall* pWall = NULL;
	iter it = pVolMy->m_pPvsList.begin();
	while(it != pVolMy->m_pPvsList.end())
	{
		pBase = *it++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalWall)))
		{
			pWall = (CPortalWall* )pBase;
			if (pWall->m_eWallType != eWT)
				continue;
			vec1 = pWall->m_pvVertex[0];
			vec2 = pWall->m_pvVertex[1];
			vec3 = pWall->m_pvVertex[2];
			if (IntersectTriangle(vOrig, vDir, vec1, vec2, vec3, ft, fu, fv, NULL))
				return true;
			vec1 = pWall->m_pvVertex[3];
			vec2 = pWall->m_pvVertex[2];
			vec3 = pWall->m_pvVertex[1];
			if (IntersectTriangle(vOrig, vDir, vec1, vec2, vec3, ft, fu, fv, NULL))
				return true;
			vec1 = pWall->m_pvVertex[0];
			vec2 = pWall->m_pvVertex[2];
			vec3 = pWall->m_pvVertex[1];
			if (IntersectTriangle(vOrig, vDir, vec1, vec2, vec3, ft, fu, fv, NULL))
				return true;
			vec1 = pWall->m_pvVertex[3];
			vec2 = pWall->m_pvVertex[1];
			vec3 = pWall->m_pvVertex[2];
			if (IntersectTriangle(vOrig, vDir, vec1, vec2, vec3, ft, fu, fv, NULL))
				return true;
		}
	}

	return false;
}

bool CPvsMgr::CheckPvsVolumnWall(const __Vector3& vOrig, const __Vector3& vDir, CPortalVol* pVolMy, e_WallType eWT)
{
	float ft, fu, fv;
	int i;

	switch (eWT)
	{
		case WALL_ZB:
			i = 6;
			break;
		case WALL_ZF:
			i = 24;
			break;
		case WALL_XL:
			i = 12;
			break;
		case WALL_XR:
			i = 18;
			break;
		case WALL_YT:
			i = 30;
			break;
		case WALL_YB:
			i = 0;
			break;
	}

/*	if ( (pVolMy->m_iID == 8) && (eWT == WALL_ZB) )
	{
		for ( int k = 0; k < 6; k++ )
			m_dcol.Vvec[k+2] = pVolMy->m_pvVertex[pVolMy->m_pIndex[i+k]];
	}*/

	if(IntersectTriangle(vOrig, vDir, pVolMy->m_pvVertex[pVolMy->m_pIndex[i]], pVolMy->m_pvVertex[pVolMy->m_pIndex[i+1]],
		pVolMy->m_pvVertex[pVolMy->m_pIndex[i+2]], ft, fu, fv, NULL))
		return true;
	if(IntersectTriangle(vOrig, vDir, pVolMy->m_pvVertex[pVolMy->m_pIndex[i+3]], pVolMy->m_pvVertex[pVolMy->m_pIndex[i+4]],
		pVolMy->m_pvVertex[pVolMy->m_pIndex[i+5]], ft, fu, fv, NULL))
		return true;

	return false;
}
 
bool CPvsMgr::IntersectTriangle(const __Vector3& vOrig, const __Vector3& vDir,
							  const __Vector3& v0, const __Vector3& v1, const __Vector3& v2,
							  float& fT, float& fU, float& fV, __Vector3* pVCol)
{
    // Find vectors for two edges sharing vert0
    static __Vector3 vEdge1, vEdge2;
	
	vEdge1 = v1 - v0;
    vEdge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    __Vector3 pVec;	float fDet;
	
//	By : Ecli666 ( On 2001-09-12 ���� 10:39:01 )

	pVec.Cross(vEdge1, vEdge2);
	fDet = pVec.Dot(vDir);
	if ( fDet > -0.0001f )
		return false;

//	~(By Ecli666 On 2001-09-12 ���� 10:39:01 )

    pVec.Cross(vDir, vEdge2);

    // If determinant is near zero, ray lies in plane of triangle
    fDet = vEdge1.Dot(pVec);
    if( fDet < 0.0001f )		// ���� 0�� ������ �ﰢ�� ���� �������� ���� �����ϴ�.
        return false;

    // Calculate distance from vert0 to ray origin
    __Vector3 tVec = vOrig - v0;

    // Calculate U parameter and test bounds
    fU = tVec.Dot(pVec);
    if( fU < 0.0f || fU > fDet )
        return false;

    // Prepare to test V parameter
    __Vector3 qVec;
    qVec.Cross(tVec, vEdge1);

    // Calculate V parameter and test bounds
    fV = D3DXVec3Dot( &vDir, &qVec );
    if( fV < 0.0f || fU + fV > fDet )
        return false;

    // Calculate t, scale parameters, ray intersects triangle
    fT = D3DXVec3Dot( &vEdge2, &qVec );
    float fInvDet = 1.0f / fDet;
    fT *= fInvDet;
    fU *= fInvDet;
    fV *= fInvDet;

	// t�� Ŭ���� �ָ� ������ ���� ������ ���� �ִ�.
	// t*dir + orig �� ���ϸ� ������ ���� ���� �� �ִ�.
	// u�� v�� �ǹ̴� �����ϱ�?
	// ���� : v0 (0,0), v1(1,0), v2(0,1) <��ȣ���� (U, V)��ǥ> �̷������� ��� ���� ������ ��Ÿ�� �� ����
	//

	if(pVCol) (*pVCol) = vOrig + (vDir * fT);	// ������ ���..

	// *t < 0 �̸� ����...
	if ( fT < 0.0f )
		return false;

	return true;	
}

CPortalVol* CPvsMgr::GetLinkedVolumnByID(int iID)
{
	CPvsBase* pBase = NULL;
	CPortalVol* pVol = NULL;
	CPvsBase* pLinked = NULL;

	iter itLinked;
	iter itTotal = m_pPvsList.begin();
	while(itTotal != m_pPvsList.end())
	{
		pBase = *itTotal++;	
		if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)))
		{
			pVol = (CPortalVol* )pBase;
			itLinked = pVol->m_pPvsList.begin();
			while(itLinked != pVol->m_pPvsList.end())
			{
				pLinked = *itLinked++;
				if (pLinked->IsKindOf(RUNTIME_CLASS(CPortalWall)) && (pLinked->m_iID == iID))
					return pVol;
			}
		}
	}

	return NULL;
}

void CPvsMgr::CalcCompile()
{
	CPvsBase* pBase = NULL;
	CPortalVol* pVol = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		pBase->SetState(STATE_NONE);
	}

	// Visibility�� �����Ѵ�.. !!!!!!!!
	m_ColList.clear();
	m_pVoltList.clear();

	int i = 0;
	if (m_iCurIndex != -1)
	{
		it = m_pPvsList.begin();
		while(it != m_pPvsList.end())
		{
			if (m_iCurIndex == i)
			{
				pBase = *it;
				if (pBase->IsKindOf(RUNTIME_CLASS(CPortalVol))) 
				{
					pVol = (CPortalVol* )pBase;
					break;
				}
			}
			it++;
			i++;
		}
	}

	pBase = NULL;
	it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if ( pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)) ) 
			((CPortalVol* )pBase)->m_eRenderType = TYPE_UNKNOWN;
	}

	if (pVol)
		ComputeVisibilty(pVol);

	SplitShapeToVolumn();
}

void CPvsMgr::SplitShapeToVolumn()
{
	CN3Shape* pShape;
	pShape = ((CMainFrame*)AfxGetMainWnd())->m_pShapeBg;

	if (!pShape)	
		return;

	//  ��� Shape�� lod ������ �ְ�� �����..
	pShape->Tick();
	pShape->SetMaxLOD();

	// �ǰ���..
	CPvsBase* pBase = NULL;
	CPortalVol* pVol = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pBase = *it++;	
		if ( pBase->IsKindOf(RUNTIME_CLASS(CPortalVol)) ) 
		{
			pVol = (CPortalVol* )pBase;

			// Shape�������� ������ŭ ���鼭.. ���� Transform ��Ű��.. �˻�.. 								
			pVol->SplitAndMakeShape(pShape);
		}
	}
}














