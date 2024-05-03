#include "StdAfx.h"
#include "N3ShapeMgr.h"

CN3ShapeMgr::CN3ShapeMgr()
{
	m_fMapWidth = 0.0f;
	m_fMapLength = 0.0f;
	m_nCollisionFaceCount = 0;
	m_pvCollisions = NULL;
	memset(m_pCells, 0, sizeof(m_pCells));
}

CN3ShapeMgr::~CN3ShapeMgr()
{
	delete [] m_pvCollisions; m_pvCollisions = NULL;
	for(int z = 0; z < MAX_CELL_MAIN; z++)
	{
		for(int x = 0; x < MAX_CELL_MAIN; x++)
		{
			delete m_pCells[x][z];
		}
	}
}

void CN3ShapeMgr::Release()
{
	m_fMapWidth = 0.0f;
	m_fMapLength = 0.0f;
	m_nCollisionFaceCount = 0;
	delete [] m_pvCollisions; m_pvCollisions = NULL;
	for(int z = 0; z < MAX_CELL_MAIN; z++)
	{
		for(int x = 0; x < MAX_CELL_MAIN; x++)
		{
			delete m_pCells[x][z];
			m_pCells[x][z] = NULL;
		}
	}
	memset(m_pCells, 0, sizeof(MAX_CELL_MAIN));
}

bool CN3ShapeMgr::LoadCollisionData(HANDLE hFile)
{
	DWORD dwRWC;
	ReadFile(hFile, &m_fMapWidth, 4, &dwRWC, NULL); // Shape Count
	ReadFile(hFile, &m_fMapLength, 4, &dwRWC, NULL); // Shape Count
	this->Create(m_fMapWidth, m_fMapLength);

	// 충돌 체크 폴리곤 데이터 읽기..
	ReadFile(hFile, &m_nCollisionFaceCount, 4, &dwRWC, NULL);
	delete [] m_pvCollisions; m_pvCollisions = NULL;
	if(m_nCollisionFaceCount > 0)
	{
		m_pvCollisions = new __Vector3[m_nCollisionFaceCount * 3];
		ReadFile(hFile, m_pvCollisions, sizeof(__Vector3) * m_nCollisionFaceCount * 3, &dwRWC, NULL);
	}

	// Cell Data 쓰기.
	BOOL bExist = FALSE;
	int z = 0;
	for(float fZ = 0.0f; fZ < m_fMapLength; fZ += CELL_MAIN_SIZE, z++)
	{
		int x = 0;
		for(float fX = 0.0f; fX < m_fMapWidth;  fX += CELL_MAIN_SIZE, x++)
		{
			delete m_pCells[x][z]; m_pCells[x][z] = NULL;

			ReadFile(hFile, &bExist, 4, &dwRWC, NULL); // 데이터가 있는 셀인지 쓰고..
			if(FALSE == bExist) continue;

			m_pCells[x][z] = new __CellMain;
			m_pCells[x][z]->Load(hFile);
		}
	}

	return true;
}

bool CN3ShapeMgr::Create(float fMapWidth, float fMapLength) // 맵의 너비와 높이를 미터 단위로 넣는다..
{
	if(	fMapWidth <= 0.0f || fMapWidth > MAX_CELL_MAIN * CELL_MAIN_SIZE ||
		fMapLength <= 0.0f || fMapLength > MAX_CELL_MAIN * CELL_MAIN_SIZE )
	{
		return false;
	}

	m_fMapWidth = fMapWidth;
	m_fMapLength = fMapLength;

	return true;
}

