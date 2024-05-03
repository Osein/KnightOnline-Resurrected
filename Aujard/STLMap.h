#pragma once

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

	bool DeleteData( int key_value )
	{
		if( m_UserTypeMap.empty() )
			return false;
		
		auto iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return false;
		else {
			T* pData = NULL;
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
