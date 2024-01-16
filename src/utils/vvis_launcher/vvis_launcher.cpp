//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// vvis_launcher.cpp : Defines the entry point for the console application.
//
#include "interface.h"
#include "tier0/icommandline.h"
#include "ilaunchabledll.h"
#include "platform.h"


int main( int argc, char* argv[] ) {
	CommandLine()->CreateCmdLine( argc, argv );
	const char* pDLLName = "vvis_dll" DLL_EXT_STRING;

	CSysModule* pModule = Sys_LoadModule( pDLLName );
	if (! pModule ) {
		printf( "vvis launcher error: can't load %s\n%s", pDLLName, Sys_LastErrorString() );
		return 1;
	}

	CreateInterfaceFn fn = Sys_GetFactory( pModule );
	if (! fn ) {
		printf( "vvis launcher error: can't get factory from %s\n", pDLLName );
		Sys_UnloadModule( pModule );
		return 2;
	}

	int retCode = 0;
	auto* pDLL = static_cast<ILaunchableDLL*>( fn( LAUNCHABLE_DLL_INTERFACE_VERSION, &retCode ) );
	if (! pDLL ) {
		printf( "vvis launcher error: can't get ILaunchableDLL interface from %s\n", pDLLName );
		Sys_UnloadModule( pModule );
		return 3;
	}

	pDLL->main( argc, argv );
	Sys_UnloadModule( pModule );

	return 0;
}

