// N3PMesh.h: interface for the CN3PMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3PMesh_h__INCLUDED_)
#define AFX_N3PMesh_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _USE_VERTEXBUFFER			// 선언하면 VertexBuffer와 IndexBuffer를 사용한다.

#include "N3BaseFileAccess.h"

class CN3PMesh : public CN3BaseFileAccess
{
friend class CN3FXPMeshInstance;
friend class CN3PMeshInstance;
friend class CN3PMeshCreate;
friend class CPMeshEditor;			// N3Viewer의 클래스

public:
	struct  __LODCtrlValue
	{
		float	fDist;
		int		iNumVertices;
	};

	// A single edge collapse / vertex split operation.
	struct __EdgeCollapse
	{
	//	float Value;
		int NumIndicesToLose, NumIndicesToChange, NumVerticesToLose;
		//int *IndexChanges;
		int iIndexChanges;	// 포인터 대신 n번째 저장 (0, 1, 2,...)
		int CollapseTo;
		bool	bShouldCollapse;	// 여기서 병합/나누기를 멈추면 구멍이 뚤린다. 다음단계를 더 진행해야함.
	};

protected:

	//int m_iNumMaterials;
	int m_iNumCollapses;			// 삼각형 병합 리스트
	int m_iTotalIndexChanges;
	int *m_pAllIndexChanges; // All the index changes

	// The mesh is an array of materials, and an edge collapse list. The mesh
	// is saved in its lowest level of detail

	__EdgeCollapse	*m_pCollapses;

	// Mesh 정보
#ifdef _USE_VERTEXBUFFER
	LPDIRECT3DVERTEXBUFFER8 m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#else
	__VertexT1*		m_pVertices;
	__VertexT2*		m_pVertices2;
	WORD*			m_pIndices;
#endif
	int m_iMaxNumVertices, m_iMaxNumIndices;
	int m_iMinNumVertices, m_iMinNumIndices;

	__Vector3	m_vMin, m_vMax; // 최대 최소값..
	float		m_fRadius; // 반지름

	// LOD 조정 정보..(거리에 따라서 Vertex의 수를 조정할 수 있다.)
	int					m_iLODCtrlValueCount;
	__LODCtrlValue*		m_pLODCtrlValues;
protected:
#ifdef _N3TOOL
	static int SortByDistance(const void *pArg1, const void *pArg2);
#endif // end of _N3TOOL

public:
#ifdef _N3TOOL
	void ReGenerateSharpNormal();
	void ReGenerateSmoothNormal();
	int LODCtrlCount() { return m_iLODCtrlValueCount; }
	void LODCtrlSet(__LODCtrlValue* pLODCtrls, int nCount);
	__LODCtrlValue* LODCtrlGet(int index) { if(index < 0 || index >= m_iLODCtrlValueCount) return NULL; return &m_pLODCtrlValues[index]; }
	int CollapsesCount() { return m_iNumCollapses; }
#endif // end of _N3TOOL

//	int					GetLODCtrlValueCount() const {return m_iLODCtrlValueCount;}
//	__PMLODCtrlValue*	GetLODCtrlValue(int iIndex) const {if (iIndex>=0&&m_iLODCtrlValueCount>iIndex)return m_pLODCtrlValues+iIndex; return NULL;}
//	void				SetLODCtrlValues(__PMLODCtrlValue* pValues) {m_pLODCtrlValues = pValues;}

	__Vector3 Min() { return m_vMin; } // 월드 행렬을 적용하지 않은상태의 최소값이다.
	__Vector3 Max() { return m_vMax; } // 월드 행렬을 적용하지 않은상태의 최대값이다.

	void Release();
	HRESULT Create(int iNumVertices, int iNumIndices);
	HRESULT GenerateSecondUV();
	
	bool Load(HANDLE hFile);
#ifdef _N3TOOL
	bool Save(HANDLE hFile);
#endif // end of _N3TOOL
	
	void FindMinMax();

	float Radius() { return m_fRadius; }
	int	GetMinNumVertices() const {return m_iMinNumVertices;};
	int	GetMaxNumVertices() const {return m_iMaxNumVertices;};
	int GetMaxNumIndices() const {return m_iMaxNumIndices;};
	int GetMinNumIndices() const {return m_iMinNumIndices;};

#ifdef _USE_VERTEXBUFFER
	LPDIRECT3DVERTEXBUFFER8	GetVertexBuffer() const {return m_pVB;}
	LPDIRECT3DINDEXBUFFER8	GetIndexBuffer() const {return m_pIB;}
#else
	__VertexT1*		GetVertices() const { return m_pVertices; };
	__VertexT2*		GetVertices2() const { return m_pVertices2; };
	WORD*			GetIndices() const { return m_pIndices;};		// 제대로된 Index가 아님 
	// (제대로 된 인덱스를 얻으려면 N3PMeshInstance로 만든후 LOD조정후 인덱스값을 얻으면 된다.)
#endif

#ifdef _N3TOOL
	void CopyMesh(CN3PMesh* pSrcPMesh);
#endif // end of _N3TOOL

	CN3PMesh();
	virtual ~CN3PMesh();
};

#endif // !defined(AFX_N3PMesh_h__INCLUDED_)
