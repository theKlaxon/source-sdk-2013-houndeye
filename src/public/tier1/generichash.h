//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Variant Pearson Hash general purpose hashing algorithm described
//			by Cargill in C++ Report 1994. Generates a 16-bit result.
//
//=============================================================================
#pragma once


uint32 FASTCALL HashString( const char* pszKey );
uint32 FASTCALL HashStringCaseless( const char* pszKey );
uint32 FASTCALL HashStringCaselessConventional( const char* pszKey );
uint32 FASTCALL Hash4 ( const void* pKey );
uint32 FASTCALL Hash8 ( const void* pKey );
uint32 FASTCALL Hash12( const void* pKey );
uint32 FASTCALL Hash16( const void* pKey );
uint32 FASTCALL HashBlock( const void* pKey, uint32 size );

uint32 FASTCALL HashInt( const int32 key );

// hash a uint32 into a uint32
ALWAYS_INLINE
uint32 HashIntAlternate( uint32 n ) {
	n = ( n + 0x7ed55d16 ) + ( n << 12 );
	n = ( n ^ 0xc761c23c ) ^ ( n >> 19 );
	n = ( n + 0x165667b1 ) + ( n << 5 );
	n = ( n + 0xd3a2646c ) ^ ( n << 9 );
	n = ( n + 0xfd7046c5 ) + ( n << 3 );
	n = ( n ^ 0xb55a4f09 ) ^ ( n >> 16 );
	return n;
}

inline uint32 HashIntConventional( const int32 n ) {// faster but less effective
	// first byte
	uint32 hash = 0xAAAAAAAA + ( n & 0xFF );
	// second byte
	hash = ( hash << 5 ) + hash + ( ( n >> 8 ) & 0xFF );
	// third byte
	hash = ( hash << 5 ) + hash + ( ( n >> 16 ) & 0xFF );
	// fourth byte
	hash = ( hash << 5 ) + hash + ( ( n >> 24 ) & 0xFF );

	return hash;

	/* this is the old version, which would cause a load-hit-store on every
	   line on a PowerPC, and therefore took hundreds of clocks to execute!
	  
	byte *p = (byte *)&n;
	uint32 hash = 0xAAAAAAAA + *p++;
	hash = ( ( hash << 5 ) + hash ) + *p++;
	hash = ( ( hash << 5 ) + hash ) + *p++;
	return ( ( hash << 5 ) + hash ) + *p;
	*/
}

//-----------------------------------------------------------------------------

template<typename T>
inline uint32 HashItem( const T& item ) {
	// Unused paths are optimized out, even on -O0
	if ( sizeof( item ) == 4 )
		return Hash4( &item );
	else if ( sizeof( item ) == 8 )
		return Hash8( &item );
	else if ( sizeof( item ) == 12 )
		return Hash12( &item );
	else if ( sizeof( item ) == 16 )
		return Hash16( &item );

	return HashBlock( &item, sizeof( item ) );
}

template<>
inline uint32 HashItem<int32>( const int32& key ) {
	return HashInt( key );
}

template<>
inline uint32 HashItem<uint32>( const uint32& key ) {
	return HashInt( (int) key );
}

template<>
inline uint32 HashItem<const char*>( const char* const& pszKey ) {
	return HashString( pszKey );
}

template<>
inline uint32 HashItem<char*>( char* const& pszKey ) {
	return HashString( pszKey );
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Murmur hash
//-----------------------------------------------------------------------------
uint32 MurmurHash2( const void* key, int32 len, uint32 seed );

// return murmurhash2 of a downcased string
uint32 MurmurHash2LowerCase( char const* pString, uint32 nSeed );

uint64 MurmurHash64( const void* key, int32 len, uint32 seed );
