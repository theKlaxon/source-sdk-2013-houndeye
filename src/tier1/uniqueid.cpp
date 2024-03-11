//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
// Unique ID generation
//=============================================================================//
#include "tier1/uniqueid.h"
#include "tier0/platform.h"
#include "tier1/utlbuffer.h"
#include <string>
#include <string_view>
#include <algorithm>

#if IsWindows()
    #include <rpc.h>
	#include <rpcdce.h>
    #undef min
    #undef max
#endif

// Indices in the UUID string representation for each byte.
static unsigned char encoded_pos[16] { 0, 2, 4, 6, 9, 11, 14, 16, 19, 21, 24, 26, 28, 30, 32, 34 };
// Hex
static const auto hex = "0123456789abcdef";
// Hex to nibble mapping.
static const unsigned char hex_to_nibble[256] {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};


//-----------------------------------------------------------------------------
// Creates a new unique id
//-----------------------------------------------------------------------------
void CreateUniqueId( UniqueId_t* pDest ) {
	#if IsWindows() && IsPC()
		Assert( sizeof( UUID ) == sizeof( *pDest ) );
		UuidCreate( (UUID*) pDest );
	#else
		// X360/linux TBD: Need a real UUID Implementation
		Q_memset( pDest, 0, sizeof( UniqueId_t ) );
	#endif
}

//-----------------------------------------------------------------------------
// Creates a new unique id from a string representation of one
//-----------------------------------------------------------------------------
bool UniqueIdFromString( UniqueId_t* pDest, const char* pBuf, int nMaxLen ) {
	if ( nMaxLen == 0 ) {
		nMaxLen = Q_strlen( pBuf );

		// we got an empty string..?
		if ( nMaxLen == 0 ) {
			return false;
		}
	}

	auto* pTemp{ static_cast<char*>( stackalloc( nMaxLen + 1 ) ) };
	V_strncpy( pTemp, pBuf, nMaxLen + 1 );
	--nMaxLen;
	while ( ( nMaxLen >= 0 ) && isspace( pTemp[ nMaxLen ] ) ) {
		--nMaxLen;
	}
	pTemp[ nMaxLen + 1 ] = 0;

	while ( *pTemp && isspace( *pTemp ) ) {
		++pTemp;
	}

	// ensure its a valid UUID
	auto len{ Q_strlen( pTemp ) };
	if ( len != 36 || pTemp[8] != '-' || pTemp[13] != '-' || pTemp[18] != '-' || pTemp[23] != '-' ) {
		return false;
	}

	for ( auto i{0}; i < sizeof( encoded_pos ); i += 1 ) {
		auto hi{ hex_to_nibble[ pTemp[ encoded_pos[i] + 0 ] ] };
		auto lo{ hex_to_nibble[ pTemp[ encoded_pos[i] + 1 ] ] };
		if (hi == 0xff or lo == 0xff) {
			return false;
		}
		pDest->m_Value[i] = hi << 4 | lo;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Sets an object ID to be an invalid state
//-----------------------------------------------------------------------------
void InvalidateUniqueId( UniqueId_t* pDest ) {
	Assert( pDest );
	memset( pDest, 0, sizeof( UniqueId_t ) );
}

bool IsUniqueIdValid( const UniqueId_t& id ) {
	UniqueId_t invalidId{}; // zero-initialization ensures this is empty/invalid
	return !IsUniqueIdEqual( invalidId, id );
}

bool IsUniqueIdEqual( const UniqueId_t& id1, const UniqueId_t& id2 ) {
	return memcmp( &id1, &id2, sizeof( UniqueId_t ) ) == 0;
}

void UniqueIdToString( const UniqueId_t& id, char* pBuf, int nMaxLen ) {
	pBuf[ 0 ] = 0;

	char string[36] { 0 };
	string[8] = string[13] = string[18] = string[23] = '-';

	for ( auto i{ 0 }; i < sizeof( encoded_pos ); i += 1 ) {
		string[encoded_pos[i] + 0] = hex[id.m_Value[i] >> 4  ];
		string[encoded_pos[i] + 1] = hex[id.m_Value[i] & 0x0f];
	}

	Q_strncpy( pBuf, string, std::min( 36, nMaxLen ) );
}

void CopyUniqueId( const UniqueId_t& src, UniqueId_t* pDest ) {
	memcpy( pDest, &src, sizeof( UniqueId_t ) );
}

bool Serialize( CUtlBuffer& buf, const UniqueId_t& src ) {
	if ( buf.IsText() ) {
		char string[36] { 0 };
		string[8] = string[13] = string[18] = string[23] = '-';

		for ( auto i{ 0 }; i < sizeof( encoded_pos ); i += 1 ) {
			string[encoded_pos[i] + 0] = hex[src.m_Value[i] >> 4  ];
			string[encoded_pos[i] + 1] = hex[src.m_Value[i] & 0x0f];
		}

		buf.Put( string, sizeof( string ) );
	} else {
		buf.Put( &src, sizeof( UniqueId_t ) );
	}
	return buf.IsValid();
}

bool Unserialize( CUtlBuffer& buf, UniqueId_t& dest ) {
	if ( buf.IsText() ) {
		int nTextLen = buf.PeekStringLength();
		auto* pBuf = static_cast<char*>( stackalloc( nTextLen ) );
		buf.GetStringManualCharCount( pBuf, nTextLen );
		UniqueIdFromString( &dest, pBuf, nTextLen );
	} else {
		buf.Get( &dest, sizeof( UniqueId_t ) );
	}
	return buf.IsValid();
}
