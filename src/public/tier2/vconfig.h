//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Utilities for setting vproject settings
//
//===========================================================================//
#pragma once


// The registry keys that vconfig uses to store the current vproject directory.
//#define VPROJECT_REG_KEY	"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
// Moved to simply 'Environment' - we need to set in HKEY_CURRENT_USER and not local machine, to avoid security issues in vista!
#define VPROJECT_REG_KEY "Environment"

// For accessing the environment variables we store the current vproject in.
void SetVConfigRegistrySetting( const char* pName, const char* pValue, bool bNotify = true );
bool GetVConfigRegistrySetting( const char* pName, char* pReturn, int size );
#if defined( PLATFORM_WINDOWS )
	bool RemoveObsoleteVConfigRegistrySetting( const char* pValueName, char* pOldValue = NULL, int size = 0 );
#endif
bool ConvertObsoleteVConfigRegistrySetting( const char* pValueName );