bool CN3ShapeMgr::CheckCollision(	const __Vector3& vPos,		// 충돌 위치
									const __Vector3& vDir,		// 방향 벡터
									float fSpeedPerSec,			// 초당 움직이는 속도
									__Vector3* pvCol,			// 충돌 지점
									__Vector3* pvNormal,		// 충돌한면의 법선벡터
									__Vector3* pVec)			// 충돌한 면 의 폴리곤 __Vector3[3]
{
	if(fSpeedPerSec <= 0) return false; // 움직이는 속도가 없거나 반대로 움직이면 넘어간다..
	static __CellSub* ppCells[128];
	__Vector3 vPosNext = vPos + (vDir* fSpeedPerSec); // 다음 위치
	int nSubCellCount = this->SubCellPathThru(vPos, vPosNext, ppCells); // 통과하는 서브셀을 가져온다..
	if(nSubCellCount <= 0 || nSubCellCount > 128) return false; // 없음 말자.

	__Vector3 vColTmp(0,0,0);
	int nIndex0, nIndex1, nIndex2;
	static float fT, fU, fV, fDistTmp, fDistClosest;
	fDistClosest = FLT_MAX;

	for ( int i = 0; i < nSubCellCount; i++ )
	{
		if ( ppCells[i]->nCCPolyCount <= 0 ) continue;

		for ( int j = 0; j < ppCells[i]->nCCPolyCount; j++ )
		{
			nIndex0 = ppCells[i]->pdwCCVertIndices[j*3];
			nIndex1 = ppCells[i]->pdwCCVertIndices[j*3+1];
			nIndex2 = ppCells[i]->pdwCCVertIndices[j*3+2];
			
			if(false == ::_IntersectTriangle(vPos, vDir, m_pvCollisions[nIndex0], m_pvCollisions[nIndex1], m_pvCollisions[nIndex2], fT, fU, fV, &vColTmp)) continue;
			if(false == ::_IntersectTriangle(vPosNext, vDir, m_pvCollisions[nIndex0], m_pvCollisions[nIndex1], m_pvCollisions[nIndex2]))
			{
				fDistTmp = (vPos - vColTmp).Magnitude(); // 거리를 재보고..
				if(fDistTmp < fDistClosest) 
				{
					fDistClosest = fDistTmp;
					// 충돌이다..
					if(pvCol) *pvCol = vColTmp;
					if(pvNormal)
					{
						(*pvNormal).Cross(m_pvCollisions[nIndex1] - m_pvCollisions[nIndex0], 
							m_pvCollisions[nIndex2] - m_pvCollisions[nIndex0]);
						(*pvNormal).Normalize();
					}
					if ( pVec )
					{
						pVec[0] = m_pvCollisions[nIndex0];
						pVec[1] = m_pvCollisions[nIndex1];
						pVec[2] = m_pvCollisions[nIndex2];
					}
				}
			}
		}
	}
	
	if(fDistClosest != FLT_MAX)
		return true;

	return false;
}

