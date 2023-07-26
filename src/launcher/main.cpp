//
// Created by ENDERZOMBI102 on 26/07/2023.
//

#include "appframework/AppFramework.h"
#include "icommandline.h"
#include "launcher.hpp"


//-----------------------------------------------------------------------------
//   Main entry point
//-----------------------------------------------------------------------------
int LauncherMain( int argc, char* argv[] ) {
	CommandLine()->CreateCmdLine( argc, argv );

	CLauncherDS launcherds;
	CSteamApplication steamApplication( &launcherds );
	return steamApplication.Run();
}
