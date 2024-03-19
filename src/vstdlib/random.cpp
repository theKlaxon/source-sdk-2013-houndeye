#include "vstdlib/random.h"
#include <random>
#include <iostream>

IUniformRandomStream* g_pUniformRandomStream;

// --- CUniformRandomStream ---
CUniformRandomStream::CUniformRandomStream() = default;
void CUniformRandomStream::SetSeed( int iSeed ) {
	this->m_Mutex.Lock();
	this->m_Engine.seed( iSeed );
	this->m_Mutex.Unlock();
}

float CUniformRandomStream::RandomFloat( float flMinVal, float flMaxVal ) {
	this->m_Mutex.Lock();
	auto res{ std::uniform_real_distribution{ flMinVal, flMaxVal }( this->m_Engine ) };
	this->m_Mutex.Unlock();
	return res;
}
int CUniformRandomStream::RandomInt( int iMinVal, int iMaxVal ) {
	this->m_Mutex.Lock();
	auto res{ std::uniform_int_distribution{ iMinVal, iMaxVal }( this->m_Engine ) };
	this->m_Mutex.Unlock();
	return res;
}
float CUniformRandomStream::RandomFloatExp( float flMinVal, float flMaxVal, float flExponent ) {
	this->m_Mutex.Lock();
	auto res{ std::uniform_real_distribution{ flMinVal, flMaxVal }( this->m_Engine ) };
	this->m_Mutex.Unlock();
	return pow( res, flExponent );
}

// --- CGaussianRandomStream ---
CGaussianRandomStream::CGaussianRandomStream( IUniformRandomStream* pUniformStream ) {
	this->m_pUniformStream = pUniformStream ? pUniformStream : g_pUniformRandomStream;
}
void CGaussianRandomStream::AttachToStream( IUniformRandomStream* pUniformStream ) {
	this->m_pUniformStream = pUniformStream ? pUniformStream : g_pUniformRandomStream;
}
float CGaussianRandomStream::RandomFloat( float flMean, float flStdDev ) {
	return std::normal_distribution{ flMean, flStdDev }( dynamic_cast<CUniformRandomStream*>( this->m_pUniformStream )->m_Engine );
}

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
