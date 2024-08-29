//
// Created by ENDERZOMBI102 on 30/08/2024.
//
#pragma once


namespace TGA {
	struct ColorMapSpec {
		uint16 entryIndex;
		uint16 entryLength;
		uint8 bpp;
	};

	struct ImageDesc {
		int32 alphaDepth : 4;
		int32 orderRightToLeft : 1;
		int32 orderTopToBottom : 1;
		int32 padding : 2;
	};

	struct ImageSpec {
		uint16 xOrigin;
		uint16 yOrigin;
		uint16 width;
		uint16 height;
		uint8 depth;
		ImageDesc imageDesc;
	};

	enum ColorMapType : uint8 {
		None,
		ColorPalette,
	};

	enum ImageType : uint8 {
		NoData,
		UncompressedColorMapped,
		UncompressedRGB,
		UncompressedGreyscale,
		RLEColorMapped = 9,
		RLERGB,
		RLEGreyscale,
		// huffman + delta + rle
		CompressedColorMapped = 32,
		// huffman + delta + rle, 4-pass quadtree-type
		CompressedColorMapped4Pass,
	};

	struct Header {
		uint8 idLength;
		ColorMapType colorMapType;
		ImageType imageType;
		ColorMapSpec colorMapSpec;
		ImageSpec imageSpec;
	};
}
