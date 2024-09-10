//
// Created by ENDERZOMBI102 on 09/09/2024.
//
#include "wildcard/wildcard.hpp"
// memdbgon must be the last include file in a .cpp file!!!
#include "memdbgon.h"


namespace Wildcard {
	auto SimpleMatch( const char* pString, const char* pPattern, const int32 pStringLen, const int32 pPatternLen ) -> bool { // NOLINT(*-no-recursion)
		// empty pattern can only match with empty string
		if ( pPatternLen == 0 ) {
			return pStringLen == 0;
		}

		// Empty string can match only with one or more occurrences of '*'
		if ( pStringLen == 0 ) {
			for ( auto i{0}; i < pPatternLen; i++ ) {
				if ( pPattern[i] != '*' ) {
					return false;
				}
			}
			return true;
		}

		// The last characters match or pattern has '?'
		if ( pString[pStringLen - 1] == pPattern[pPatternLen - 1] || pPattern[pPatternLen - 1] == '?' ) {
			return SimpleMatch( pString, pPattern, pStringLen - 1, pPatternLen - 1 );
		}

		// If last character is '*', we either ignore it or match it with a character
		if ( pPattern[pPatternLen - 1] == '*' ) {
			return SimpleMatch( pString, pPattern, pStringLen, pPatternLen - 1 ) || SimpleMatch( pString, pPattern, pStringLen - 1, pPatternLen );
		}

		return false;
	}
}
