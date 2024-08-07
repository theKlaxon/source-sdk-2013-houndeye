//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include <cstdlib>

// helper functions for parsing unicode file buffers
ucs2* AdvanceOverWhitespace( ucs2* start );
ucs2* ReadUnicodeToken( ucs2* start, ucs2* token, int tokenBufferSize, bool& quoted );
ucs2* ReadUnicodeTokenNoSpecial( ucs2* start, ucs2* token, int tokenBufferSize, bool& quoted );
ucs2* ReadToEndOfLine( ucs2* start );

// writing to unicode files via CUtlBuffer
class CUtlBuffer;
void WriteUnicodeString( CUtlBuffer& buffer, const wchar_t* string, bool addQuotes = false );
void WriteAsciiStringAsUnicode( CUtlBuffer& buffer, const char* string, bool addQuotes = false );
