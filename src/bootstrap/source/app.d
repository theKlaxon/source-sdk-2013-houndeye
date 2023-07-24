import libloader;
import std.stdio;
import std.process;
import core.runtime;
import std.path;
import std.conv;
import std.typecons;

string lastError() {
	version ( Windows ) {
		import std.windows.syserror;
		import core.sys.windows.winbase;

		return sysErrorString( GetLastError() );
	} else {
		import core.sys.posix.dlfcn;

		return to!string( dlerror() );
	}
}


int main( string[] argv ) {
	immutable auto binDir = argv[0].absolutePath.dirName ~ dirSeparator ~ "bin";
	writeln( "bin directory is at " ~ binDir );

	// Temporarily hardcode SDK2013MP path
	environment["PATH"] =  binDir ~ pathSeparator ~ "D:\\SteamLibrary\\steamapps\\common\\Source SDK Base 2013 Multiplayer\\bin;" ~ environment[ "PATH" ];

	auto lib = library( "launcher" );

	if (! lib.isLoaded() ) {
		writeln( "Failed to load `launcher` dll, look above for errors" ~ lastError() );
		return 1;
	}

	Nullable!int res;
	version ( Windows ) {
		import core.sys.windows.windef;
		import core.sys.windows.winbase;
		res = lib.callC!( int, HINSTANCE, HINSTANCE, LPSTR, int )( "LauncherMain", GetModuleHandle( null ), null, GetCommandLineA(), 0 );
	} else {
		char*[] args = new char*[ argv.length ];
		res = lib.callC!( int, int, char** )( "LauncherMain", argv.sizeof, args.ptr );
	}

	if ( res.isNull() ) {
		writeln( "Failed to load `LauncherMain`: " ~ lastError() );
		return -1;
	}

	return res.get();
}
