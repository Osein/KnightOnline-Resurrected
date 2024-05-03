//////////////////////////////////////////////////////////////////////
//
// STLMap.h: interface for the CSTLMap class.
//
// This class can use ordinary map of the standard template library 
//
// Designed by SAMMA	 2002. 03. 15
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_)
#define AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)		// Visual C++ Only

#include <map>
using namespace std;

template <class T1, class T2> class CSTLMap  
{
public:
	map < T1, T2* >		m_UserTypeMap;

	int GetSize() { return m_UserTypeMap.size(); };
	
	bool PutData( T1 key_value, T2* pData)
	{
		if( !pData ) return false;
		auto temp_pair = m_UserTypeMap.insert( std::pair<T1, T2*>( key_value, pData ) );
		return temp_pair.second;
	};
	
	T2* GetData( T1 key_value )
	{
		T2* pData = NULL;
		auto iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return NULL;
		else
			pData = (*iter).second;
		
		return pData;
	};

	bool DeleteData( T1 key_value )
	{
		if( m_UserTypeMap.empty() )
			return false;
		
		auto iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return false;
		else {
			T2* pData = NULL;
			pData = (*iter).second;

			m_UserTypeMap.erase(iter);

			delete pData;
			pData = NULL;

			return true;
		}
	};

	void DeleteAllData()
	{
		auto iter1 = m_UserTypeMap.begin(), iter2 = m_UserTypeMap.end();
		for(; iter1 != iter2; iter1++ )
			delete (*iter1).second;
		
		m_UserTypeMap.clear();
	};

	bool IsExist( T1 key_value )
	{
		auto iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return false;
		else
			return true;
	};

	bool IsEmpty() { return m_UserTypeMap.empty(); };

	CSTLMap() {};
	virtual ~CSTLMap() {
		DeleteAllData();
	};
};

#endif // !defined(AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_)