int CN3ShapeMgr::SubCellPathThru(const __Vector3& vFrom, const __Vector3& vAt, __CellSub** ppSubCells) // 벡터 사이에 걸친 셀포인터 돌려준다..
{
	if(NULL == ppSubCells) return 0;

	// 범위를 정하고..
	int xx1 = 0, xx2 = 0, zz1 = 0, zz2 = 0;

	if(vFrom.x < vAt.x) { xx1 = (int)(vFrom.x / CELL_SUB_SIZE); xx2 = (int)(vAt.x / CELL_SUB_SIZE); }
	else { xx1 = (int)(vAt.x / CELL_SUB_SIZE); 	xx2 = (int)(vFrom.x / CELL_SUB_SIZE); }

	if(vFrom.z < vAt.z)	{ zz1 = (int)(vFrom.z / CELL_SUB_SIZE); zz2 = (int)(vAt.z / CELL_SUB_SIZE); }
	else { zz1 = (int)(vAt.z / CELL_SUB_SIZE); zz2 = (int)(vFrom.z / CELL_SUB_SIZE); }

	bool bPathThru;
	float fZMin, fZMax, fXMin, fXMax;
	int nSubCellCount = 0;
	for(int z = zz1; z <= zz2; z++) // 범위만큼 처리..
	{
		fZMin = (float)(z * CELL_SUB_SIZE);
		fZMax = (float)((z+1) * CELL_SUB_SIZE);
		for(int x = xx1; x <= xx2; x++)
		{
			fXMin = (float)(x * CELL_SUB_SIZE);
			fXMax = (float)((x+1) * CELL_SUB_SIZE);

			// Cohen thuderland algorythm
			DWORD dwOC0 = 0, dwOC1 = 0; // OutCode 0, 1
			if(vFrom.z > fZMax) dwOC0 |= 0xf000;
			if(vFrom.z < fZMin) dwOC0 |= 0x0f00;
			if(vFrom.x > fXMax) dwOC0 |= 0x00f0;
			if(vFrom.x < fXMin) dwOC0 |= 0x000f;
			if(vAt.z > fZMax) dwOC1 |= 0xf000;
			if(vAt.z < fZMin) dwOC1 |= 0x0f00;
			if(vAt.x > fXMax) dwOC1 |= 0x00f0;
			if(vAt.x < fXMin) dwOC1 |= 0x000f;
			
			bPathThru = false;
			if(dwOC0 & dwOC1) bPathThru = false; // 두 끝점이 같은 변의 외부에 있다.
			else if(dwOC0 == 0 && dwOC1 == 0) bPathThru = true;// 선분이 사각형 내부에 있음
			else if((dwOC0 == 0 && dwOC1 != 0) || (dwOC0 != 0 && dwOC1 == 0)) bPathThru = true;// 선분 한점은 셀의 내부에 한점은 외부에 있음.
			else if((dwOC0 & dwOC1) == 0) // 두 L점 모두 셀 외부에 있지만 판단을 다시 해야 한다.
			{
				float fXCross = vFrom.x + (fZMax - vFrom.z) * (vAt.x - vFrom.x) / (vAt.z - vFrom.z); // 위의 변과의 교차점을 계산하고..
				if(fXCross < fXMin) bPathThru = false; // 완전히 외곽에 있다.
				else bPathThru = true; // 걸처있다.
			}

			if(false == bPathThru) continue;

			// 충돌 정보를 써야 한다..
			int nX = x / CELL_MAIN_DEVIDE;
			int nZ = z / CELL_MAIN_DEVIDE;
			if(nX < 0 || nX >= MAX_CELL_MAIN || nZ < 0 && nZ >= MAX_CELL_MAIN) continue; // 메인셀바깥에 있음 지나간다.
			if(NULL == m_pCells[nX][nZ]) continue; // 메인셀이 널이면 지나간다..

			int nXSub = x%CELL_MAIN_DEVIDE;
			int nZSub = z%CELL_MAIN_DEVIDE;

			ppSubCells[nSubCellCount] = &(m_pCells[nX][nZ]->SubCells[nXSub][nZSub]);
			nSubCellCount++;
		} // end of for(int x = xx1; x <= xx2; x++)
	} // end of for(int z = zz1; z <= zz2; z++) // 범위만큼 처리..

	return nSubCellCount; // 걸친 셀 포인터 돌려주기..
}

float CN3ShapeMgr::GetHeightNearstPos(const __Vector3 &vPos, __Vector3* pvNormal) // 가장 가까운 높이값을 돌려준다. 없으면 -FLT_MAX 을 돌려준다.
{
	__CellSub* pCell = this->SubCell(vPos.x, vPos.z); // 서브셀을 가져온다..
	if(NULL == pCell || pCell->nCCPolyCount <= 0) return -FLT_MAX; // 없음 말자.

	__Vector3 vPosV = vPos; vPosV.y = 5000.0f; // 꼭대기에 위치를 하고..
	__Vector3 vDir(0,-1, 0); // 수직 방향 벡터
	__Vector3 vColTmp(0,0,0); // 최종적으로 가장 가까운 충돌 위치..

	int nIndex0, nIndex1, nIndex2;
	float fT, fU, fV;
	float fNearst = FLT_MAX, fMinTmp = 0, fHeight = -FLT_MAX;		// 일단 최소값을 큰값으로 잡고..

	for ( int i = 0; i < pCell->nCCPolyCount; i++ )
	{
		nIndex0 = pCell->pdwCCVertIndices[i*3];
		nIndex1 = pCell->pdwCCVertIndices[i*3+1];
		nIndex2 = pCell->pdwCCVertIndices[i*3+2];
		
		// 충돌된 점이 있으면..
		if(true == ::_IntersectTriangle(vPosV, vDir, m_pvCollisions[nIndex0], m_pvCollisions[nIndex1], m_pvCollisions[nIndex2], fT, fU, fV, &vColTmp))
		{
			fMinTmp = (vColTmp - vPos).Magnitude();
			if(fMinTmp < fNearst) // 가장 가까운 충돌 위치를 찾기 위한 코드..
			{
				fNearst = fMinTmp;
				fHeight = vColTmp.y; // 높이값.

				if(pvNormal)
				{
					pvNormal->Cross(m_pvCollisions[nIndex1] - m_pvCollisions[nIndex0], m_pvCollisions[nIndex2] - m_pvCollisions[nIndex0]);
					pvNormal->Normalize();
				}
			}
		}
	}

	return fHeight;
}

