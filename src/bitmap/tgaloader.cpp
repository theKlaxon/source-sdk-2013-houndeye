//
// Created by ENDERZOMBI102 on 12/08/2024.
//
#include "bitmap/tgaloader.h"
#include "filesystem.h"
#include "dbg.h"
// This must be the final include in a .cpp or .h file!!!
#include "memdbgon.h"


namespace TGALoader {
	int32 TGAHeaderSize() {
		return 18;
	}

	bool GetInfo( const char* fileName, int32* width, int32* height, ImageFormat* imageFormat, float* sourceGamma ) {
		return {};
	}
	bool GetInfo( CUtlBuffer& buf, int32* width, int32* height, ImageFormat* imageFormat, float* sourceGamma ) {
		return {};
	}

	bool Load( unsigned char* imageData, const char* fileName, int32 width, int32 height, ImageFormat imageFormat, float targetGamma, bool mipmap ) {
		return {};
	}
	bool Load( unsigned char* imageData, CUtlBuffer& buf, int32 width, int32 height, ImageFormat imageFormat, float targetGamma, bool mipmap ) {
		return {};
	}

	bool LoadRGBA8888( const char* pFileName, CUtlMemory<unsigned char>& outputData, int32& outWidth, int32& outHeight ) {
		return {};
	}
	bool LoadRGBA8888( CUtlBuffer& buf, CUtlMemory<unsigned char>& outputData, int32& outWidth, int32& outHeight ) {
		return {};
	}
}