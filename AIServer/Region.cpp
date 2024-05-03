// Region.cpp: implementation of the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "Region.h"
#include "Npc.h"
#include "User.h"

CRegion::CRegion()
{
}

CRegion::~CRegion()
{
	if( !m_RegionUserArray.IsEmpty() )
		m_RegionUserArray.DeleteAllData();

	if( !m_RegionNpcArray.IsEmpty() )
		m_RegionNpcArray.DeleteAllData();
}