float CN3ShapeMgr::GetHeight(float fX, float fZ, __Vector3* pvNormal) // 가장 높은 곳을 돌려준다.. 없으면 -FLT_MAX값을 돌려준다.
{
	__CellSub* pCell = this->SubCell(fX, fZ); // 서브셀을 가져온다..
	if(NULL == pCell || pCell->nCCPolyCount <= 0) return -FLT_MAX; // 없음 말자.

	__Vector3 vPosV(fX, 5000.0f, fZ); // 꼭대기에 위치를 하고..
	__Vector3 vDir(0,-1, 0); // 수직 방향 벡터
	__Vector3 vColTmp(0,0,0); // 최종적으로 가장 가까운 충돌 위치..

	int nIndex0, nIndex1, nIndex2;
	float fT, fU, fV;
	float fMaxTmp = -FLT_MAX;;

	for ( int i = 0; i < pCell->nCCPolyCount; i++ )
	{
		nIndex0 = pCell->pdwCCVertIndices[i*3];
		nIndex1 = pCell->pdwCCVertIndices[i*3+1];
		nIndex2 = pCell->pdwCCVertIndices[i*3+2];
		
		// 충돌된 점이 있으면..
		if(true == ::_IntersectTriangle(vPosV, vDir, m_pvCollisions[nIndex0], m_pvCollisions[nIndex1], m_pvCollisions[nIndex2], fT, fU, fV, &vColTmp))
		{
			if(vColTmp.y > fMaxTmp)
			{
				fMaxTmp = vColTmp.y;
				if(pvNormal)
				{
					pvNormal->Cross(m_pvCollisions[nIndex1] - m_pvCollisions[nIndex0], m_pvCollisions[nIndex2] - m_pvCollisions[nIndex0]);
					pvNormal->Normalize();
				}
			}
		}
	}

	return fMaxTmp;
}

