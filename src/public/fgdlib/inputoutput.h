//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=============================================================================
#pragma once
#include "fgdlib/entitydefs.h"
#include <utlvector.h>


enum InputOutputType_t {
	iotInvalid = -1,
	iotVoid,
	iotInt,
	iotBool,
	iotString,
	iotFloat,
	iotVector,
	iotEHandle,
	iotColor,
};


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CClassInputOutputBase {
public:
	CClassInputOutputBase();
	CClassInputOutputBase( const char* pszName, InputOutputType_t eType );
	virtual ~CClassInputOutputBase();

	inline const char* GetName() { return ( m_szName ); }
	InputOutputType_t GetType() { return ( m_eType ); }
	const char* GetTypeText();
	inline const char* GetDescription();

	inline void SetName( const char* szName ) { V_strcpy_safe( m_szName, szName ); }
	inline void SetType( InputOutputType_t eType ) { m_eType = eType; }
	InputOutputType_t SetType( const char* szType );
	inline void SetDescription( char* pszDescription ) { m_pszDescription = pszDescription; }

	CClassInputOutputBase& operator=( CClassInputOutputBase& );

protected:
	static const char* g_pszEmpty;

	char m_szName[ MAX_IO_NAME_LEN ];
	InputOutputType_t m_eType;
	char* m_pszDescription;
};


//-----------------------------------------------------------------------------
// Purpose: Returns this I/O's long description.
//-----------------------------------------------------------------------------
const char* CClassInputOutputBase::GetDescription() {
	if ( m_pszDescription != nullptr ) {
		return ( m_pszDescription );
	}

	return ( g_pszEmpty );
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CClassInput : public CClassInputOutputBase {
public:
	CClassInput();
	CClassInput( const char* pszName, InputOutputType_t eType );
};


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CClassOutput : public CClassInputOutputBase {
public:
	CClassOutput();
	CClassOutput( const char* pszName, InputOutputType_t eType );
};


typedef CUtlVector<CClassInput*> CClassInputList;
typedef CUtlVector<CClassOutput*> CClassOutputList;
