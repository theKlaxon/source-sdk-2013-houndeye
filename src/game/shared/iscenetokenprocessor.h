//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once

abstract_class ISceneTokenProcessor {
public:
	virtual const char* CurrentToken() = 0;
	virtual bool GetToken( bool crossline ) = 0;
	virtual bool TokenAvailable() = 0;
	virtual void Error( PRINTF_FORMAT_STRING const char* fmt, ... ) = 0;
};
