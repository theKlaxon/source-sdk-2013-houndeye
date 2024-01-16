//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#include <type_traits>

//-----------------------------------------------------------------------------
// Purpose: Basic handler for a rgb set of colors
//			This class is fully inline
//-----------------------------------------------------------------------------
class Color {
public:
	// constructors
	constexpr Color() = default;
	constexpr Color( int _r, int _g, int _b ) {
		this->SetColor( _r, _g, _b, 0 );
	}
	constexpr Color( int _r, int _g, int _b, int _a ) {
		this->SetColor( _r, _g, _b, _a );
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	constexpr void SetColor( int _r, int _g, int _b, int _a = 0 ) {
		_color[ 0 ] = static_cast<unsigned char>( _r );
		_color[ 1 ] = static_cast<unsigned char>( _g );
		_color[ 2 ] = static_cast<unsigned char>( _b );
		_color[ 3 ] = static_cast<unsigned char>( _a );
	}

	constexpr void GetColor( int& _r, int& _g, int& _b, int& _a ) const {
		_r = _color[ 0 ];
		_g = _color[ 1 ];
		_b = _color[ 2 ];
		_a = _color[ 3 ];
	}

	void SetRawColor( int color32 ) {
		*reinterpret_cast<int*>( this ) = color32;
	}

	[[nodiscard]] int GetRawColor() const {
		return *reinterpret_cast<const int*>( this );
	}

	[[nodiscard]] inline int r() const { return _color[ 0 ]; }
	[[nodiscard]] inline int g() const { return _color[ 1 ]; }
	[[nodiscard]] inline int b() const { return _color[ 2 ]; }
	[[nodiscard]] inline int a() const { return _color[ 3 ]; }

	constexpr unsigned char& operator[]( int index ) {
		return _color[ index ];
	}

	// Thanks, @JJl77! this the most readable version of operator[] i've ever seen
	template<typename Integral>
	constexpr inline const unsigned char& operator[]( const Integral index ) const noexcept requires std::is_integral_v<Integral> && std::is_convertible_v<Integral, unsigned> {
		return _color[ static_cast<int>( index ) ];
	}

	bool operator==( const Color& rhs ) const noexcept {
		return *reinterpret_cast<const int*>( this ) == *reinterpret_cast<const int*>( &rhs );
	}

	bool operator!=( const Color& rhs ) const {
		return ! this->operator==( rhs );
	}

	Color& operator=( const Color& rhs ) {
		if ( &rhs == this )
			return *this;

		this->SetRawColor( rhs.GetRawColor() );
		return *this;
	}

private:
	unsigned char _color[ 4 ] { 0 };
};
