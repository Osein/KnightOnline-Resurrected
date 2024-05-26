#include "stdafx.h"
#include "MakeItemGroupSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemGroupSet

IMPLEMENT_DYNAMIC(CMakeItemGroupSet, CRecordset)

CMakeItemGroupSet::CMakeItemGroupSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CItemGroupSet)
	m_iItemGroupNum = 0;
	//m_name = _T("");
	m_item1 = 0;
	m_item2 = 0;
	m_item3 = 0;
	m_item4 = 0;
	m_item5 = 0;
	m_item6 = 0;
	m_item7 = 0;
	m_item8 = 0;
	m_item9 = 0;
	m_item10 = 0;
	m_item11 = 0;
	m_item12 = 0;
	m_item13 = 0;
	m_item14 = 0;
	m_item15 = 0;
	m_item16 = 0;
	m_item17 = 0;
	m_item18 = 0;
	m_item19 = 0;
	m_item20 = 0;
	m_item21 = 0;
	m_item22 = 0;
	m_item23 = 0;
	m_item24 = 0;
	m_item25 = 0;
	m_item26 = 0;
	m_item27 = 0;
	m_item28 = 0;
	m_item29 = 0;
	m_item30 = 0;
	m_nFields = 31;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CMakeItemGroupSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=KN_Online;UID=knight;PWD=knight");
}

CString CMakeItemGroupSet::GetDefaultSQL()
{
	return _T("[dbo].[MAKE_ITEM_GROUP]");
}

void CMakeItemGroupSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CItemGroupSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[iItemGroupNum]"), m_iItemGroupNum);
	//RFX_Text(pFX, _T("[name]"), m_name);
	RFX_Long(pFX, _T("[iItem_1]"), m_item1);
	RFX_Long(pFX, _T("[iItem_2]"), m_item2);
	RFX_Long(pFX, _T("[iItem_3]"), m_item3);
	RFX_Long(pFX, _T("[iItem_4]"), m_item4);
	RFX_Long(pFX, _T("[iItem_5]"), m_item5);
	RFX_Long(pFX, _T("[iItem_6]"), m_item6);
	RFX_Long(pFX, _T("[iItem_7]"), m_item7);
	RFX_Long(pFX, _T("[iItem_8]"), m_item8);
	RFX_Long(pFX, _T("[iItem_9]"), m_item9);
	RFX_Long(pFX, _T("[iItem_10]"), m_item10);
	RFX_Long(pFX, _T("[iItem_11]"), m_item11);
	RFX_Long(pFX, _T("[iItem_12]"), m_item12);
	RFX_Long(pFX, _T("[iItem_13]"), m_item13);
	RFX_Long(pFX, _T("[iItem_14]"), m_item14);
	RFX_Long(pFX, _T("[iItem_15]"), m_item15);
	RFX_Long(pFX, _T("[iItem_16]"), m_item16);
	RFX_Long(pFX, _T("[iItem_17]"), m_item17);
	RFX_Long(pFX, _T("[iItem_18]"), m_item18);
	RFX_Long(pFX, _T("[iItem_19]"), m_item19);
	RFX_Long(pFX, _T("[iItem_20]"), m_item20);
	RFX_Long(pFX, _T("[iItem_21]"), m_item21);
	RFX_Long(pFX, _T("[iItem_22]"), m_item22);
	RFX_Long(pFX, _T("[iItem_23]"), m_item23);
	RFX_Long(pFX, _T("[iItem_24]"), m_item24);
	RFX_Long(pFX, _T("[iItem_25]"), m_item25);
	RFX_Long(pFX, _T("[iItem_26]"), m_item26);
	RFX_Long(pFX, _T("[iItem_27]"), m_item27);
	RFX_Long(pFX, _T("[iItem_28]"), m_item28);
	RFX_Long(pFX, _T("[iItem_29]"), m_item29);
	RFX_Long(pFX, _T("[iItem_30]"), m_item30);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CItemGroupSet diagnostics

#ifdef _DEBUG
void CMakeItemGroupSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CMakeItemGroupSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
