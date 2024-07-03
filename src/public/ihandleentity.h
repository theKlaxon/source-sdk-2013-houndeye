//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once

class CBaseHandle;

// An IHandleEntity-derived class can go into an entity list and use ehandles.
class IHandleEntity {
public:
	virtual ~IHandleEntity() = default;
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	[[nodiscard]]
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};
