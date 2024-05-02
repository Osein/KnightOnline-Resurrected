// Region.cpp: implementation of the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ebenezer.h"
#include "Region.h"

CRegion::CRegion()
{
}

CRegion::~CRegion()
{
	if( !m_RegionItemArray.IsEmpty() )
		m_RegionItemArray.DeleteAllData();

	if( !m_RegionUserArray.IsEmpty() )
		m_RegionUserArray.DeleteAllData();

	if( !m_RegionNpcArray.IsEmpty() )
		m_RegionNpcArray.DeleteAllData();
}
