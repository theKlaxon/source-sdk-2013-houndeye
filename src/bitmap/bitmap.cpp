//
// Created by ENDERZOMBI102 on 12/08/2024.
//
#include "bitmap/bitmap.h"
#include "filesystem.h"
#include "dbg.h"
// This must be the final include in a .cpp or .h file!!!
#include "memdbgon.h"


void Bitmap_t::Init( int32 nWidth, int32 nHeight, ImageFormat imageFormat, int32 nStride ) {
	if ( m_bOwnsBuffer ) {}
}

void Bitmap_t::SetBuffer( int32 nWidth, int32 nHeight, ImageFormat imageFormat, unsigned char* pBits, bool bAssumeOwnership, int32 nStride ) {
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_ImageFormat = imageFormat;
	m_pBits = pBits;
	m_bOwnsBuffer = bAssumeOwnership;
	m_nStride = nStride;
	m_nPixelSize = ImageLoader::ImageFormatInfo(imageFormat).m_NumBytes;
}

void Bitmap_t::Clear() { AssertUnreachable(); }

bool Bitmap_t::IsValid() const {
	// TODO: Maybe do a better check?
	return m_pBits == nullptr;
}

Color Bitmap_t::GetColor( int32 x, int32 y ) const {
	auto* pix{ m_pBits + ( y * m_nStride ) + x * m_nPixelSize };

	AssertUnreachable();
	return {};
}

void Bitmap_t::SetColor( int32 x, int32 y, Color c ) {
	AssertUnreachable();
}

void Bitmap_t::MakeLogicalCopyOf( Bitmap_t& src, bool bTransferBufferOwnership ) {
	AssertUnreachable();
}

void Bitmap_t::Crop( int32 x0, int32 y0, int32 nWidth, int32 nHeight, const Bitmap_t* pImgSource ) {
	AssertUnreachable();
}

void Bitmap_t::SetPixelData( const Bitmap_t& src, int32 nSrcX1, int32 nSrcY1, int32 nCopySizeX, int32 nCopySizeY, int32 nDestX1, int32 nDestY1 ) {
	AssertUnreachable();
}

void Bitmap_t::SetPixelData( const Bitmap_t& src, int32 nDestX1, int32 nDestY1 ) {
	AssertUnreachable();
}
