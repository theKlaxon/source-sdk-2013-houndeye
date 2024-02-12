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
	long ThreadInterlockedIncrement( long volatile* pIt ) { // NOLINT(*-non-const-parameter)
		// type __atomic_add_fetch(type *ptr, type val, int memorder)
		return __atomic_add_fetch( pIt, 1, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedDecrement( long volatile* pIt ) { // NOLINT(*-non-const-parameter)
		// type __atomic_sub_fetch(type *ptr, type val, int memorder)
		return __atomic_sub_fetch( pIt, 1, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedExchange( long volatile* pIt, long pValue ) { // NOLINT(*-non-const-parameter)
		// type __atomic_exchange_n(type *ptr, type val, int memorder)
		return __atomic_exchange_n( pIt, pValue, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedExchangeAdd( long volatile* pIt, long pValue ) { // NOLINT(*-non-const-parameter)
		// type __atomic_fetch_add(type *ptr, type val, int memorder)
		return __atomic_fetch_add( pIt, pValue, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedCompareExchange( long volatile* pIt, long pValue, long comperand ) { // NOLINT(*-non-const-parameter)
		long last;
		long expected = comperand;
		do {
			// void __atomic_load (type *ptr, type *ret, int memorder)
			__atomic_load( pIt, &last, __ATOMIC_RELAXED );
			// bool __atomic_compare_exchange_n(type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)
		} while (! __atomic_compare_exchange_n( pIt, &expected, pValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED ) );
		return last;
	}
	bool ThreadInterlockedAssignIf( long volatile* pIt, long value, long comperand ) { // NOLINT(*-non-const-parameter)
		// bool __atomic_compare_exchange_n(type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)
		return __atomic_compare_exchange_n( pIt, &comperand, value, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED );
	}
#endif
int64 ThreadInterlockedIncrement64( int64 volatile* pIt ) {// NOLINT(*-non-const-parameter)
	return __atomic_add_fetch( pIt, 1, __ATOMIC_ACQ_REL );
}
int64 ThreadInterlockedDecrement64( int64 volatile* pIt ) {// NOLINT(*-non-const-parameter)
	return __atomic_sub_fetch( pIt, 1, __ATOMIC_ACQ_REL );
}
int64 ThreadInterlockedCompareExchange64( int64 volatile* pIt, int64 pValue, int64 comperand ) {// NOLINT(*-non-const-parameter)
	int64 last;
	int64 expected = comperand;
	do __atomic_load( pIt, &last, __ATOMIC_RELAXED );
	while (! __atomic_compare_exchange_n( pIt, &expected, pValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED ) );
	return last;
}
int64 ThreadInterlockedExchange64( int64 volatile* pIt, int64 pValue ) {                        // NOLINT(*-non-const-parameter)
	return __atomic_exchange_n( pIt, pValue, __ATOMIC_ACQ_REL );
}
int64 ThreadInterlockedExchangeAdd64( int64 volatile* pIt, int64 pValue ) {// NOLINT(*-non-const-parameter)
	return __atomic_fetch_add( pIt, pValue, __ATOMIC_ACQ_REL );
}
bool ThreadInterlockedAssignIf64( volatile int64* pDest, int64 pValue, int64 comperand ) { // NOLINT(*-non-const-parameter)
	return __atomic_compare_exchange_n( pDest, &comperand, pValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED );
}
