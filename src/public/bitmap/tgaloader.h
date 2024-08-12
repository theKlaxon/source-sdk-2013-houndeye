//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//===========================================================================//
#pragma once
#include "bitmap/imageformat.h"
#include "tier1/utlmemory.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CUtlBuffer;


namespace TGALoader {
	int32 TGAHeaderSize();

	bool GetInfo( const char* fileName, int32* width, int32* height, ImageFormat* imageFormat, float* sourceGamma );
	bool GetInfo( CUtlBuffer& buf, int32* width, int32* height, ImageFormat* imageFormat, float* sourceGamma );

	bool Load( unsigned char* imageData, const char* fileName, int32 width, int32 height,
			   ImageFormat imageFormat, float targetGamma, bool mipmap );
	bool Load( unsigned char* imageData, CUtlBuffer& buf, int32 width, int32 height,
			   ImageFormat imageFormat, float targetGamma, bool mipmap );

	bool LoadRGBA8888( const char* pFileName, CUtlMemory<unsigned char>& outputData, int32& outWidth, int32& outHeight );
	bool LoadRGBA8888( CUtlBuffer& buf, CUtlMemory<unsigned char>& outputData, int32& outWidth, int32& outHeight );
}
