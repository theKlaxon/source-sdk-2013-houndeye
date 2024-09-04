//
// Created by ENDERZOMBI102 on 22/07/2023.
//
#include "steamapploader.hpp"
#include "tier0/icommandline.h"
#include "appframework/AppFramework.h"
// This must be the final include in a .cpp file!!!
#include "memdbgon.h"


SpewRetval_t LauncherSpewFunc( SpewType_t spewType, char const* pMsg );

//-----------------------------------------------------------------------------
//   Main entry point
//-----------------------------------------------------------------------------
DLL_EXPORT int LauncherMain( int argc, char* argv[] ) {
	// first init the CommandLine
	CommandLine()->CreateCmdLine( argc, argv );

	// then set the Spew function, so we can log stuff as soon as possible
	SpewOutputFunc( LauncherSpewFunc );

	// are we in a debug session?
	if ( Plat_IsInDebugSession() ) {
		Log( "running with debugger attached!\n" );
	}

	// check we have a game in our hands
	if (! CommandLine()->FindParm( "-game" ) ) {
		Warning( "was not given a game to load!! defaulting to `Half-Life 2`" );
		CommandLine()->AppendParm( "-game", "hl2" );
	}

	// then init and launch Source
	CSteamAppLoader loader;
	CSteamApplication app{ &loader };
	return app.Run();
}

SpewRetval_t LauncherSpewFunc( SpewType_t spewType, char const* pMsg ) {
	switch ( spewType ) {
		case SPEW_MESSAGE:
			printf( "[I] %s", pMsg );
			break;
		case SPEW_WARNING:
			printf( "[W] %s", pMsg );
			break;
		case SPEW_ASSERT:
			printf( "[assert] %s", pMsg );
			break;
		case SPEW_ERROR:
			printf( "[E] %s", pMsg );
			break;
		case SPEW_LOG:
			printf( "[D] %s", pMsg );
			break;
		case SPEW_TYPE_COUNT:
			break;
	}
	return SpewRetval_t::SPEW_CONTINUE;
}
