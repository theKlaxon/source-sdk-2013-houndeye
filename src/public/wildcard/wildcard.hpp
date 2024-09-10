//
// Created by ENDERZOMBI102 on 09/09/2024.
//
#pragma once
#include "tier0/platform.h"


namespace Wildcard {
	/**
	 * Performs unoptimized wildcard matching.
	 * @param pString
	 * @param pPattern
	 * @param pStringLen
	 * @param pPatternLen
	 * @return
	 */
	auto SimpleMatch( const char* pString, const char* pPattern, int32 pStringLen, int32 pPatternLen ) -> bool;
}
