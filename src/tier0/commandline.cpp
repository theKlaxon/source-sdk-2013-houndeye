//
// Created by ENDERZOMBI102 on 09/02/2024.
//
#include "commandline.hpp"
#include "tier0/dbg.h"

static CCommandLine* g_pCommandLine{ nullptr };


const char* tokenize( const char* line, std::string& buffer ) {
	// if we're already at the end, do nothing
	if ( !line || *line == '\0' )
		return nullptr;

	// ignore leading space
	while ( *line == ' ' || *line == '\t' )
		line += 1;

	size_t offset;
	const char* start;
	if ( ( offset = *line == '"' ) ) {  // parse a quoted string
		start = ( ++line );
		while ( *line != '"' && *line != '\0' )
			line += 1;
	} else {  // parse a single token
		start = line;
		while ( *line != ' ' && *line != '\t' && *line != '\0' )
			line += 1;
	}

	buffer.clear();
	buffer.append( start, line - start );
	// return the new initial position
	return line + offset;
}


void CCommandLine::CreateCmdLine( const char* pCommandLine ) {
	this->Reset();
	// allocates new cmdline
	const char* line{ pCommandLine };
	std::string token;
	while ( (line = tokenize( line, token )) ) {
		this->m_Params.emplace_back( token );
		this->m_sCmdLine += ( token + " " );
	}

	this->m_sCmdLine.resize( this->m_sCmdLine.length() - 1 );
	this->m_sCmdLine.shrink_to_fit();
}
void CCommandLine::CreateCmdLine( int argc, char** argv ) {
	using namespace std::string_literals;
	this->Reset();
	// allocates new cmdline, wrapping every token in `"`
	for ( int i{ 0 }; i < argc; i += 1 ) {
		this->m_Params.emplace_back( argv[ i ] );
		this->m_sCmdLine += ( "\""s + argv[ i ] + "\" " );
	}
	this->m_sCmdLine.resize( this->m_sCmdLine.length() - 1 );
	this->m_sCmdLine.shrink_to_fit();
}
const char* CCommandLine::GetCmdLine() const {
	// returns our version of the cmdline
	return this->m_sCmdLine.c_str();
}

const char* CCommandLine::CheckParm( const char* psz, const char** ppszValue ) const {
	auto index{ this->FindParm( psz ) };
	if ( ppszValue )
		*ppszValue = nullptr;

	if (! index )
		return nullptr;

	if ( ppszValue && index + 1 < this->m_Params.size() && this->m_Params[index + 1][0] != '-' )
		*ppszValue = this->m_Params[index + 1].c_str();
	return this->m_Params[index].c_str();
}
void CCommandLine::RemoveParm( const char* parm ) {
	// remove `-key` and its `value`
	AssertUnreachable();
}
void CCommandLine::AppendParm( const char* pszParm, const char* pszValues ) {
	// appends to the cmdline string too, without wrapping ( unless pre-wrapped by caller )
	this->m_Params.emplace_back( pszParm );
	this->m_sCmdLine.append( " " ).append( pszParm );

	// FIXME: This has invalid behavior, it doesn't parse the param correctly.
	if ( pszValues ) {
		this->m_Params.emplace_back( pszValues );
		this->m_sCmdLine.append( " " ).append( pszValues );
	}
}

const char* CCommandLine::ParmValue( const char* psz, const char* pDefaultVal ) const {
	for ( int i{ 0 }; i < this->m_Params.size(); i += 1 ) {
		if ( this->m_Params[i] == psz && i + 1 < this->m_Params.size() )
			return this->m_Params[ i + 1 ].c_str();
	}
	return pDefaultVal;
}
int CCommandLine::ParmValue( const char* psz, int nDefaultVal ) const {
	for ( int i{ 0 }; i < this->m_Params.size(); i += 1 ) {
		if ( this->m_Params[i] == psz && i + 1 < this->m_Params.size() ) {
			char* invalid;
			auto value{ strtol( this->m_Params[ i + 1 ].c_str(), &invalid, 10 ) };
			if ( invalid )
				break;

			return value;
		}
	}
	return nDefaultVal;
}
float CCommandLine::ParmValue( const char* psz, float flDefaultVal ) const {
	for ( int i{ 0 }; i < this->m_Params.size(); i += 1 ) {
		if ( this->m_Params[i] == psz && i + 1 < this->m_Params.size() ) {
			char* invalid;
			auto value{ strtof( this->m_Params[ i + 1 ].c_str(), &invalid ) };
			if ( invalid )
				break;

			return value;
		}
	}
	return flDefaultVal;
}

int CCommandLine::ParmCount() const {
	// counting both `-key`s and `value`s
	return static_cast<int>( this->m_Params.size() );
}
int CCommandLine::FindParm( const char* psz ) const {
	// ignores wrapping
	for ( int i{ 0 }; i < this->m_Params.size(); i += 1 ) {
		if ( this->m_Params[i] == psz )
			return i;
	}
	return 0;
}
const char* CCommandLine::GetParm( int nIndex ) const {
	if ( this->m_Params.size() < nIndex )
		return "";

	return this->m_Params[nIndex].c_str();
}

void CCommandLine::SetParm( int nIndex, const char* pNewParm ) {
	if ( this->m_Params.size() < nIndex )
		return;

	this->m_Params[nIndex] = pNewParm;
}

const char* CCommandLine::ParmValueByIndex( int nIndex, const char* pDefaultVal ) const {
	if ( nIndex == 0 || nIndex >= this->m_Params.size() )
		return pDefaultVal;

	return this->m_Params[nIndex + 1].c_str();
}

void CCommandLine::Reset() {
	this->m_sCmdLine.clear();
	this->m_Params.clear();
}


ICommandLine* CommandLine_Tier0() {
	if (! g_pCommandLine )
		g_pCommandLine = new CCommandLine();

	return g_pCommandLine;
}
