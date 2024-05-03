// STLMap.h: interface for the CSTLMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_)
#define AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <map>
using namespace std;

template <class T> class CSTLMap  
{
public:
	map < int, T* >		m_UserTypeMap;
	
	int GetSize() { return m_UserTypeMap.size(); };
	
	bool PutData( int key_value, T* pData)
	{
		if( !pData ) return false;
		auto temp_pair = m_UserTypeMap.insert( std::pair( key_value, pData ) );
		return temp_pair.second;
	};
	
	T* GetData( int key_value )
	{
		T* pData = NULL;
		auto iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return NULL;
		else
			pData = (*iter).second;
		
		return pData;
	};

	auto DeleteData( int key_value )
	{
		if( m_UserTypeMap.empty() )
			return m_UserTypeMap.end();
		
		auto iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return m_UserTypeMap.end();
		else {
			T* pData = NULL;
			pData = (*iter).second;

			iter = m_UserTypeMap.erase(iter);

			delete pData;
			pData = NULL;

			return iter;
		}
	};

	void DeleteAllData()
	{
		auto iter1 = m_UserTypeMap.begin(), iter2 = m_UserTypeMap.end();
		for(; iter1 != iter2; iter1++ )
			delete (*iter1).second;
		
		m_UserTypeMap.clear();
	};

	bool IsExist( int key_value )
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
