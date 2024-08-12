//
// Created by ENDERZOMBI102 on 12/08/2024.
//
#include "bitmap/tgawriter.h"
#include "filesystem.h"
#include "dbg.h"
// This must be the final include in a .cpp or .h file!!!
#include "memdbgon.h"


namespace TGAWriter {
	bool WriteToBuffer( unsigned char* pImageData, CUtlBuffer& buffer, int32 width, int32 height, ImageFormat srcFormat, ImageFormat dstFormat ) {
		AssertUnreachable();
		return {};
	}

	// write out a simple tga file from a memory buffer.
	bool WriteTGAFile( const char* fileName, int32 width, int32 height, enum ImageFormat srcFormat, uint8 const* srcData, int32 nStride ) {
		AssertUnreachable();
		return {};
	}

	// A pair of routines for writing to files without allocating any memory in the TGA writer
	// Useful for very large files such as posters, which are rendered as sub-rects anyway
	bool WriteDummyFileNoAlloc( const char* fileName, int32 width, int32 height, ImageFormat dstFormat ) {
		AssertUnreachable();
		return {};
	}
	bool WriteRectNoAlloc( unsigned char* pImageData, const char* fileName, int32 nXOrigin, int32 nYOrigin, int32 width, int32 height, int32 nStride, ImageFormat srcFormat ) {
		AssertUnreachable();
		return {};
	}
}
