//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Purpose: Interface to handle results of SQL queries
//-----------------------------------------------------------------------------
class ISQLDBReplyTarget {
public:
	// handles a response from the database
	virtual void SQLDBResponse( int cmdID, int returnState, int returnVal, void* data ) = 0;

	// called from a seperate thread; tells the reply target that a message is waiting for it
	virtual void WakeUp() = 0;
};
