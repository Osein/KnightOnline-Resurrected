// UIEDoc.cpp : implementation of the CUIEDoc class
//

#include "stdafx.h"
#include "UIE.h"

#include "UIEDoc.h"
#include "../N3Base/N3UIBase.h"
#include "../N3Base/N3UIImage.h"
#include "../N3Base/N3UIString.h"
#include "../N3Base/N3UIButton.h"
#include "../N3Base/N3UIStatic.h"
#include "../N3Base/N3UIEdit.h"
#include "../N3Base/N3UIProgress.h"
#include "../N3Base/N3UITrackBar.h"
#include "../N3Base/N3UIToolTip.h"
#include "../N3Base/N3UIScrollBar.h"
#include "../N3Base/N3Texture.h"
#include "../N3Base/N3UIArea.h"
#include "../N3Base/N3UIList.h"
#ifdef _REPENT
#include "../N3Base/N3UIIconSlot.h"
#endif

#include "DlgReplace.h"
#include "DlgTexture.h"
#include "MainFrm.h"
#include "UIEView.h"
#include "DlgChangeImage.h"
#include "DlgUnusedFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIEDoc

IMPLEMENT_DYNCREATE(CUIEDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIEDoc, CDocument)
	//{{AFX_MSG_MAP(CUIEDoc)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_IMPORT_TOOLTIP, OnFileImportTooltip)
	ON_COMMAND(ID_FILE_EXPORT_TOOLTIP, OnFileExportTooltip)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_TOOLTIP, OnUpdateFileExportTooltip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_INSERT_IMAGE, OnInsertImage)
	ON_COMMAND(ID_INSERT_STRING, OnInsertString)
	ON_COMMAND(ID_INSERT_BUTTON, OnInsertButton)
	ON_COMMAND(ID_INSERT_STATIC, OnInsertStatic)
	ON_COMMAND(ID_INSERT_EDIT, OnInsertEdit)
	ON_COMMAND(ID_INSERT_PROGRESS, OnInsertProgress)
	ON_COMMAND(ID_INSERT_TRACKBAR, OnInsertTrackbar)
	ON_COMMAND(ID_INSERT_SCROLLBAR, OnInsertScrollbar)
	ON_COMMAND(ID_INSERT_AREA, OnInsertArea)
	ON_COMMAND(ID_INSERT_ICONSLOT, OnInsertIconslot)
	ON_COMMAND(ID_INSERT_LIST, OnInsertList)
	ON_COMMAND(ID_EDIT_DUPLICATE, OnEditDuplicate)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_ZORDER, OnEditZorder)
	ON_COMMAND(ID_EDIT_MAKE_GROUP, OnEditMakeGroup)
	ON_COMMAND(ID_EDIT_PARENT, OnEditParent)
	ON_COMMAND(ID_EDIT_UNPARENT, OnEditUnparent)
	ON_COMMAND(ID_EDIT_MOVE_TO_LOWEST, OnEditMoveToLowest)
	ON_COMMAND(ID_EDIT_MOVE_TO_LOWER, OnEditMoveToLower)
	ON_COMMAND(ID_EDIT_MOVE_TO_UPPER, OnEditMoveToUpper)
	ON_COMMAND(ID_EDIT_REPLACE_TEX, OnEditReplaceTex)
	ON_COMMAND(ID_INSERT_GROUP, OnInsertGroup)
	ON_COMMAND(ID_BATCH_TOOL_CHANGE_IMAGE_PATH, OnBatchToolChangeImagePath)
	ON_COMMAND(ID_BATCH_TOOL_CHANGE_FONT, OnBatchToolChangeFont)
	ON_COMMAND(ID_BATCH_TOOL_GATHER_IMAGE_FILE_NAME, OnBatchToolGatherImageFileName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIEDoc construction/destruction

CUIEDoc::CUIEDoc()
{
	// TODO: add one-time construction code here

}

CUIEDoc::~CUIEDoc()
{
}

BOOL CUIEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	Release();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUIEDoc serialization

void CUIEDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUIEDoc diagnostics

#ifdef _DEBUG
void CUIEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUIEDoc commands

BOOL CUIEDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	Release();
	SetCurrentDirectory(m_RootUI.PathGet().c_str());
	return m_RootUI.LoadFromFile(lpszPathName);
}

BOOL CUIEDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	OnEditZorder();
	SetModifiedFlag(FALSE);
	return m_RootUI.SaveToFile(lpszPathName);
//	return CDocument::OnSaveDocument(lpszPathName);
}

void CUIEDoc::OnCloseDocument() 
{
	Release();
	CDocument::OnCloseDocument();
}

void CUIEDoc::Release()
{
	m_RootUI.Release();
	RECT rcRegion = {0,0,64,64};
	m_RootUI.SetRegion(rcRegion);
	m_SelectedUIs.clear();
}

