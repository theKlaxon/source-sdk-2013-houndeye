//
// Created by ENDERZOMBI102 on 09/02/2024.
//
#include "tier0/dbg.h"
#include "icommandline.h"
#include <cassert>
#include <string>

static SpewOutputFunc_t g_pSpewOutFunction{ DefaultSpewFunc };
static AssertFailedNotifyFunc_t g_pAssertFailedListener{ nullptr };
static bool g_bAssertionsDisabled{ false };
static SDL_Window* g_pDialogParent{ nullptr };


void SpewOutputFunc( SpewOutputFunc_t func ) {
	if ( func )
		g_pSpewOutFunction = func;
	else
		g_pSpewOutFunction = DefaultSpewFunc;
}

SpewOutputFunc_t GetSpewOutputFunc() {
	return g_pSpewOutFunction;
}

SpewRetval_t DefaultSpewFunc( SpewType_t pSpewType, const tchar* pMsg ) {
	switch ( pSpewType ) {
		case SPEW_MESSAGE:
			printf( "[I] %s", pMsg );
			break;
		case SPEW_WARNING:
			printf( "[W] %s", pMsg );
			break;
		case SPEW_ASSERT:
			printf( "[A] %s", pMsg );
			return SpewRetval_t::SPEW_DEBUGGER;
		case SPEW_ERROR:
			printf( "[E] %s", pMsg );
			break;
		case SPEW_LOG:
			printf( "[D] %s", pMsg );
			break;
		default:
			printf( "Invalid spew type: %d (msg=`%s`)", pSpewType, pMsg );
			return SpewRetval_t::SPEW_DEBUGGER;
	}
	return SpewRetval_t::SPEW_CONTINUE;
}

SpewRetval_t DefaultSpewFuncAbortOnAsserts( SpewType_t pSpewType, const tchar* pMsg ) {
	auto res{ DefaultSpewFunc( pSpewType, pMsg ) };
	if ( pSpewType == SpewType_t::SPEW_ASSERT )
		return SpewRetval_t::SPEW_DEBUGGER;

	return res;
}

// TODO: Implement these
const tchar* GetSpewOutputGroup() { AssertUnreachable(); }
int GetSpewOutputLevel() { AssertUnreachable(); }
const Color* GetSpewOutputColor() { AssertUnreachable(); }

// TODO: Implement these
void SpewActivate( const tchar* pGroupName, int level ) { assert( false ); }
bool IsSpewActive( const tchar* pGroupName, int level ) { assert( false ); }

// TODO: Implement these
void _SpewInfo( SpewType_t type, const tchar* pFile, int line ) { assert( false ); }
SpewRetval_t _SpewMessage( const tchar* pMsg, ... ) { assert( false ); }
SpewRetval_t _DSpewMessage( const tchar* pGroupName, int level, const tchar* pMsg, ... ) { assert( false ); }
SpewRetval_t ColorSpewMessage( SpewType_t type, const Color* pColor, const tchar* pMsg, ... ) { assert( false ); }
void _ExitOnFatalAssert( const tchar* pFile, int line ) { assert( false ); }
bool ShouldUseNewAssertDialog() { return true; }

bool DoNewAssertDialog( const tchar* pFile, int line, const tchar* pExpression ) {
	using namespace std::string_literals;
	auto message{
		"---- Assertion Failed ----"
		"\nFile: "s + pFile +
		"\nLine: " + std::to_string( line ) +
		"\nAssert: " + pExpression +
		"--------------------------"
	};

	puts( message.c_str() );

	return false;
}

// TODO: Implement these
bool AreAllAssertsDisabled() {
	return g_bAssertionsDisabled;
}
void SetAllAssertsDisabled( bool bAssertsEnabled ) {
	g_bAssertionsDisabled = bAssertsEnabled;
}

void SetAssertFailedNotifyFunc( AssertFailedNotifyFunc_t pFunc ) {
	g_pAssertFailedListener = pFunc;
}
void CallAssertFailedNotifyFunc( const char* pchFile, int nLine, const char* pchMessage ) {
	if ( g_pAssertFailedListener )
		g_pAssertFailedListener( pchFile, nLine, pchMessage );
}

bool HushAsserts() {
	return CommandLine()->FindParm( "-hushasserts" ) != 0;
}

void SetAssertDialogParent( struct SDL_Window* pWindow ) {
	g_pDialogParent = pWindow;
}
struct SDL_Window* GetAssertDialogParent() {
	return g_pDialogParent;
}


static void SpewInternal( SpewType_t pType, const tchar* pMsg, const va_list& args ) {
	char buffer[MAX_PATH] { 0 };
	vsnprintf( buffer, sizeof( buffer ), pMsg, args );
	auto res{ g_pSpewOutFunction( pType, buffer ) };

	if ( res == SpewRetval_t::SPEW_CONTINUE )
		return;

	if ( res == SpewRetval_t::SPEW_ABORT ) {
		puts( "Fatal spew! Aborting execution." );
		exit( 1 );
	}

	DebuggerBreak();
}

void Msg( const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_MESSAGE, pMsg, args );
	va_end( args );
}
void DMsg( const tchar* pGroupName, int level, const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_MESSAGE, pMsg, args );
	va_end( args );
}

void Warning( const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_WARNING, pMsg, args );
	va_end( args );
}
void DWarning( const tchar* pGroupName, int level, const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_WARNING, pMsg, args );
	va_end( args );
}

void Log( const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_LOG, pMsg, args );
	va_end( args );
}
void DLog( const tchar* pGroupName, int level, const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_LOG, pMsg, args );
	va_end( args );
}

void Error( const tchar* pMsg, ... ) {
	va_list args;
	va_start( args, pMsg );
	SpewInternal( SpewType_t::SPEW_ERROR, pMsg, args );
	va_end( args );
}

void COM_TimestampedLog( const char* fmt, ... ) {
	AssertUnreachable();
}

// TODO: Figure out how to do these without crashing the engine
void _AssertValidReadPtr( void* ptr, int count ) { }
void _AssertValidWritePtr( void* ptr, int count ) { }
void _AssertValidReadWritePtr( void* ptr, int count ) { }
void AssertValidStringPtr( const tchar* ptr, int maxchar ) { }


