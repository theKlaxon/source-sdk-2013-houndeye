//
// Created by ENDERZOMBI102 on 19/03/2024.
//
#pragma once

#include <random>

class PcgEngine {
public:
	using result_type = uint32_t;

	// E()
	PcgEngine() : m_luState{ 0xEBABEFF0C0f33173 } { }
	// E(x)
	PcgEngine( const PcgEngine& pOther ) = default;// copy-constructor
	// E(s)
	explicit PcgEngine( const uint32_t uSeed ) : m_luState{ static_cast<uint32_t>( uSeed ) + increment } { };
	// E(q)
	explicit PcgEngine( std::random_device& pOther ) : m_luState{ static_cast<uint32_t>( pOther() ) + increment } { };

	// e.seed()
	void seed();
	// e.seed(s)
	void seed( uint32_t uSeed );
	// e.seed(q)
	void seed( std::random_device& pOther );

	// e()
	result_type operator()() {
		// NOTE: Adapted from https://en.wikipedia.org/wiki/Permuted_congruential_generator
		static constexpr auto rotr32 = []( uint32_t x, unsigned r ) constexpr -> uint32_t {
			return x >> r | x << ( -r & 31 );
		};

		uint64_t x{ this->m_luState };
		auto count{ static_cast<unsigned>( x >> 59 ) };

		this->m_luState = x * multiplier + increment;
		x ^= x >> 18;

		return rotr32( static_cast<uint32_t>( x >> 27 ), count );
	}
	// e.discard(z)
	void discard( unsigned long long n );

	// x == y
	bool operator==( const PcgEngine& rhs ) const;
	// x != y
	bool operator!=( const PcgEngine& rhs ) const;

	// os << x
	friend std::ostream& operator<<( std::ostream& os, const PcgEngine& pcgEngine );
	// is >> x
	friend std::istream& operator>>( std::istream& is, PcgEngine& pEngine );

	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<uint32_t>::max(); }
private:
	uint64_t m_luState;

	static constexpr uint64_t multiplier{ 6364136223846793005u };
	static constexpr uint64_t increment{ 1442695040888963407u };
};