//
// Created by ENDERZOMBI102 on 12/08/2024.
//
#include "bitmap/tgaloader.h"
#include "filesystem.h"
#include "dbg.h"
#include "utlbuffer.h"
#include "tgaspec.hpp"
// This must be the final include in a .cpp or .h file!!!
#include "memdbgon.h"


namespace TGALoader {
	int32 TGAHeaderSize() {
		return 18;
	}

	bool GetInfo( const char* fileName, int32* width, int32* height, ImageFormat* imageFormat, float* sourceGamma ) {
		const auto handle{ g_pFullFileSystem->Open( fileName, "rb" ) };
		if (! handle ) {
			return false;
		}

		// read tga header
		TGA::Header header{};
		const auto readRes = g_pFullFileSystem->Read( &header, sizeof(TGA::Header), handle );
		g_pFullFileSystem->Close( handle );
		if ( readRes == -1 ) {
			return false;
		}

		// the data!
		*width = header.imageSpec.width;
		*height = header.imageSpec.height;
		*imageFormat = ImageFormat::IMAGE_FORMAT_UNKNOWN;
//		*sourceGamma = .0;

		return true;
	}
	bool GetInfo( CUtlBuffer& buf, int32* width, int32* height, ImageFormat* imageFormat, float* sourceGamma ) {
		// read tga header
		TGA::Header header{};
		buf.GetUpTo( &header, sizeof(TGA::Header) );

		// the data!
		*width = header.imageSpec.width;
		*height = header.imageSpec.height;
		*imageFormat = ImageFormat::IMAGE_FORMAT_UNKNOWN;
//		*sourceGamma = .0;

		return true;
	}

	bool Load( unsigned char* imageData, const char* fileName, int32 width, int32 height, ImageFormat imageFormat, float targetGamma, bool mipmap ) {
		AssertUnreachable();
		return {};
	}
	bool Load( unsigned char* imageData, CUtlBuffer& buf, int32 width, int32 height, ImageFormat imageFormat, float targetGamma, bool mipmap ) {
		AssertUnreachable();
		return {};
	}

	bool LoadRGBA8888( const char* pFileName, CUtlMemory<unsigned char>& outputData, int32& outWidth, int32& outHeight ) {
		AssertUnreachable();
		return {};
	}
	bool LoadRGBA8888( CUtlBuffer& buf, CUtlMemory<unsigned char>& outputData, int32& outWidth, int32& outHeight ) {
		AssertUnreachable();
		return {};
	}
}
