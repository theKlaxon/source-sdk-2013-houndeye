//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include <fstream>
#include "tier0/platform.h"
#include "tier0/basetypes.h"


enum trtoken_t {
	TOKENSTRINGTOOLONG = -4,
	TOKENERROR = -3,
	TOKENNONE = -2,
	TOKENEOF = -1,
	OPERATOR,
	INTEGER,
	STRING,
	IDENT
};


#define IsToken( s1, s2 ) !strcmpi( s1, s2 )

#define MAX_TOKEN 128 + 1
#define MAX_IDENT 64 + 1
#define MAX_STRING 128 + 1


class TokenReader : private std::ifstream {
public:
	TokenReader();

	bool Open( const char* pszFilename );
	trtoken_t NextToken( char* pszStore, int nSize );
	trtoken_t NextTokenDynamic( char** ppszStore );
	void Close();

	void IgnoreTill( trtoken_t ttype, const char* pszToken );
	void Stuff( trtoken_t ttype, const char* pszToken );
	bool Expecting( trtoken_t ttype, const char* pszToken );
	const char* Error( char* error, ... );
	trtoken_t PeekTokenType( char* = nullptr, int maxlen = 0 );

	inline int GetErrorCount() const;
private:
	// compiler can't generate an assignment operator since descended from std::ifstream
	inline TokenReader( TokenReader const& );
	inline int operator=( TokenReader const& );

	trtoken_t GetString( char* pszStore, int nSize );
	bool SkipWhiteSpace();

	int m_nLine;
	int m_nErrorCount;

	char m_szFilename[ 128 ];
	char m_szStuffed[ 128 ];
	bool m_bStuffed;
	trtoken_t m_eStuffed;
};


//-----------------------------------------------------------------------------
// Purpose: Returns the total number of parsing errors since this file was opened.
//-----------------------------------------------------------------------------
int TokenReader::GetErrorCount() const {
	return m_nErrorCount;
}
