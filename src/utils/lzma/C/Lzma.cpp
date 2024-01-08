/**
 *
 * Lzma.c - Implements certain unresolved symbols for tooling LZMA needs.
 *
 * Credit: Based on code by JJL772, https://github.com/JJL772/source-sdk-custom
 */
#include "LzmaEnc.h"
#include <memory.h>
#include <cstdlib>
extern "C" {
	#include "lzma/lzma.h"
}

#define LZMA_ID ( ( 'A' << 24 ) | ( 'M' << 16 ) | ( 'Z' << 8 ) | ( 'L' ) )
#define LZMA_ORIGINAL_HEADER_SIZE 13

#pragma pack( 1 )
struct LzmaHeader {
	unsigned int id;
	unsigned int actualSize;// always little endian
	unsigned int lzmaSize;  // always little endian
	unsigned char properties[ 5 ];
};
#pragma pack()

// TODO: Verify this all works! It wasn't tested nor logic-verified!

//-----------------------------------------------------------------------------
// Encoding glue. Returns non-null Compressed buffer if successful.
// Caller must free.
//-----------------------------------------------------------------------------
unsigned char* LZMA_Compress( unsigned char* pInput, unsigned int inputSize, unsigned int* pOutputSize ) {
	*pOutputSize = 0;

	if ( inputSize <= sizeof( LzmaHeader ) ) {
		// pointless
		return nullptr;
	}

	// using same work buffer calcs as the SDK 105% + 64K
	unsigned outSize = inputSize / 20 * 21 + ( 1 << 16 );
	auto* pOutputBuffer = reinterpret_cast<unsigned char*>( malloc( outSize ) );
	if ( !pOutputBuffer ) {
		return nullptr;
	}

	ISzAlloc allocator{
		.Alloc = [](void*, size_t size) { return malloc(size); },
		.Free = [](void*, void* ptr) { free(ptr); }
	};

	// compress, skipping past our header
	size_t propsSize = LZMA_PROPS_SIZE;
	CLzmaEncProps props;
	LzmaEncProps_Init( &props );
	props.numThreads = 4;
	auto outBufferSize = static_cast<size_t>( outSize - sizeof( LzmaHeader ) );
	int result = LzmaEncode(
		pOutputBuffer + sizeof( LzmaHeader ) + LZMA_PROPS_SIZE, &outBufferSize,
		pInput, inputSize,
		&props, pOutputBuffer + sizeof( LzmaHeader ), &propsSize, (int) props.writeEndMark,
		nullptr, &allocator, &allocator
	);
	if ( result != SZ_OK ) {
		free( pOutputBuffer );
		return nullptr;
	}

	if ( outBufferSize - LZMA_ORIGINAL_HEADER_SIZE + sizeof( LzmaHeader ) >= inputSize ) {
		// compression got worse or stayed the same
		free( pOutputBuffer );
		return nullptr;
	}

	// construct our header, strip theirs
	auto* pHeader = reinterpret_cast<LzmaHeader*>( pOutputBuffer );
	pHeader->id = LZMA_ID;
	pHeader->actualSize = inputSize;
	pHeader->lzmaSize = outBufferSize - LZMA_ORIGINAL_HEADER_SIZE;
	memcpy( pHeader->properties, pOutputBuffer + sizeof( LzmaHeader ), 5 );

	// shift the compressed data into place
	memcpy( pOutputBuffer + sizeof( LzmaHeader ), pOutputBuffer + sizeof( LzmaHeader ) + LZMA_ORIGINAL_HEADER_SIZE, outBufferSize - LZMA_ORIGINAL_HEADER_SIZE );

	// final output size is our header plus compressed bits
	*pOutputSize = sizeof( LzmaHeader ) + outBufferSize - LZMA_ORIGINAL_HEADER_SIZE;

	return pOutputBuffer;
}

//-----------------------------------------------------------------------------
// Decoding glue. Returns TRUE if succesful.
//-----------------------------------------------------------------------------
bool LZMA_Uncompress( unsigned char* pInput, unsigned char** ppOutput, unsigned int* pOutputSize ) {
	*pOutputSize = 0;

	if (! LZMA_IsCompressed( pInput ) )
		return false;

	auto* header = reinterpret_cast<LzmaHeader*>( pInput );

	auto* output = reinterpret_cast<unsigned char*>( malloc( header->actualSize ) );
	auto outSize = header->actualSize;

	// TODO: Finish this impl
//	unsigned srcLen = inBuf.size() - LZMA_PROPS_SIZE;
//	auto res = LzmaUncompress(
//		output, &outSize,
//		&inBuf[LZMA_PROPS_SIZE], &srcLen,
//		&inBuf[0], LZMA_PROPS_SIZE);

//	if ( res != SZ_OK ) {
//		free( output );
//		return false;
//	}

	*ppOutput = output;
	*pOutputSize = header->actualSize;
	return true;
}

//-----------------------------------------------------------------------------
// Decoding helper, returns TRUE if buffer is LZMA compressed.
//-----------------------------------------------------------------------------
bool LZMA_IsCompressed( unsigned char* pInput ) {
	auto* pHeader = reinterpret_cast<LzmaHeader*>( pInput );
	if ( pHeader && pHeader->id == LZMA_ID ) {
		return true;
	}

	// unrecognized
	return false;
}

//-----------------------------------------------------------------------------
// Decoding helper, returns non-zero size of data when uncompressed, otherwise 0.
//-----------------------------------------------------------------------------
unsigned int LZMA_GetActualSize( unsigned char* pInput ) {
	auto* pHeader = reinterpret_cast<LzmaHeader*>( pInput );
	if ( pHeader && pHeader->id == LZMA_ID ) {
		return pHeader->actualSize;
	}

	// unrecognized
	return 0;
}