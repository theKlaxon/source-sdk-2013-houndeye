//
// Created by ENDERZOMBI102 on 10/02/2024.
//
#include "tier0/platform.h"
#include "tier0/dbg.h"
#include <cstdio>
#include <ctime>
#include <fstream>

#if IsWindows()
	#include <sysinfoapi.h>
#endif

double MonotonicTime() {
	#if IsWindows()
		static LARGE_INTEGER freq{}; // cache the value
		if ( freq.QuadPart == 0 ) {
			if (! QueryPerformanceFrequency( &freq ) )
				freq.QuadPart = 0;
		}

		LARGE_INTEGER time;
		// do we have a valid frequency? if so, use the performance counter
		if ( freq.QuadPart > 0 && QueryPerformanceCounter( &time ) ) {
			return static_cast<double>( time.QuadPart ) * 1000 / freq.QuadPart;
		}

		// last resort
		return static_cast<double>( GetTickCount64() ) / 1000;
	#elif IsLinux()
		std::timespec ts{};
		std::timespec_get( &ts, CLOCK_MONOTONIC ); // `CLOCK_MONOTONIC` is a linux extension to C's time library
		return static_cast<double>( ts.tv_sec ) + static_cast<double>( ts.tv_nsec ) / 1e9;
	#endif
}

static bool g_bBenchmarkMode{ false };
static struct ModuleData {
	double m_LoadTime;

	ModuleData() : m_LoadTime( MonotonicTime() ) { };
} g_ModuleData{};


void Plat_SetBenchmarkMode( bool bBenchmarkMode ) {
	g_bBenchmarkMode = bBenchmarkMode;
}
bool Plat_IsInBenchmarkMode() {
	return g_bBenchmarkMode;
}


double Plat_FloatTime() {
	return static_cast<double>( MonotonicTime() - g_ModuleData.m_LoadTime );
}
unsigned int Plat_MSTime() {
	return static_cast<unsigned int>( ( MonotonicTime() - g_ModuleData.m_LoadTime ) * 1000 );
}
char* Plat_ctime( const time_t* timep, char* buf, size_t bufsize ) {
	return ctime_r( timep, buf );  // TODO: Verify this doesn't exceed the buffer
}
struct tm* Plat_gmtime( const time_t* timep, struct tm* result ) {
	return gmtime_r( timep, result );
}
time_t Plat_timegm( struct tm* timeptr ) {
	return timegm( timeptr );
}
struct tm* Plat_localtime( const time_t* timep, struct tm* result ) {
	return localtime_r( timep, result );
}

const CPUInformation* GetCPUInformation();

float GetCPUUsage();

void GetCurrentDate( int* pDay, int* pMonth, int* pYear ) {
	auto lTime{ time( nullptr ) };
	auto lDate{ localtime( &lTime ) };

	*pDay =lDate->tm_mday;
	*pMonth = lDate->tm_mon;
	*pYear = lDate->tm_year;
}

void InitPME();
void ShutdownPME();

void Plat_SetHardwareDataBreakpoint( const void* pAddress, int nWatchBytes, bool bBreakOnRead );
void Plat_ApplyHardwareDataBreakpointsToNewThread( unsigned long dwThreadID );

const tchar* Plat_GetCommandLine() {
	static tchar cmdline[2048] { 0 };
	if ( cmdline[0] != '\0' )
		return cmdline;

	auto file{ std::fopen( "/proc/self/cmdline", "r" ) };
	std::fread( cmdline, 1, sizeof( cmdline ) - 1, file );
	std::fclose( file );

	return cmdline;
}
#if ! IsWindows()
	void Plat_SetCommandLine( const char* cmdLine ) {
	AssertUnreachable();
}
#endif
const char* Plat_GetCommandLineA() {
	static char cmdline[2048] { 0 };
	if ( cmdline[0] != '\0' )
		return cmdline;

	auto file{ std::fopen( "/proc/self/cmdline", "r" ) };
	std::fread( cmdline, 1, sizeof( cmdline ) - 1, file );
	std::fclose( file );

	return cmdline;
}

bool Plat_VerifyHardwareKeyDriver();

bool Plat_VerifyHardwareKey();

bool Plat_VerifyHardwareKeyPrompt();

bool Plat_FastVerifyHardwareKey();

void* Plat_SimpleLog( const tchar* file, int line );

#if IsWindows() || IsLinux()
	PLATFORM_INTERFACE bool Plat_IsInDebugSession() {
		static char buffer[32] { 0 };
		std::ifstream file{ "/proc/self/status" };

		int tracer;
		while ( file.getline( buffer, sizeof( buffer )  - 1 ) ) {
			if ( sscanf( buffer, "TracerPid: %d", &tracer ) == 1 ) { // NOLINT(*-err34-c), using it to pattern-match the input string...
				return tracer != 0;
			}
		}

		return false;
	}
	PLATFORM_INTERFACE void Plat_DebugString( const char* );
#else
	inline bool Plat_IsInDebugSession( bool bForceRecheck = false ) { return false; }
	#define Plat_DebugString( s ) ( (void) 0 )
#endif

bool Is64BitOS();