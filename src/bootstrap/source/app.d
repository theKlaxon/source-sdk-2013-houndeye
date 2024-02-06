import libloader;
import std.stdio;
import std.path;
import std.typecons;
import std.process;
import std.string : toStringz;


int main( string[] argv ) {
	auto binDir = dirName(argv[0]);
	environment["LD_LIBRARY_PATH"] = binDir;
	auto lib = library( binDir ~ "/launcher" );


	if (! lib.isLoaded() ) {
		writeln( "Failed to load `launcher` dll: " ~ lib.getLastError() );
		return 1;
	}

	auto args = new char*[ argv.length ];

	for ( auto i = 0; i < argv.length; i++ )
		args[i] = cast(char*) argv[i].toStringz();

	Nullable!int res = lib.callC!( int, int, char** )( "LauncherMain", cast(int)( args.length ), args.ptr );

	if ( res.isNull() ) {
		writeln( "Failed to load `LauncherMain`: " ~ lib.getLastError() );
		return -1;
	}

	return res.get();
}