void CN3ShapeMgr::SubCell(const __Vector3& vPos, __CellSub** ppSubCell)			// 해당 위치의 셀 포인터를 돌려준다.
{
	int x = (int)(vPos.x / CELL_MAIN_SIZE);
	int z = (int)(vPos.z / CELL_MAIN_SIZE);
	
	__ASSERT(x >= 0 && x < MAX_CELL_MAIN && z >= 0 && z < MAX_CELL_MAIN, "Invalid cell number");

	int xx = (((int)vPos.x)%CELL_MAIN_SIZE)/CELL_SUB_SIZE;			// 2, 3, 4
	int zz = (((int)vPos.z)%CELL_MAIN_SIZE)/CELL_SUB_SIZE;			// 1, 0, 5
																	// 8, 7, 6	
	for ( int i = 0; i < 9; i++ )
	{
		switch( i )
		{
			case 0:
				if ( m_pCells[x][z] != NULL )
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx][zz]);
				else
					ppSubCell[i] = NULL;
				break;

			case 1:
				if ( (x == 0) && (xx == 0) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (x != 0) && (xx == 0) )
				{
					if ( m_pCells[x-1][z] != NULL )
						ppSubCell[i] = &(m_pCells[x-1][z]->SubCells[CELL_MAIN_DEVIDE-1][zz]);
					else
						ppSubCell[i] = NULL;
					break;
				}

				if ( m_pCells[x][z] != NULL )
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx-1][zz]);
				else
					ppSubCell[i] = NULL;
				break;

			case 2:
				if ( (x == 0) && (xx == 0) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (z == (CELL_MAIN_SIZE-1)) && ( zz == (CELL_MAIN_DEVIDE-1) ) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (x != 0) && (xx == 0) )											// x 감소, z 증가.
				{
					if ( (z != (MAX_CELL_MAIN-1)) && ( zz == (CELL_MAIN_DEVIDE-1) ) )
						if ( m_pCells[x-1][z+1] != NULL )
							ppSubCell[i] = &(m_pCells[x-1][z+1]->SubCells[CELL_MAIN_DEVIDE-1][0]);
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x-1][z] != NULL )
							ppSubCell[i] = &(m_pCells[x-1][z]->SubCells[CELL_MAIN_DEVIDE-1][zz+1]);
						else
							ppSubCell[i] = NULL;
					break;
				}

				if ( (z != (MAX_CELL_MAIN-1)) && (zz == (CELL_MAIN_DEVIDE-1) ) )		// x 감소, z 증가.
				{
					if ( (x != 0) && (xx == 0) )
						if ( m_pCells[x-1][z+1] != NULL )
							ppSubCell[i] = &(m_pCells[x-1][z+1]->SubCells[CELL_MAIN_DEVIDE-1][0]);	
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x][z+1] != NULL )
							ppSubCell[i] = &(m_pCells[x][z+1]->SubCells[xx-1][0]);	
						else
							ppSubCell[i] = NULL;
					break;
				}
							
				if ( m_pCells[x][z] != NULL )
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx-1][zz+1]);						
				else
					ppSubCell[i] = NULL;					
					break;

			case 3:
				if ( (z == (MAX_CELL_MAIN-1)) && (zz == (CELL_MAIN_DEVIDE-1)) )			// z 증가.
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (z != (MAX_CELL_MAIN-1)) && (zz == (CELL_MAIN_DEVIDE-1)) )
				{
					if ( m_pCells[x-1][z] != NULL )
						ppSubCell[i] = &(m_pCells[x-1][z]->SubCells[xx][0]);
					else
						ppSubCell[i] = NULL;
					break;
				}

				if ( m_pCells[x][z] != NULL )
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx][zz+1]);
				else
					ppSubCell[i] = NULL;					
				break;

			case 4:
				if ( (x == (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )			// x 증가, z 증가.
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (z == (MAX_CELL_MAIN-1)) && ( zz == (CELL_MAIN_DEVIDE-1)) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (x != (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )
				{
					if ( (z != (MAX_CELL_MAIN-1)) && ( zz == (CELL_MAIN_DEVIDE-1)) )
						if ( m_pCells[x+1][z+1] != NULL )
							ppSubCell[i] = &(m_pCells[x+1][z+1]->SubCells[0][0]);
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x+1][z] != NULL )
							ppSubCell[i] = &(m_pCells[x+1][z]->SubCells[0][zz+1]);
						else
							ppSubCell[i] = NULL;
					break;
				}

				if ( (z != (MAX_CELL_MAIN-1)) && (zz == (CELL_MAIN_DEVIDE-1)) )
				{
					if ( (x != (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )
						if ( m_pCells[x+1][z+1] != NULL )
							ppSubCell[i] = &(m_pCells[x+1][z+1]->SubCells[0][0]);	
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x][z+1] != NULL )
							ppSubCell[i] = &(m_pCells[x][z+1]->SubCells[xx+1][0]);	
						else
							ppSubCell[i] = NULL;
					break;
				}

				if ( m_pCells[x][z] != NULL )								
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx+1][zz+1]);						
				else
					ppSubCell[i] = NULL;					
				break;

			case 5:																		// x 증가.
				if ( (x == (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (x != (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )
				{
					if ( m_pCells[x+1][z] != NULL )
						ppSubCell[i] = &(m_pCells[x+1][z]->SubCells[0][zz]);
					else
						ppSubCell[i] = NULL;
					break;
				}

				if ( m_pCells[x][z] != NULL )								
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx+1][zz]);
				else
					ppSubCell[i] = NULL;					
				break;

			case 6:																		// x 증가. z 감소.		
				if ( (x == (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (z == 0) && (zz == 0) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (x != (MAX_CELL_MAIN-1)) && (xx == (CELL_MAIN_DEVIDE-1)) )			
				{
					if ( (z != 0) && (zz == 0) )
						if ( m_pCells[x+1][z-1] != NULL )								
							ppSubCell[i] = &(m_pCells[x+1][z-1]->SubCells[0][CELL_MAIN_DEVIDE-1]);
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x+1][z] != NULL )								
							ppSubCell[i] = &(m_pCells[x+1][z]->SubCells[0][zz-1]);
						else
							ppSubCell[i] = NULL;
					break;
				}

				if ( (z != 0) && (zz == 0) )
				{
					if ( (x != (CELL_MAIN_SIZE-1)) && (xx == (CELL_MAIN_DEVIDE-1) ) )
						if ( m_pCells[x+1][z-1] != NULL )								
							ppSubCell[i] = &(m_pCells[x+1][z-1]->SubCells[0][CELL_MAIN_DEVIDE-1]);
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x][z-1] != NULL )								
							ppSubCell[i] = &(m_pCells[x][z-1]->SubCells[xx+1][CELL_MAIN_DEVIDE-1]);	
						else
							ppSubCell[i] = NULL;
					break;
				}

				if ( m_pCells[x][z] != NULL )								
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx+1][zz-1]);						
				else
					ppSubCell[i] = NULL;					
				break;

			case 7:																		// z 감소.
				if ( (z == 0) && (zz == 0) )	
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (z != 0) && (zz == 0) )
				{
					if ( m_pCells[x][z-1] != NULL )								
						ppSubCell[i] = &(m_pCells[x][z-1]->SubCells[xx][CELL_MAIN_DEVIDE-1]);
					else
						ppSubCell[i] = NULL;					
					break;
				}

				if ( m_pCells[x][z] != NULL )								
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx][zz-1]);
				else
					ppSubCell[i] = NULL;					
				break;

			case 8:																		// x 감소, z 감소.
				if ( (x == 0) && (xx == 0) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (z == 0) && (zz == 0) )
				{
					ppSubCell[i] = NULL;
					break;
				}

				if ( (x != 0) && (xx == 0) )
				{
					if ( (z != 0) && (zz == 0) )
						if ( m_pCells[x-1][z-1] != NULL )								
							ppSubCell[i] = &(m_pCells[x-1][z-1]->SubCells[CELL_MAIN_DEVIDE-1][CELL_MAIN_DEVIDE-1]);
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x-1][z] != NULL )								
							ppSubCell[i] = &(m_pCells[x-1][z]->SubCells[CELL_MAIN_DEVIDE-1][zz-1]);
						else
							ppSubCell[i] = NULL;
					break;
				}

				if ( (z != 0) && (zz == 0) )
				{
					if ( (x != 0) && (xx == 0) )
						if ( m_pCells[x-1][z-1] != NULL )								
							ppSubCell[i] = &(m_pCells[x-1][z-1]->SubCells[CELL_MAIN_DEVIDE-1][CELL_MAIN_DEVIDE-1]);
						else
							ppSubCell[i] = NULL;
					else
						if ( m_pCells[x][z-1] != NULL )								
							ppSubCell[i] = &(m_pCells[x][z-1]->SubCells[xx-1][CELL_MAIN_DEVIDE-1]);	
						else
							ppSubCell[i] = NULL;
					break;
				}
							
				if ( m_pCells[x][z] != NULL )								
					ppSubCell[i] = &(m_pCells[x][z]->SubCells[xx-1][zz-1]);						
				else
					ppSubCell[i] = NULL;
				break;
		}	// switch
	}	// for 
}

