//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once

//-----------------------------------------------------------------------------
// Purpose: Generic interface for routing messages to users
//-----------------------------------------------------------------------------
class IRecipientFilter {
public:
	virtual ~IRecipientFilter() {}

	virtual bool IsReliable() const = 0;
	virtual bool IsInitMessage() const = 0;

	virtual int GetRecipientCount() const = 0;
	virtual int GetRecipientIndex( int slot ) const = 0;
};
