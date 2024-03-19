//
// Created by ENDERZOMBI102 on 19/03/2024.
//
#include "vstdlib/pcgengine.hpp"

#include <iostream>

void PcgEngine::seed() {
	this->m_luState = 0xEBABEFF0C0f33173;
	this->operator()();  // when seeding, also "tick" the state.
}
void PcgEngine::seed( uint32_t uSeed ) {
	this->m_luState = uSeed + increment;
	this->operator()();
}
// e.seed(q)
void PcgEngine::seed( std::random_device& pOther ) {
	this->m_luState = pOther() + increment;
	this->operator()();
}

// e.discard(z)
void PcgEngine::discard( unsigned long long n ) {
	for ( auto i{ 0 }; i < n; i++ )
		this->operator()();
}

// x == y
bool PcgEngine::operator==( const PcgEngine& rhs ) const {
	return this->m_luState == rhs.m_luState;
}
// x != y
bool PcgEngine::operator!=( const PcgEngine& rhs ) const {
	return this->m_luState != rhs.m_luState;
}

// os << x
std::ostream& operator<<( std::ostream& os, const PcgEngine& pcgEngine ) {
	return os << pcgEngine.m_luState;
}
// is >> x
std::istream& operator>>( std::istream& is, PcgEngine& pEngine ) {
	return is >> pEngine.m_luState;
}