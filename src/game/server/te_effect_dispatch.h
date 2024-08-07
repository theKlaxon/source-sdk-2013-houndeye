//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "effect_dispatch_data.h"
#include "recipientfilter.h"


void DispatchEffect( const char* pName, const CEffectData& data );
void DispatchEffect( const char* pName, const CEffectData& data, CRecipientFilter& filter );
