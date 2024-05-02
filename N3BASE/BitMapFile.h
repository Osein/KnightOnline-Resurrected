// BitMapFile.h: interface for the CBitMapFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPFILE_H__F2C24227_F5AE_43B2_8616_2F2E057B59B8__INCLUDED_)
#define AFX_BITMAPFILE_H__F2C24227_F5AE_43B2_8616_2F2E057B59B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBitMapFile
{
protected:
	BITMAPFILEHEADER m_bmfHeader;
	BITMAPINFOHEADER m_bmInfoHeader;

public:
	void* m_pPixels; // 실제 픽셀 데이터
	int Pitch() { return ((int)((m_bmInfoHeader.biWidth*3 + 3)/4))*4; } // 비트맵의 실제 너비(byte 단위)..
	bool Create(int nWidth, int nHeight, int nBPP = 24);
	bool SaveRectToFile(const std::string& szFN, RECT rc);
	void* Pixels(int x = 0, int y = 0);
	BITMAPINFOHEADER * GetBitmapInfoHeader() { return &m_bmInfoHeader;}
	BITMAPFILEHEADER * GetBitmapFileHeader() { return &m_bmfHeader;}
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	bool LoadFromFile(const char* pszFN);
	bool SaveToFile(const char* pszFN);

	int Width() { return m_bmInfoHeader.biWidth; }
	int Height() { return m_bmInfoHeader.biHeight; }
	
	void Release();
	CBitMapFile();
	virtual ~CBitMapFile();

};

#endif // !defined(AFX_BITMAPFILE_H__F2C24227_F5AE_43B2_8616_2F2E057B59B8__INCLUDED_)
