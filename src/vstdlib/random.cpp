#include "vstdlib/random.h"

IUniformRandomStream* g_pUniformRandomStream;



// --- CUniformRandomStream ---
static const uint64_t multiplier{ 6364136223846793005u };
static const uint64_t increment{ 1442695040888963407u };

CUniformRandomStream::CUniformRandomStream() = default;
void CUniformRandomStream::SetSeed( int iSeed ) {
	this->m_liState = static_cast<uint32_t>( iSeed ) + increment;
	this->GenerateRandomNumber();
}

float CUniformRandomStream::RandomFloat( float flMinVal, float flMaxVal ) {
	// map from range of `0 ..= UINT32::MAX` to range of `flMin ..= flMax`
	auto res{ flMaxVal + static_cast<double>( this->GenerateRandomNumber() - 0 ) * ( flMaxVal - flMinVal ) / ( std::numeric_limits<uint32_t>::max() - 0 ) };
	return static_cast<float>( res );
}
int CUniformRandomStream::RandomInt( int iMinVal, int iMaxVal ) {
	// NOTE: For selecting within a range we use code "inspired" from [here](https://www.pcg-random.org/posts/bounded-rands.html)
	uint32_t bound = ( iMaxVal - iMinVal ) + 1;
	uint32_t threshold = -bound % bound;

	int res;
	do res = this->GenerateRandomNumber();
	while ( res < threshold );

	return static_cast<int>( res % bound ) + iMinVal;
}
float CUniformRandomStream::RandomFloatExp( float flMinVal, float flMaxVal, float flExponent ) {
	// map from range of `0 ..= UINT32::MAX` to range of `flMin ..= flMax`
	auto res{ flMaxVal + pow( static_cast<double>( this->GenerateRandomNumber() - 0 ), flExponent ) * ( flMaxVal - flMinVal ) / ( std::numeric_limits<uint32_t>::max() - 0 ) };
	return static_cast<float>( res );
}

int CUniformRandomStream::GenerateRandomNumber() {
	// NOTE: Adapted from https://en.wikipedia.org/wiki/Permuted_congruential_generator
	static constexpr auto rotr32 = []( uint32_t x, unsigned r ) constexpr -> uint32_t {
		return x >> r | x << ( -r & 31 );
	};

	uint64_t x{ this->m_liState };
	auto count{ static_cast<unsigned>( x >> 59 ) };

	this->m_liState = x * multiplier + increment;
	x ^= x >> 18;

	return static_cast<int>( rotr32( static_cast<uint32_t>( x >> 27 ), count ) );
}

// --- CGaussianRandomStream ---


// --- globals ---
void RandomSeed( int iSeed ) {
	g_pUniformRandomStream->SetSeed( iSeed );
}
float RandomFloat( float flMinVal, float flMaxVal ) {
	g_pUniformRandomStream->RandomFloat( flMinVal, flMaxVal );
}
float RandomFloatExp( float flMinVal, float flMaxVal, float flExponent ) {
	g_pUniformRandomStream->RandomFloatExp( flMinVal, flMaxVal, flExponent );
}
int RandomInt( int iMinVal, int iMaxVal ) {
	g_pUniformRandomStream->RandomInt( iMinVal, iMaxVal );
}
float RandomGaussianFloat( float flMean, float flStdDev );

void InstallUniformRandomStream( IUniformRandomStream* pStream ) {
	g_pUniformRandomStream = pStream;
}
