// N3Eng.h: interface for the CN3Eng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Engine_h__INCLUDED_)
#define AFX_N3Engine_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3BaseFileAccess.h"
#include "ddraw.h"

class CN3Eng : public CN3Base
{
public:
	typedef struct __D3DDEV_INFO
	{
		char	szDeviceName[128];
		char	szDeviceDesc[128];
		int		nAdapter; // ���° �׷��� ī������.
		int		nDevice; // ���° ��ġ����.
		
		D3DDEVTYPE		DevType; // �ϵ���� ��������.
		int				nModeCount; // ���÷��� ��� ����
		D3DDISPLAYMODE* pModes; // ���÷��� ���
	} __D3DDevInfo;

	LPDIRECT3D8			m_lpD3D;
	LPDIRECTDRAW		m_lpDD;

protected:
	int		m_nModeActive; // ���� ���õ� Mode
	int		m_nAdapterCount; // �׷��� ī�� ����
	__D3DDEV_INFO m_DeviceInfo; // Device ����	
	
public:
	
	HKEY RegistryOpen(const std::string& szKey);
	bool RegistryClose(HKEY& hKey);
	bool RegistryValueGet(HKEY hKey, const std::string& szName, std::string& szValue);
	bool RegistryValueGet(HKEY hKey, const std::string& szName, void* pValue, DWORD dwBytes);
	bool RegistryValueSet(HKEY hKey, const std::string& szName, std::string& szValue);
	bool RegistryValueSet(HKEY hKey, const std::string& szName, void* pValue, DWORD dwBytes);
	
	void SetDefaultEnvironment();
	bool Reset(BOOL bWindowed, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);
	static void Clear(D3DCOLOR crFill, RECT* pRC = NULL);
	static void ClearAuto(RECT* pRC = NULL);
	static void ClearZBuffer(const RECT* pRC = NULL);
	static void Present(HWND hWnd, RECT* pRC = NULL);

	void LookAt(__Vector3& vEye, __Vector3& vAt, __Vector3& vUp);
	void SetProjection(float fNear, float fFar, float fLens, float fAspect);
	void SetViewPort(RECT& pRC);

	void Release();

//	void InitEnv();
	bool Init(BOOL bWindowed, HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, BOOL bUseHW);
	BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat);

	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	CN3Eng();
	virtual ~CN3Eng();
};

#endif // !defined(AFX_N3Engine_h__INCLUDED_)
