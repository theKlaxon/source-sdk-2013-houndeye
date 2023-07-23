import libloader;
import std.stdio;
import std.process;
import core.runtime;
import std.path;


import core.sys.windows.windef;
import core.sys.windows.winbase;
import std.windows.syserror;


int main( string[] argv ) {
	immutable auto binDir = argv[0].absolutePath.dirName ~ dirSeparator ~ "bin";
	writeln( binDir );
	environment["PATH"] =  binDir ~ pathSeparator ~ "D:\\SteamLibrary\\steamapps\\common\\Source SDK Base 2013 Singleplayer\\bin;" ~ environment[ "PATH" ];
	
	auto lib = library( "launcher" );
	
	writeln( "LoadLibraryA: " ~ sysErrorString( GetLastError() ) );
	
	if ( lib.isLoaded() ) {
		auto res = lib.callC!( int, HINSTANCE, HINSTANCE, LPSTR, int )( "LauncherMain", GetModuleHandle( null ), null, GetCommandLineA(), 0 );
		
		writeln( "GetProcAddress: " ~ sysErrorString( GetLastError() ) );
		
		if ( res.isNull() ) {
			writeln( "Failed to launch!" );
			return -1;
		}
		
		writeln( "Done?" );
		
		return res.get();
	} else
		writeln( "Failed to load" );
	
	return -1;
}