void CUIEDoc::SetSelectedUI(CN3UIBase* pUI)
{
	if(NULL == pUI) m_SelectedUIs.clear();
	else
	{
		it_UI it = m_SelectedUIs.begin(), itEnd = m_SelectedUIs.end();
		bool bOverLapped = false;
		for(; it != itEnd; )
		{
			if(pUI == *it)
			{
				it = m_SelectedUIs.erase(it); // ���� �Ÿ� ���ÿ��� ����.
				bOverLapped = true;
			}
			else it++;
		}

		if(false == bOverLapped) m_SelectedUIs.push_front(pUI); // ��ġ�� ������.. �߰�..
	}

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrm);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_NONE);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertImage() 
{
	SetModifiedFlag();
	CN3UIImage* pUI = new CN3UIImage();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if (FALSE == SetImageInfos(pUI))
	{
		if (IDYES == pFrm->MessageBox("Image ���� ������ ��ҵǾ����ϴ�.\n������ �̹����� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� �̹����� ���� ���Դϴ�.");
		return;
	}

	pFrm->MessageBox("��ġ�� ũ�⸦ �����ּ���");
	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertString()
{
	SetModifiedFlag();
	CN3UIString* pUI = new CN3UIString();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(FALSE == SetStringInfos(pUI))
	{
		if (IDYES == pFrm->MessageBox("Font ������ ��ҵǾ����ϴ�.\n������ UIString�� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("Font ���� �� ��Ÿ ������ �ؾ� UIString�� ���� ���Դϴ�.");
		return;
	}
	// ���� ����
	SIZE size;
	CRect rcRegion(0,0,20,30);
	if (pUI->GetTextExtent("��", lstrlen("��"), &size)) rcRegion.SetRect(0,0,size.cx, size.cy);

	pUI->SetRegion(rcRegion);
	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertButton() 
{
	SetModifiedFlag();
	CN3UIButton* pUI = new CN3UIButton();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->CreateImages();
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if (FALSE == SetButtonInfos(pUI))
	{
		if (IDYES == pFrm->MessageBox("�ؽ��� ������ ��ҵǾ����ϴ�.\n������ ��ư�� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� ��ư�� ���� ���Դϴ�.");
		return;
	}
	// ��ư ��ġ ����, size(width,height)�� image���� ������ ������.
	CRect rcRegion = pUI->GetRegion();
	rcRegion.OffsetRect(-rcRegion.TopLeft());	// 0,0�������� ���߱�
	pUI->SetRegion(rcRegion);

	pFrm->MessageBox("1. ��ư�� ��ġ�� ������ �������ּ���.\n2. �׸� ��ư�� �ƴҰ�� click�Ǵ� ������ �������ּ���.\n3. üũ��ư���� ����� ������ style�� �������ּ���.");
	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
	// click ������ ���� ������ �ʿ䰡 ������ �����Ѵ�.
	// style ����
}

void CUIEDoc::OnInsertStatic() 
{
	SetModifiedFlag();
	CN3UIStatic* pUI = new CN3UIStatic();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->CreateImageAndString();
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);	

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	// image�� ���õ� ������ ���ְ� image������ ������ static�� ������ ������
	CN3UIImage* pBkImage = pUI->GetImageBkGnd();
	SetImageInfos(pBkImage);
	RECT rcRegion = pBkImage->GetRegion();
	pUI->SetRegion(rcRegion);

	// string ���� ����, �� text�� �������ش�.
	CN3UIString* pUIString = pUI->GetUIString();
	if (FALSE == SetStringInfos(pUIString))	pFrm->MessageBox("text�� ������ �־�� �۾��� ����� ���� ���Դϴ�.");

	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertEdit() 
{
	SetModifiedFlag();
	CN3UIEdit* pUI = new CN3UIEdit();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->CreateImageAndString();
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);		

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	// image�� ���õ� ������ ���ְ� image������ ������ edit�� ������ ������
	CN3UIImage* pBkImage = pUI->GetImageBkGnd();
	SetImageInfos(pBkImage);
	RECT rcRegion = pBkImage->GetRegion();
	pUI->SetRegion(rcRegion);
	// string ���� ����
	CN3UIString* pUIString = pUI->GetUIString();
	if (FALSE == SetStringInfos(pUIString))	pFrm->MessageBox("text�� ������ �־�� �۾��� ����� ���� ���Դϴ�.");

	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertProgress() 
{
	SetModifiedFlag();
	CN3UIProgress* pUI = new CN3UIProgress();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->CreateImages();
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	// background�̹����� foreground�̹����� �����ϰ�
	CN3UIImage* pUIImage = pUI->GetBkGndImgRef();
	ASSERT(pUIImage);
	// texture ����
	char szTexture[_MAX_PATH];
	while(1)
	{
		if (FALSE == SelectTexture(szTexture))
		{
			if (IDYES == pFrm->MessageBox("�ؽ��� ������ ��ҵǾ����ϴ�.\nProgress�� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
			else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� progress�� ���� ���Դϴ�.");
			return;
		}
		pUIImage->SetTex(szTexture);
		if (NULL == pUIImage->GetTex())
		{
			if (IDYES == pFrm->MessageBox("�ؽ��ĸ� Load�� �� �����ϴ�.\n�ٽ� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) continue;
			else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� progress�� ���� ���Դϴ�.");
			return;
		}
		else break;
	}
	// image�� normal on down disable�׸� ���� ����
	CDlgTexture dlg;
	dlg.SetTexture(szTexture);
	char szNames[2][20] = {"Back", "Fore"};
	char* szImageTypeNames[2] = {szNames[0], szNames[1]};
	dlg.SetImageTypes(2, szImageTypeNames);
	if (IDCANCEL == dlg.DoModal())
	{	
		if (IDYES == pFrm->MessageBox("�ؽ��� UV��ǥ ������ ��ҵǾ����ϴ�.\n������ Progress�� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("UV��ǥ ���� �� ��Ÿ ������ �ؾ� Progress�� ���� ���Դϴ�.");
		return;
	}
	int i;
	CRect rcRegion;
	for (i=0; i<CN3UIProgress::NUM_IMAGETYPE; ++i)
	{
		if (CN3UIProgress::IMAGETYPE_BKGND == i) pUIImage = pUI->GetBkGndImgRef();
		else if (CN3UIProgress::IMAGETYPE_FRGND == i) pUIImage = pUI->GetFrGndImgRef();
		else pUIImage = NULL;
		ASSERT(pUIImage);
		__FLOAT_RECT frcUV;
		rcRegion = dlg.GetImageRect(i, &frcUV);
		pUIImage->SetTex(szTexture);
		pUIImage->SetUVRect(frcUV.left, frcUV.top, frcUV.right, frcUV.bottom);
	}

	// image������ ������ progress ������ ����
	rcRegion = dlg.GetImageRect(0);
	pUI->SetFrGndUVFromFrGndImage();
	pUI->SetRegion(rcRegion);
	// style ����
	pFrm->MessageBox("����â���� ��Ÿ��(����/����)�� �������ּ���.");

	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertTrackbar() 
{
	SetModifiedFlag();
	CN3UITrackBar* pUI = new CN3UITrackBar();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->CreateImages();
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);

	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if (FALSE == SetTrackBarInfos(pUI))
	{
		if (IDYES == pFrm->MessageBox("Trackbar ���� ������ ��ҵǾ����ϴ�.\n������ Trackbar�� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� Trackbar�� ���� ���Դϴ�.");
		return;
	}
	// style ����
	pFrm->MessageBox("����â���� ��Ÿ��(����/����)�� �������ּ���.");

	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertScrollbar() 
{
	SetModifiedFlag();
	CN3UIScrollBar* pUI = new CN3UIScrollBar();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->CreateTrackBarAndBtns();
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);	
	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->MessageBox("(��ũ�ѹ� �����)��/���� ��ư �����Դϴ�.");
	// 2���� button�� �����ϰ�
	CN3UIButton* pUIBtn = pUI->GetBtnRef(CN3UIScrollBar::BTN_LEFTUP);
	ASSERT(pUIBtn);
	if (FALSE == SetButtonInfos(pUIBtn))
	{
		if (IDYES == pFrm->MessageBox("�ؽ��� ������ ��ҵǾ����ϴ�.\n������ ��ũ���� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� ��ũ���� ���� ���Դϴ�.");
		return;
	}
	// ��ư ��ġ ����, size(width,height)�� image���� ������ ������.
	CRect rcRegion = pUIBtn->GetRegion();
	rcRegion.OffsetRect(-rcRegion.TopLeft());	// 0,0�������� ���߱�
	pUIBtn->SetRegion(rcRegion);
	// 2��° ��ư 
	pFrm->MessageBox("(��ũ�ѹ� �����)�Ʒ�/������ ��ư �����Դϴ�.");
	pUIBtn = pUI->GetBtnRef(CN3UIScrollBar::BTN_RIGHTDOWN);
	ASSERT(pUIBtn);
	if (FALSE == SetButtonInfos(pUIBtn))
	{
		if (IDYES == pFrm->MessageBox("�ؽ��� ������ ��ҵǾ����ϴ�.\n������ ��ũ���� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� ��ũ���� ���� ���Դϴ�.");
		return;
	}
	// ��ư ��ġ ����, size(width,height)�� image���� ������ ������.
	rcRegion = pUIBtn->GetRegion();
	rcRegion.OffsetRect(CPoint(20,20)-rcRegion.TopLeft());	// 20,20�������� ���߱�
	pUIBtn->SetRegion(rcRegion);
	pFrm->MessageBox("(��ũ�ѹ� �����)��� Track bar�����Դϴ�.");
	// trackbar�� �����ϰ�

	CN3UITrackBar* pUITrackBar = pUI->GetTrackBarRef();
	if (FALSE == SetTrackBarInfos(pUITrackBar))
	{
		if (IDYES == pFrm->MessageBox("Trackbar ���� ������ ��ҵǾ����ϴ�.\n������ ScrollBar �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) OnEditDelete();
		else pFrm->MessageBox("�ؽ������� �� ��Ÿ ������ �ؾ� ScrollBar�� ���� ���Դϴ�.");
		return;
	}
	rcRegion = pUITrackBar->GetRegion();
	rcRegion.OffsetRect(CPoint(40,40)-rcRegion.TopLeft());	// 40,40�������� ���߱�
	pUITrackBar->SetRegion(rcRegion);
	pUI->SetRegion(rcRegion);

	// ��ư�� trackbar������ ���ؼ� scrollbar������ ����
	// style�� ���ϰ�
	pFrm->MessageBox("1. ����â���� ��Ÿ��(����/����)�� �������ּ���.\n��ũ���� ������ �ݵ�� �� ��ư�� ��� Ʈ���ٰ� ���� �����ּ���");

	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertArea() 
{
	SetModifiedFlag();
	CN3UIArea* pUI = new CN3UIArea();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->SetRegion(CRect(0,0,20,20));

	SetSelectedUI(NULL);
	SetSelectedUI(pUI);
	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
}

void CUIEDoc::OnInsertIconslot() 
{
#ifdef _REPENT
	SetModifiedFlag();
	CN3UIIconSlot* pUI = new CN3UIIconSlot();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	SetSelectedUI(pUI);
	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->MessageBox("������ ������ �ּ���.");
	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);
	UpdateAllViews(NULL);
#else
	AfxGetMainWnd()->MessageBox("Repent�� �����Ǵ� �����Դϴ�.");
#endif
}

void CUIEDoc::OnInsertList()
{
	SetModifiedFlag();
	CN3UIList* pUI = new CN3UIList();
//	pUI->Init(&m_RootUI);
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	pUI->SetRegion(CRect(0,0,100,50));

	SetSelectedUI(NULL);
	SetSelectedUI(pUI);
	// �Ʒ��� �⺻������ ���ָ� ���� �͵��� �������.
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->GetRightPane()->SetMode(CUIEView::UIEMODE_EDIT);
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);

//	this->OnInsertScrollbar();
}

void CUIEDoc::OnEditDelete()	// ���õ� ui �����
{
	CN3UIBase* pUI = NULL;
	it_UI it = m_SelectedUIs.begin(), itEnd = m_SelectedUIs.end();
	for(; it != itEnd; )
	{
		pUI = *it;
//		if (pUI && (&m_RootUI) == pUI->GetParent())
//		{	// ���õ� ui�� m_RootUI�� child�̸� �����.
		if (pUI && (&m_RootUI) != pUI) // Root UI �� �ƴϸ� �����..
		{
			delete pUI;
			it = m_SelectedUIs.erase(it);
			SetModifiedFlag();
		}
		else
		{
			it++;
		}
	}

	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	pCmdUI->Enable((pUI != (&m_RootUI)) ? TRUE : FALSE);
}

void CUIEDoc::OnEditZorder() 
{
	SetModifiedFlag();
	m_RootUI.ArrangeZOrder();
	CN3UIBase* pUI = this->GetSelectedUI();
	if (pUI && UI_TYPE_IMAGE == pUI->UIType()) ((CN3UIImage*)pUI)->ReorderChildImage();
	UpdateAllViews(NULL);
}

BOOL CUIEDoc::SetImageInfos(CN3UIImage* pUI)
{
	if (NULL == pUI) return FALSE;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	// texture ����
	char szTexture[_MAX_PATH];
	while(1)
	{
		if (FALSE == SelectTexture(szTexture))	return FALSE;
		pUI->SetTex(szTexture);
		if (NULL == pUI->GetTex())
		{
			if (IDYES == pFrm->MessageBox("�ؽ��ĸ� Load�� �� �����ϴ�.\n�ٽ� �����Ͻðڽ��ϱ�?", NULL, MB_YESNO)) continue;
			return FALSE;
		}
		else break;
	}
	// texture ���� ���̴� �κ� ���� (������ UV������ �ϱ� ����)
	CDlgTexture dlg;
	dlg.SetTexture(szTexture);
	if (IDCANCEL == dlg.DoModal()) return FALSE;
	__FLOAT_RECT frcUV;
	CRect rcRegion;
	if (FALSE == dlg.GetSelectedUVRect(&frcUV)) return FALSE;
	rcRegion = dlg.GetSelectedRect();
	// UV��ǥ ����
	pUI->SetUVRect(frcUV.left, frcUV.top, frcUV.right, frcUV.bottom);
	// ���� ����(texture�� ũ�⿡ ���� width�� height�� �ڵ����� �����ϰ� ��ġ�� �����)
	rcRegion.OffsetRect(-rcRegion.TopLeft());	// 0,0�������� ���߱�
	pUI->SetRegion(rcRegion);
	return TRUE;
}

BOOL CUIEDoc::SetStringInfos(CN3UIString* pUI)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	// font ����, ���� �� ����
	CFontDialog dlg;
	if (IDCANCEL == dlg.DoModal())		return FALSE;
	__ASSERT(dlg.GetSize()>0, "font height�� 0���� �۽��ϴ�.");
	CString strTmp = dlg.GetFaceName();

	std::string strFontName = "����ü";
	if(strTmp.GetLength() > 0) strFontName = strTmp;

	pUI->SetFont( strFontName, dlg.GetSize()/10, dlg.IsBold(), dlg.IsItalic());
	COLORREF color = dlg.GetColor();
	D3DCOLOR d3dColor = 0xff000000| ((color&0x00ff0000)>>16) | (color&0x0000ff00) | ((color&0x000000ff)<<16);
	pUI->SetColor(d3dColor);
	// style ����
	pFrm->MessageBox("1. ���� â���� �١ڡ١�[[[Style]]]�١ڡ١��� �������ֽð�(�߿�),\n2. ��ġ�� ũ�⸦ �����ּ���.\n3. ǥ�õ� ���ڰ� ������ ���� �Ʒ�â�� ���ּ���.");
	return TRUE;
}

BOOL CUIEDoc::SetTrackBarInfos(CN3UITrackBar* pUI)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	// background�̹����� thumb�̹����� �����ϰ�
	CN3UIImage* pUIImage = pUI->GetBkGndImgRef();
	ASSERT(pUIImage);
	// texture ����
	char szTexture[_MAX_PATH];
	while(1)
	{
		if (FALSE == SelectTexture(szTexture))return FALSE;
		pUIImage->SetTex(szTexture);
		if (NULL == pUIImage->GetTex())	return FALSE;
		else break;
	}
	// image�� normal on down disable�׸� ���� ����
	CDlgTexture dlg;
	dlg.SetTexture(szTexture);
	char szNames[2][20] = {"Back", "Thumb"};
	char* szImageTypeNames[2] = {szNames[0], szNames[1]};
	dlg.SetImageTypes(2, szImageTypeNames);
	if (IDCANCEL == dlg.DoModal())	return FALSE;
	int i;
	CRect rcRegion;
	for (i=0; i<CN3UITrackBar::NUM_IMAGETYPE; ++i)
	{
		if (CN3UITrackBar::IMAGETYPE_BKGND == i) pUIImage = pUI->GetBkGndImgRef();
		else if (CN3UITrackBar::IMAGETYPE_THUMB == i) pUIImage = pUI->GetThumbImgRef();
		else pUIImage = NULL;
		ASSERT(pUIImage);
		__FLOAT_RECT frcUV;
		rcRegion = dlg.GetImageRect(i, &frcUV);
		pUIImage->SetTex(szTexture);
		pUIImage->SetUVRect(frcUV.left, frcUV.top, frcUV.right, frcUV.bottom);
		rcRegion.OffsetRect(-rcRegion.TopLeft());
		pUIImage->SetRegion(rcRegion);
	}
	// background image������ ������ trackbar ������ ����
	rcRegion = dlg.GetImageRect(0);
	rcRegion.OffsetRect(-rcRegion.TopLeft());
	pUI->SetRegion(rcRegion);
	return TRUE;
}

BOOL CUIEDoc::SetButtonInfos(CN3UIButton* pUI)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CN3UIImage* pUIImage = pUI->GetImageRef(CN3UIButton::BS_NORMAL);
	ASSERT(pUIImage);
	// texture ����
	char szTexture[_MAX_PATH];
	while(1)
	{
		if (FALSE == SelectTexture(szTexture)) return FALSE;
		pUIImage->SetTex(szTexture);
		if (NULL == pUIImage->GetTex()) return FALSE;
		else break;
	}
	// image�� normal on down disable�׸� ���� ����
	CDlgTexture dlg;
	dlg.SetTexture(szTexture);
	char szNames[4][_MAX_PATH] = {"Normal", "Down", "On", "Disable"};
	char* szImageTypeNames[4] = {szNames[0], szNames[1], szNames[2], szNames[3]};
	dlg.SetImageTypes(4, szImageTypeNames);
	if (IDCANCEL == dlg.DoModal()) return FALSE;
	int i;
	CRect rcRegion;
	for (i=0; i<CN3UIButton::NUM_BTN_STATE; ++i)
	{
		pUIImage = pUI->GetImageRef((CN3UIButton::eBTN_STATE)i);
		ASSERT(pUIImage);
		__FLOAT_RECT frcUV;
		rcRegion = dlg.GetImageRect(i, &frcUV);
		pUIImage->SetTex(szTexture);
		pUIImage->SetRegion(rcRegion);
		pUIImage->SetUVRect(frcUV.left, frcUV.top, frcUV.right, frcUV.bottom);
	}
	rcRegion = dlg.GetImageRect(0);
	rcRegion.OffsetRect(-rcRegion.TopLeft());
	pUI->SetRegion(rcRegion);
	return TRUE;
}


void CUIEDoc::OnFileExportTooltip() 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	if (NULL == pUI) return;
	ASSERT(UI_TYPE_STATIC == pUI->UIType());	// tooltip�� static���� ��ӹް� ���� �����ϴ� ������ ����.

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(FALSE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	if (IDCANCEL == dlg.DoModal()) return;

	pUI->SaveToFile((LPCTSTR)dlg.GetPathName());
}

void CUIEDoc::OnUpdateFileExportTooltip(CCmdUI* pCmdUI) 
{	
	CN3UIBase* pUI = this->GetSelectedUI();
	pCmdUI->Enable((pUI && UI_TYPE_STATIC == pUI->UIType()) ? TRUE:FALSE);
}

void CUIEDoc::OnFileImportTooltip() 
{
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(TRUE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	if (IDCANCEL == dlg.DoModal()) return;

	CN3UIStatic* pStatic = new CN3UIStatic();
	pStatic->Init(&m_RootUI);
	pStatic->LoadFromFile((LPCTSTR)dlg.GetPathName());
	SetSelectedUI(NULL);
	SetSelectedUI(pStatic);
}

void CUIEDoc::OnEditDuplicate() 
{
	if(m_SelectedUIs.empty()) return;

	CN3UIBase* pUISrc = NULL;
	CN3UIBase* pUIDest = NULL;
	it_UI it = m_SelectedUIs.begin(), itEnd = m_SelectedUIs.end();
	for(; it != itEnd; it++)
	{
		pUISrc = *it;

		if(NULL == pUISrc) continue;

		pUIDest = NULL;
		switch(pUISrc->UIType())
		{
		case UI_TYPE_BASE:
			{ 
				pUIDest = new CN3UIBase();
				*pUIDest = *pUISrc;
			}
			break;
		case UI_TYPE_BUTTON:
			{
				CN3UIButton *pUINew = new CN3UIButton();
				*pUINew = *((CN3UIButton*)pUISrc);
				pUIDest = pUINew;
			}
			break;	// button
		case UI_TYPE_STATIC:	
			{ 
				CN3UIStatic* pUINew = new CN3UIStatic();		
				*pUINew = *((CN3UIStatic*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// static (���׸��� ���ڰ� ������ Ŭ����)
		case UI_TYPE_PROGRESS:	
			{ 
				CN3UIProgress* pUINew = new CN3UIProgress();	
				*pUINew = *((CN3UIProgress*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// progress
		case UI_TYPE_IMAGE:		
			{ 
				CN3UIImage* pUINew = new CN3UIImage();		
				*pUINew = *((CN3UIImage*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// image
		case UI_TYPE_SCROLLBAR:	
			{ 
				CN3UIScrollBar* pUINew = new CN3UIScrollBar();	
				*pUINew = *((CN3UIScrollBar*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// scroll bar
		case UI_TYPE_STRING:	
			{ 
				CN3UIString* pUINew = new CN3UIString();		
				*pUINew = *((CN3UIString*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// string
		case UI_TYPE_TRACKBAR:	
			{ 
				CN3UITrackBar* pUINew = new CN3UITrackBar();	
				*pUINew = *((CN3UITrackBar*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// track bar
		case UI_TYPE_EDIT:		
			{ 
				CN3UIEdit* pUINew = new CN3UIEdit();		
				*pUINew = *((CN3UIEdit*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// edit
		case UI_TYPE_AREA:		
			{ 
				CN3UIArea* pUINew = new CN3UIArea();		
				*pUINew = *((CN3UIArea*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// area
		case UI_TYPE_TOOLTIP:
			{ 
				CN3UITooltip* pUINew = new CN3UITooltip();		
				*pUINew = *((CN3UITooltip*)pUISrc); 
				pUIDest = pUINew;
			} 
		case UI_TYPE_LIST:
			{ 
				CN3UIList* pUINew = new CN3UIList();
				*pUINew = *((CN3UIList*)pUISrc); 
				pUIDest = pUINew;
			} 
			break;	// tooltip
//		case UI_TYPE_ICON:		pUIDest = new CN3UIIcon();		*pUIDest = *((CN3UIBase*)pUISrc); break;	// icon
//		case UI_TYPE_ICON_MANAGER:	pUIDest = new CN3UIIconManager();	*pUIDest = *((CN3UIBase*)pUISrc); break;	// icon manager.. 
#ifdef _REPENT
		case UI_TYPE_ICONSLOT:
			{
				CN3UIIconSlot* pUINew = new CN3UIIconSlot();
				*pUINew = *((CN3UIIconSlot*)pUISrc);
				pUIDest = pUINew;
			}
			break;	// icon slot
#endif
		}

		if(NULL == pUIDest) continue;

		if(pUISrc->GetParent())
		{
			pUIDest->SetParent(pUISrc->GetParent());
		}
		pUIDest->MoveOffset(10, 10);
	}

	// region �����ϴ� �Լ� ���� ó���ϱ�
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->GetRightPane()->SelectRectType(CUIEView::RT_REGION);

	this->UpdateAllViews(NULL);
	SetModifiedFlag();
}

void CUIEDoc::OnEditMakeGroup() 
{
	CN3UIBase* pUI = NULL;
	CN3UIBase* pUIFirst = NULL;
	it_UI it = m_SelectedUIs.begin(), itEnd = m_SelectedUIs.end();
	bool bOverLapped = false;
	for(int iUIC = 0; it != itEnd; it++, iUIC++)
	{
		pUI = *it;
		if(iUIC == iUIC) pUIFirst = pUI;
		else // ��� ���� �оƮ���� Ȯ��..
		{
			if(pUIFirst->GetParent() != pUI->GetParent())
			{
				MessageBox(AfxGetMainWnd()->m_hWnd, "�׷��ų UI �� ��� ���� �оƮ�� �־�� �մϴ�.", "�׷����� ����", MB_OK);
				return;
			}
		}
	}
	
	if(iUIC <= 1)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, "�ΰ��̻��� UI �� �����ؾ� �׷��� ����� �ֽ��ϴ�.", "�׷����� ����", MB_OK);
		return;
	}

	CN3UIBase* pUIParentOld = pUIFirst->GetParent();
	CN3UIBase* pUIParentNew = new CN3UIBase();
	pUIParentNew->Init(pUIParentOld); // �׷��� ���� �оƮ �ؿ� �ְ�..
	
	it = m_SelectedUIs.begin();
	itEnd = m_SelectedUIs.end();
	CRect rc(0,0,0,0), rcP(0,0,0,0);
	for(; it != itEnd; it++)
	{
		pUI = *it;

		pUI->SetParent(pUIParentNew);
		rc = pUI->GetRegion();
		if(rc.left < rcP.left) rcP.left = rc.left;
		if(rc.top < rcP.top) rcP.top = rc.top;
		if(rc.right > rcP.right) rcP.right = rc.right;
		if(rc.bottom > rcP.bottom) rcP.bottom = rc.bottom;
	}
	pUIParentNew->SetRegion(rcP);

	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnEditParent() 
{
	if(m_SelectedUIs.size() < 2) return;

	CN3UIBase* pUIFirst = *(m_SelectedUIs.begin());
	it_UI it = m_SelectedUIs.begin(), itEnd = m_SelectedUIs.end();
	bool bOverLapped = false;
	for(int i = 0; it != itEnd; it++, i++)
	{
		if(i == 0) continue;

		CN3UIBase* pUI = pUI = *it;
		if(pUI->GetParent() == NULL) continue;

		pUI->GetParent()->RemoveChild(pUI);
		pUI->SetParent(pUIFirst);
	}

	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnEditUnparent() 
{
	if(m_SelectedUIs.empty()) return;

	it_UI it = m_SelectedUIs.begin(), itEnd = m_SelectedUIs.end();
	for(; it != itEnd; it++)
	{
		CN3UIBase* pUI = *it;
		if(pUI->GetParent() == NULL || pUI->GetParent() == &(m_RootUI)) continue;

		pUI->GetParent()->RemoveChild(pUI);
		pUI->SetParent(&m_RootUI);
	}

	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnEditMoveToLowest() 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	if(NULL == pUI) return;
	
	CN3UIBase* pUIParent = pUI->GetParent();
	if(NULL == pUIParent) return;

	pUIParent->MoveToLowest(pUI);
	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnEditMoveToLower() 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	if(NULL == pUI) return;
	
	CN3UIBase* pUIParent = pUI->GetParent();
	if(NULL == pUIParent) return;

	pUIParent->MoveToLower(pUI);
	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnEditMoveToUpper() 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	if(NULL == pUI) return;
	
	CN3UIBase* pUIParent = pUI->GetParent();
	if(NULL == pUIParent) return;

	pUIParent->MoveToUpper(pUI);
	this->UpdateAllViews(NULL);
}

void CUIEDoc::OnFileExport() 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	if (NULL == pUI) return;

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(FALSE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	if (IDCANCEL == dlg.DoModal()) return;

	std::string szFN = dlg.GetPathName();
	pUI->SaveToFile(szFN);
}

void CUIEDoc::OnFileImport() 
{
	CN3UIBase* pUI = this->GetSelectedUI();
	if (NULL == pUI) return;

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(TRUE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	if (IDCANCEL == dlg.DoModal()) return;

	std::string szFN = dlg.GetPathName();
	pUI->LoadFromFile(szFN);
}

void CUIEDoc::OnEditReplaceTex() 
{
	CDlgReplace dlg;
	if (IDCANCEL == dlg.DoModal()) return;
	if (m_RootUI.ReplaceAllTextures(LPCTSTR(dlg.m_strFind), LPCTSTR(dlg.m_strReplace)))
	{
		CString strMsg;
		strMsg.Format("%s �ؽ��ĸ� %s �ؽ��ķ� ��� �ٲپ����ϴ�.", dlg.m_strFind, dlg.m_strReplace);
		AfxGetMainWnd()->MessageBox(strMsg);
	}
	else
	{
		CString strMsg;
		strMsg.Format("%s �ؽ��ĸ� %s �ؽ��� �ٲٱ� ���� �Ͽ����ϴ�.", dlg.m_strFind, dlg.m_strReplace);
		AfxGetMainWnd()->MessageBox(strMsg);
	}
}

void CUIEDoc::OnInsertGroup() 
{
	SetModifiedFlag();
	CN3UIBase* pUI = new CN3UIBase();
	if(this->GetSelectedUI()) pUI->Init(this->GetSelectedUI());
	else pUI->Init(&m_RootUI);
	SetSelectedUI(NULL);
	SetSelectedUI(pUI);

	UpdateAllViews(NULL);
}

void CUIEDoc::OnBatchToolChangeImagePath()
{
	char szBuff[102400] = "";
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CFileDialog dlg(TRUE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	char szCurPath[256]; GetCurrentDirectory(256, szCurPath);
	dlg.m_ofn.lpstrInitialDir = szCurPath;
	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrFile = szBuff;

	if(dlg.DoModal() == IDCANCEL) return;


	CDlgChangeImage dlg2; // �̹��� ���� �̸� ��������..
	if(dlg2.DoModal() == IDCANCEL) return;

	CN3Texture Tex;
	POSITION pos = dlg.GetStartPosition();
	CString FileName;
	std::string szFNOld = dlg2.m_szFN_Old;
	std::string szFNNew = dlg2.m_szFN_New;

	while(pos != NULL)
	{
		CN3UIBase base;
		FileName = dlg.GetNextPathName(pos);

		base.LoadFromFile((const char*)FileName);
		base.ChangeImagePath(szFNOld, szFNNew);
		base.SaveToFile();
	}
}

void CUIEDoc::OnBatchToolChangeFont() 
{
	char szBuff[102400] = "";
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CFileDialog dlg(TRUE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	char szCurPath[256]; GetCurrentDirectory(256, szCurPath);
	dlg.m_ofn.lpstrInitialDir = szCurPath;
	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrFile = szBuff;

	if(dlg.DoModal() == IDCANCEL) return;

	CFontDialog dlg2; // �̹��� ���� �̸� ��������..
	if(dlg2.DoModal() == IDCANCEL) return;

	CN3Texture Tex;
	POSITION pos = dlg.GetStartPosition();
	CString FileName;
	CString szFont = dlg2.GetFaceName();
	while(pos != NULL)
	{
		CN3UIBase base;
		FileName = dlg.GetNextPathName(pos);

		base.LoadFromFile((const char*)FileName);
		base.ChangeFont((const char*)szFont);
		base.SaveToFile();
	}
}

void CUIEDoc::OnBatchToolGatherImageFileName() 
{
	char szBuff[102400] = "";
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CFileDialog dlg(TRUE, "uif", NULL, dwFlags, "UI Files(*.uif)|*.uif;||", NULL);
	char szCurPath[256]; GetCurrentDirectory(256, szCurPath);
	dlg.m_ofn.lpstrInitialDir = szCurPath;
	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrFile = szBuff;

	if(dlg.DoModal() == IDCANCEL) return;

	std::set<std::string> setImgFNs;
	POSITION pos = dlg.GetStartPosition();
	while(pos != NULL)
	{
		CN3UIBase base;
		CString FileName = dlg.GetNextPathName(pos);
		base.LoadFromFile((const char*)FileName);
		base.GatherImageFileName(setImgFNs);
	}
 
	// ���� �����ϱ�..
	char szFolder[_MAX_PATH] = "";
	BROWSEINFO bi;
	LPCITEMIDLIST lpidl;
    bi.hwndOwner = AfxGetMainWnd()->m_hWnd;
    bi.pidlRoot=NULL;
    bi.pszDisplayName = szFolder;
    bi.lpszTitle="�����̸��� ���� ������ �������ּ���";
    bi.ulFlags=BIF_RETURNONLYFSDIRS;
    bi.lpfn=NULL;
    bi.lParam=0;

	lpidl=SHBrowseForFolder(&bi);
	if (lpidl && SHGetPathFromIDList(lpidl, szFolder))
	{
		char szPathOld[256];
		::GetCurrentDirectory(_MAX_PATH, szPathOld);
		::SetCurrentDirectory(szFolder);
		
		CFileFind ff;
		if(ff.FindFile("*.dxt"))
		{
			CDlgUnusedFileList dlg2;
			CString szBasePath = CN3Base::PathGet().c_str();
			while(ff.FindNextFile())
			{
				CString szPath = ff.GetFilePath();
				CString szFN;
				szPath.MakeLower();
				int ii = szPath.Find(szBasePath);
				if(ii >= 0)
				{
					szFN = szPath.Mid(ii + szBasePath.GetLength());
				}
				
				if(szFN.GetLength() >= 0) 
				{
					std::set<std::string>::iterator it = setImgFNs.find((const char*)szFN);
					if(it == setImgFNs.end())
					{
						dlg2.m_szFileNames.Add(szPath);
					}
				}
			}

			dlg2.DoModal();
		}

		::SetCurrentDirectory(szPathOld);
	}
}
