//
// Created by ENDERZOMBI102 on 10/02/2024.
//
#include "threadtools.h"
#if IsWindows()
	#include <processthreadsapi.h>
#endif

static ThreadedLoadLibraryFunc_t g_pThrLoadLibFunc;


ThreadHandle_t CreateSimpleThread( ThreadFunc_t, void* pParam, ThreadId_t* pID, unsigned stackSize );
ThreadHandle_t CreateSimpleThread( ThreadFunc_t, void* pParam, unsigned stackSize );
bool ReleaseThreadHandle( ThreadHandle_t );

void ThreadSleep( unsigned duration );
uint ThreadGetCurrentId() {
	#if IsWindows()
		return static_cast<uint>( GetCurrentThreadId() );
	#elif IsPosix()
		return pthread_self();
	#endif
}
ThreadHandle_t ThreadGetCurrentHandle();
int ThreadGetPriority( ThreadHandle_t hThread );
bool ThreadSetPriority( ThreadHandle_t hThread, int priority );
bool ThreadInMainThread();
void DeclareCurrentThreadIsMainThread();

void SetThreadedLoadLibraryFunc( ThreadedLoadLibraryFunc_t func ) {
	g_pThrLoadLibFunc = func;
}
ThreadedLoadLibraryFunc_t GetThreadedLoadLibraryFunc() {
	return g_pThrLoadLibFunc;
}

bool ThreadJoin( ThreadHandle_t, unsigned timeout );
void ThreadDetach( ThreadHandle_t );

void ThreadSetDebugName( ThreadId_t id, const char* pszName );

void ThreadSetAffinity( ThreadHandle_t hThread, int nAffinityMask );

#if IsWindows()
	int ThreadWaitForObjects( int nEvents, const HANDLE* pHandles, bool bWaitAll, unsigned timeout );
#endif

#if !defined( USE_INTRINSIC_INTERLOCKED )
	long ThreadInterlockedIncrement( long volatile* ) { }
	long ThreadInterlockedDecrement( long volatile* );
	long ThreadInterlockedExchange( long volatile*, long value ) { }
	long ThreadInterlockedExchangeAdd( long volatile*, long value );
	long ThreadInterlockedCompareExchange( long volatile*, long value, long comperand );
	bool ThreadInterlockedAssignIf( long volatile*, long value, long comperand ) { }
#endif
int64 ThreadInterlockedIncrement64( int64 volatile* );
int64 ThreadInterlockedDecrement64( int64 volatile* );
int64 ThreadInterlockedCompareExchange64( int64 volatile*, int64 value, int64 comperand );
int64 ThreadInterlockedExchange64( int64 volatile*, int64 value );
int64 ThreadInterlockedExchangeAdd64( int64 volatile*, int64 value );
bool ThreadInterlockedAssignIf64( volatile int64* pDest, int64 value, int64 comperand ) { }
