//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A simple tool for coverage tests
//
//=============================================================================
#pragma once

#include "tier1/utlrbtree.h"
#include "vstdlib.h"

class CVCoverage {
public:
	CVCoverage() = default;

	[[nodiscard]] bool IsActive() const {
		return this->m_bActive;
	}

	void SetActive( bool bActive ) {
		Assert( bActive != this->m_bActive );
		this->m_bActive = bActive;
		if ( bActive )
			++this->m_token;
	}

	void Begin() {
		++this->m_depth;
	}

	void End() {
		--this->m_depth;
	}

	void Reset() {
		this->m_locations.RemoveAll();
	}

	[[nodiscard]] bool ShouldCover( unsigned token ) const {
		return this->m_bActive && this->m_depth > 0 && token != this->m_token;
	}

	unsigned Cover( const char* pszFile, int line ) {
		Location_t location = { pszFile, line };

		this->m_locations.Insert( location );

		return this->m_token;
	}

	void Report() {
		for ( int i = this->m_locations.FirstInorder(); i != CUtlRBTree<CVCoverage::Location_t>::InvalidIndex(); i = this->m_locations.NextInorder( i ) ) {
			Msg( "%s(%d) :\n", this->m_locations[ i ].pszFile, this->m_locations[ i ].line );
		}
	}

private:
	struct Location_t {
		const char* pszFile;
		int line;
	};

	class CLocationLess {
	public:
		explicit CLocationLess( int ignored ) { }
		bool operator!() { return false; }

		bool operator()( const Location_t& lhs, const Location_t& rhs ) const {
			if ( lhs.line < rhs.line ) {
				return true;
			}

			return CaselessStringLessThan( lhs.pszFile, rhs.pszFile );
		}
	};

	bool m_bActive{ false };
	int m_depth{ 0 };
	unsigned m_token{ 1 };

	CUtlRBTree<Location_t, unsigned short, CLocationLess> m_locations;
};

VSTDLIB_INTERFACE CVCoverage g_VCoverage;

#if defined( VCOVER_ENABLED )
	#define VCOVER()                                             \
		do {                                                     \
			static token;                                        \
			if ( g_VCoverage.ShouldCover( token ) ) {            \
				token = g_VCoverage.Cover( __FILE__, __LINE__ ); \
			}                                                    \
		} while ( 0 )
#else
	#define VCOVER() ( (void) 0 )
#endif
