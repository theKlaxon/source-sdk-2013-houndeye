import libloader;
import std.stdio;
import std.process;
import core.runtime;
import std.path;
import std.conv;
import std.typecons;
import std.string : toStringz;
import std.algorithm : canFind;


int main( string[] argv ) {
	immutable auto binDir = argv[0].absolutePath.dirName ~ dirSeparator ~ "bin";
	writeln( "bin directory is at " ~ binDir );

	// Temporarily hardcode SDK2013MP path
	environment["PATH"] = binDir ~ pathSeparator ~ environment["SOURCE_SDK_2013_MP"] ~ pathSeparator ~ environment[ "PATH" ];

	immutable auto dedicated = argv.canFind( "--dedicated" );
	if ( dedicated )
		writeln( "starting Aurora Source dedicated server..." );

	immutable auto libname = dedicated ? "dedicated" : "launcher";
	immutable auto funcname = dedicated ? "DedicatedMain" : "LauncherMain";

	auto lib = library( libname );

	if (! lib.isLoaded() ) {
		writeln( "Failed to load `" ~ libname ~ "` dll:" ~ lib.getLastError() );
		return 1;
	}

	auto args = new char*[ argv.length ];

	for ( auto i = 0; i < argv.length; i++ )
		args[i] = ( argv[i] ~ "\0" ).dup().ptr;

	Nullable!int res = lib.callC!( int, int, char** )( funcname, args.length, args.ptr );

	if ( res.isNull() ) {
		writeln( "Failed to load `" ~ funcname ~ "`: " ~ lib.getLastError() );
		return -1;
	}

	return res.get();
}
