// RiverMesh.h: interface for the CRiverMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIVERMESH_H__473FE46A_9CD7_4D5C_8196_45F0BFA94F04__INCLUDED_)
#define AFX_RIVERMESH_H__473FE46A_9CD7_4D5C_8196_45F0BFA94F04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//const int MAXNUM_RIVERMESH_VERTEX = 2048;
#define MAX_RIVERMESH_LINE		200
#define MAX_RIVERMESH_VERTEX	200*4
#define MAX_RIVERMESH_INDEX		199*18
#define INDEXBUFFER_PERLINE		18

#include "../N3Base/N3BaseFileAccess.h"

class CRiverMesh : public CN3BaseFileAccess
{
public:
	CRiverMesh();
	virtual ~CRiverMesh();

// Attributes
public:
	int				GetRiverID() const {return m_iRiverID;}
	void			SetRiverID(int iRiverID) {m_iRiverID = iRiverID;}
	float			GetSpeed1() const {return m_fSpeed1;}
	float			GetSpeed2() const {return m_fSpeed2;}
	void			SetSpeed1(float fSpeed) {m_fSpeed1 = fSpeed;}
	void			SetSpeed2(float fSpeed) {m_fSpeed2 = fSpeed;}
	float			GetMeterPerU() const {return m_fMeterPerU;}
	void			SetMeterPerU(float fMeterPerU) {m_fMeterPerU = fMeterPerU;}
	float			GetMeterPerV() const {return m_fMeterPerV;}
	void			SetMeterPerV(float fMeterPerV) {m_fMeterPerV = fMeterPerV;}
	float			GetMeterPerU2() const {return m_fMeterPerU2;}
	void			SetMeterPerU2(float fMeterPerU) {m_fMeterPerU2 = fMeterPerU;}
	float			GetMeterPerV2() const {return m_fMeterPerV2;}
	void			SetMeterPerV2(float fMeterPerV) {m_fMeterPerV2 = fMeterPerV;}
	CN3Texture*		TexGet() const {return m_pTexture;}
	int				VertexCount() const {return m_iVC;}
	int				IndexCount() const {return m_iIC;}
	int				GetAnimTexCount() const {return m_iAnimTextureCount;}
	CN3Texture*		AnimTexGet(int iIndex) const { if (iIndex<0 || iIndex>=m_iAnimTextureCount) return NULL; return m_pAnimTextures[iIndex];}
	float			GetAnimTexFPS() const {return m_fAnimTexFPS;}
	void			SetAnimTexFPS(float fFPS) {m_fAnimTexFPS = fFPS;}
	DWORD			GetAlphaFactor() const {return m_dwAlphaFactor;}
	void			SetAlphaFactor(DWORD dwFactor) {m_dwAlphaFactor = dwFactor;}
protected:
	int				m_iRiverID;
	__VertexXyzT2	m_pVertices[MAX_RIVERMESH_VERTEX];
	WORD			m_wIndex[MAX_RIVERMESH_INDEX];
	int				m_iIC;					// Index Buffer Count.
	int				m_iVC;					// Vertex Count.
	CN3Texture*		m_pTexture;
	int				m_iAnimTextureCount;	// Animation�Ǵ� �ؽ��� ����
	CN3Texture**	m_pAnimTextures;		// Animation�Ǵ� �ؽ��� �����͵�..
	float			m_fSpeed1;			// ���� : �ʴ� v��ǥ�� ��ȭ��
	float			m_fSpeed2;			// ���� : �ʴ� v2��ǥ�� ��ȭ��
	float			m_fMeterPerU;		// U��ǥ 1.0�� �ش��ϴ� ���� ����
	float			m_fMeterPerV;		// V��ǥ 1.0�� �ش��ϴ� ���� ����
	float			m_fMeterPerU2;		// U2��ǥ 1.0�� �ش��ϴ� ���� ����
	float			m_fMeterPerV2;		// V2��ǥ 1.0�� �ش��ϴ� ���� ����
	float			m_fAnimTexFPS;		// AnimTexture�� �ʴ� frame ��;
	DWORD			m_dwAlphaFactor;	// ���� �����ϰ� �ϱ� ���� ���İ�

// Operations
public:
	virtual void	Release();
	virtual bool	Load(HANDLE hFile);
	virtual bool	Save(HANDLE hFile);
	void			Render();
	void			RenderVertexPoint();	// �ߺ��̰� ���� �ٽ� �׸���

	int				AddVertex(__Vector3& vPos1, __Vector3& vPos2, __Vector3& vPos3, __Vector3& vPos4);
	int				AddVertex();
	int				DeleteVertex(int iIndex);	// ������ ����� �ڿ� �ִ� �͵��� ������ �ΰ��� �̵�
	__VertexXyzT2*	GetVertex(int iIndex) {if (iIndex<0 || iIndex>=m_iVC) return NULL; return m_pVertices+iIndex;}
	__Vector3		GetCenter();

	BOOL			SetTextureName(LPCTSTR pszFName);
	BOOL			SetAnimTextureName(LPCTSTR pszFName, LPCTSTR pszExt, int iCount);
	void			ReCalcUV();
protected:
	void			ReleaseAnimTextures();
};

#endif // !defined(AFX_RIVERMESH_H__473FE46A_9CD7_4D5C_8196_45F0BFA94F04__INCLUDED_)
